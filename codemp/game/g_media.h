#pragma once

typedef struct gentity_s gentity_t;

void G_MediaInit( void );
void G_MediaShutdown( void );
void G_MediaRunFrame( void );
void G_MediaClientDisconnect( int clientNum );
void G_MediaEntityFree( gentity_t *ent );
void G_MediaMapMusicChanged( void );
void SP_target_media( gentity_t *ent );
