/* Server-only, logical media controllers using the existing JA protocol. */
#include "g_local.h"
#include "g_media.h"

typedef enum {
	MEDIA_MUSIC,
	MEDIA_SOUND,
	MEDIA_VOICE,
	MEDIA_EFFECT,
	MEDIA_NUM_OWNED = MEDIA_EFFECT
} mediaType_t;

typedef enum {
	MEDIA_ACTIVATOR,
	MEDIA_ALL,
	MEDIA_RED,
	MEDIA_BLUE,
	MEDIA_CLIENTS,
	MEDIA_SAVED
} mediaRecipients_t;

typedef struct {
	qboolean configured;
	qboolean stop;
	mediaType_t type;
	mediaRecipients_t recipients;
	qboolean clients[MAX_CLIENTS];
	char file[MAX_QPATH * 2];
	char controller[MAX_QPATH];
	int assetIndex;
	qboolean atPlayer;
} mediaTarget_t;

static mediaTarget_t s_media[MAX_ENTITIESTOTAL];
/* Ownership also remembers recipients when a later script has no player activator.
 * A newer controller replaces an older one; stopping the old one is then a no-op. */
static int s_owners[MAX_CLIENTS][MEDIA_NUM_OWNED];

static qboolean Media_Connected( int clientNum ) {
	return clientNum >= 0 && clientNum < level.maxclients &&
		g_entities[clientNum].inuse && g_entities[clientNum].client &&
		g_entities[clientNum].client->pers.connected == CON_CONNECTED;
}

static qboolean Media_Select( const mediaTarget_t *media, int clientNum, gentity_t *activator ) {
	if ( !Media_Connected( clientNum ) ) return qfalse;
	switch ( media->recipients ) {
	case MEDIA_ACTIVATOR: return activator == &g_entities[clientNum];
	case MEDIA_ALL: case MEDIA_SAVED: return qtrue;
	case MEDIA_RED: return g_entities[clientNum].client->sess.sessionTeam == TEAM_RED;
	case MEDIA_BLUE: return g_entities[clientNum].client->sess.sessionTeam == TEAM_BLUE;
	case MEDIA_CLIENTS: return media->clients[clientNum];
	default: return qfalse;
	}
}

static void Media_Music( int clientNum, const char *music ) {
	/* Send directly: SetConfigstring would change the music for everybody and
	 * suppress identical updates. Each cs command makes an unmodified cgame
	 * force-start its background track, including repeated uses of this entity.
	 * Legacy JA does not stop a stream for an empty music string. Its "none"
	 * fallback closes the stream, also restoring silence on maps without music. */
	trap->SendServerCommand( clientNum, va( "cs %d \"%s\"", CS_MUSIC, music[0] ? music : "none" ) );
}

static gentity_t *Media_Event( int clientNum, int event, vec3_t origin ) {
	gentity_t *temp = G_TempEntity( origin, event );
	/* BROADCAST bypasses PVS; SINGLECLIENT still restricts the recipient. */
	temp->r.svFlags |= SVF_SINGLECLIENT | SVF_BROADCAST;
	temp->r.singleClient = clientNum;
	return temp;
}

static void Media_Stop( int clientNum, mediaType_t type ) {
	gentity_t *temp;
	char music[MAX_STRING_CHARS];
	if ( s_owners[clientNum][type] < 0 ) return;
	s_owners[clientNum][type] = -1;
	if ( !Media_Connected( clientNum ) ) return;
	if ( type == MEDIA_MUSIC ) {
		trap->GetConfigstring( CS_MUSIC, music, sizeof( music ) );
		Media_Music( clientNum, music );
	} else {
		temp = Media_Event( clientNum, EV_MUTE_SOUND, g_entities[clientNum].r.currentOrigin );
		temp->s.trickedentindex2 = clientNum;
		temp->s.trickedentindex = type == MEDIA_VOICE ? CHAN_VOICE : CHAN_MENU1;
	}
}

static void Media_Play( gentity_t *ent, int clientNum ) {
	int entityNum = (int)( ent - g_entities );
	mediaTarget_t *media = &s_media[entityNum];
	gentity_t *player = &g_entities[clientNum];
	gentity_t *temp;
	if ( media->type < MEDIA_NUM_OWNED ) s_owners[clientNum][media->type] = entityNum;
	if ( media->type == MEDIA_MUSIC ) {
		Media_Music( clientNum, media->file );
	} else if ( media->type == MEDIA_EFFECT ) {
		temp = Media_Event( clientNum, EV_PLAY_EFFECT_ID,
			media->atPlayer ? player->r.currentOrigin : ent->s.origin );
		temp->s.eventParm = media->assetIndex;
		VectorCopy( media->atPlayer ? player->client->ps.viewangles : ent->s.angles, temp->s.angles );
	} else {
		/* A fixed entity/channel pair makes another play replace the old sound.
		 * The recipient hears it at their own position, independently of the NPC. */
		temp = Media_Event( clientNum, EV_ENTITY_SOUND, player->r.currentOrigin );
		temp->s.eventParm = media->assetIndex;
		temp->s.clientNum = clientNum;
		temp->s.trickedentindex = media->type == MEDIA_VOICE ? CHAN_VOICE : CHAN_MENU1;
	}
}

static void Use_Target_Media( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	mediaTarget_t *media = &s_media[ent - g_entities];
	int clientNum;
	if ( media->stop ) {
		gentity_t *controller = NULL;
		while ( ( controller = G_Find( controller, FOFS( targetname ), media->controller ) ) != NULL ) {
			int owner = (int)( controller - g_entities );
			mediaTarget_t *source = &s_media[owner];
			if ( !source->configured || source->stop || source->type >= MEDIA_NUM_OWNED ) continue;
			for ( clientNum = 0; clientNum < level.maxclients; ++clientNum ) {
				if ( s_owners[clientNum][source->type] == owner && Media_Select( media, clientNum, activator ) )
					Media_Stop( clientNum, source->type );
			}
		}
	} else {
		for ( clientNum = 0; clientNum < level.maxclients; ++clientNum ) {
			if ( Media_Select( media, clientNum, activator ) ) Media_Play( ent, clientNum );
		}
	}
	G_UseTargets( ent, activator );
}

void G_MediaInit( void ) {
	memset( s_media, 0, sizeof( s_media ) );
	memset( s_owners, -1, sizeof( s_owners ) );
}

void G_MediaShutdown( void ) {
	int clientNum;
	/* map_restart need not resend an unchanged global CS_MUSIC. Restore the
	 * clients' private copies before the VM forgets who had an override. */
	for ( clientNum = 0; clientNum < level.maxclients; ++clientNum ) {
		Media_Stop( clientNum, MEDIA_MUSIC );
	}
}

void G_MediaClientDisconnect( int clientNum ) {
	if ( clientNum < 0 || clientNum >= MAX_CLIENTS ) return;
	memset( s_owners[clientNum], -1, sizeof( s_owners[clientNum] ) );
}

void G_MediaEntityFree( gentity_t *ent ) {
	int entityNum = (int)( ent - g_entities );
	int clientNum, type;
	if ( !s_media[entityNum].configured ) return;
	for ( clientNum = 0; clientNum < level.maxclients; ++clientNum ) {
		for ( type = 0; type < MEDIA_NUM_OWNED; ++type ) {
			if ( s_owners[clientNum][type] == entityNum ) Media_Stop( clientNum, (mediaType_t)type );
		}
	}
	memset( &s_media[entityNum], 0, sizeof( s_media[entityNum] ) );
}

void G_MediaRunFrame( void ) {
	int clientNum, type;
	for ( clientNum = 0; clientNum < level.maxclients; ++clientNum ) {
		if ( !Media_Connected( clientNum ) ) {
			G_MediaClientDisconnect( clientNum );
		} else if ( g_entities[clientNum].health <= 0 ||
			g_entities[clientNum].client->sess.sessionTeam == TEAM_SPECTATOR ) {
			for ( type = 0; type < MEDIA_NUM_OWNED; ++type ) Media_Stop( clientNum, (mediaType_t)type );
		}
	}
}

void G_MediaMapMusicChanged( void ) {
	int clientNum;
	/* A global target_play_music still sets the default for new players and
	 * future restores. Reapply active private tracks after that global update. */
	for ( clientNum = 0; clientNum < level.maxclients; ++clientNum ) {
		int owner = s_owners[clientNum][MEDIA_MUSIC];
		if ( owner >= 0 && Media_Connected( clientNum ) ) Media_Music( clientNum, s_media[owner].file );
	}
}

static qboolean Media_Path( const char *path ) {
	const unsigned char *p = (const unsigned char *)path;
	if ( !path[0] || strlen( path ) >= MAX_QPATH || strstr( path, ".." ) ||
		path[0] == '/' || path[0] == '\\' || path[0] == '*' ) return qfalse;
	for ( ; *p; ++p ) {
		if ( !( isalnum( *p ) || *p == '/' || *p == '_' || *p == '-' || *p == '.' ) ) return qfalse;
	}
	return qtrue;
}

/*QUAKED target_media (0.2 0.7 1.0) (-8 -8 -8) (8 8 8)
Logical, server-only playback for selected players. See documentation/target-media.md.
"action" play (default), stop, or restore (alias for stop).
"type" music (default), sound, voice, effect. "file" game-relative asset path.
"loop" optional music loop path; defaults to file. Music always loops.
"recipients" activator (play default), saved (stop default), all, red, blue, clients.
"clients" space-separated client slots when recipients is clients.
"controller" targetname of play controller to stop/restore, remembering its players.
"position" origin (effect default) or player. Sounds/voice are player-local.
"target" optional map target fired after the operation, preserving the activator.
*/
void SP_target_media( gentity_t *ent ) {
	mediaTarget_t *media = &s_media[ent - g_entities];
	char *value, *file, *loop;
	const char *cursor;
	qboolean haveClient = qfalse;
	memset( media, 0, sizeof( *media ) );
	G_SpawnString( "action", "play", &value );
	if ( !Q_stricmp( value, "stop" ) || !Q_stricmp( value, "restore" ) ) media->stop = qtrue;
	else if ( Q_stricmp( value, "play" ) ) goto invalid;
	G_SpawnString( "recipients", media->stop ? "saved" : "activator", &value );
	if ( !Q_stricmp( value, "activator" ) ) media->recipients = MEDIA_ACTIVATOR;
	else if ( !Q_stricmp( value, "all" ) ) media->recipients = MEDIA_ALL;
	else if ( !Q_stricmp( value, "red" ) ) media->recipients = MEDIA_RED;
	else if ( !Q_stricmp( value, "blue" ) ) media->recipients = MEDIA_BLUE;
	else if ( !Q_stricmp( value, "saved" ) && media->stop ) media->recipients = MEDIA_SAVED;
	else if ( !Q_stricmp( value, "clients" ) ) {
		media->recipients = MEDIA_CLIENTS;
		G_SpawnString( "clients", "", &value );
		cursor = value;
		while ( 1 ) {
			const char *token = COM_ParseExt( &cursor, qtrue );
			char *end;
			long clientNum;
			if ( !token[0] ) break;
			clientNum = strtol( token, &end, 10 );
			if ( *end || clientNum < 0 || clientNum >= MAX_CLIENTS ) goto invalid;
			media->clients[clientNum] = qtrue;
			haveClient = qtrue;
		}
		if ( !haveClient ) goto invalid;
	} else goto invalid;

	if ( media->stop ) {
		G_SpawnString( "controller", "", &value );
		if ( !value[0] || strlen( value ) >= sizeof( media->controller ) ) goto invalid;
		Q_strncpyz( media->controller, value, sizeof( media->controller ) );
	} else {
		G_SpawnString( "type", "music", &value );
		if ( !Q_stricmp( value, "music" ) ) media->type = MEDIA_MUSIC;
		else if ( !Q_stricmp( value, "sound" ) ) media->type = MEDIA_SOUND;
		else if ( !Q_stricmp( value, "voice" ) ) media->type = MEDIA_VOICE;
		else if ( !Q_stricmp( value, "effect" ) ) media->type = MEDIA_EFFECT;
		else goto invalid;
		G_SpawnString( "file", "", &file );
		if ( !Media_Path( file ) ) goto invalid;
		G_SpawnString( "loop", file, &loop );
		if ( !Media_Path( loop ) ) goto invalid;
		if ( media->type == MEDIA_MUSIC ) {
			Com_sprintf( media->file, sizeof( media->file ), "%s %s", file, loop );
		} else {
			Q_strncpyz( media->file, file, sizeof( media->file ) );
			media->assetIndex = media->type == MEDIA_EFFECT ? G_EffectIndex( file ) : G_SoundIndex( file );
			if ( !media->assetIndex ) goto invalid;
		}
		G_SpawnString( "position", "origin", &value );
		if ( !Q_stricmp( value, "player" ) ) media->atPlayer = qtrue;
		else if ( Q_stricmp( value, "origin" ) ) goto invalid;
	}
	media->configured = qtrue;
	ent->r.svFlags |= SVF_NOCLIENT;
	ent->use = Use_Target_Media;
	return;

invalid:
	trap->Print( "target_media '%s': invalid settings; see documentation/target-media.md\n",
		ent->targetname ? ent->targetname : "<unnamed>" );
	G_FreeEntity( ent );
}
