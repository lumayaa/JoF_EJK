#include "game/g_local.h"
#include "game/g_media.h"

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "FAIL %d: %s\n", __LINE__, #x); exit(1); } } while (0)

gentity_t g_entities[MAX_ENTITIESTOTAL];
level_locals_t level;
static gclient_t clients[MAX_CLIENTS];
static gameImport_t imports;
gameImport_t *trap = &imports;
static char mapMusic[MAX_STRING_CHARS] = "music/map/intro.mp3 music/map/loop.mp3";
static char messages[128][MAX_STRING_CHARS];
static int recipients[128], messageCount, eventCount, warnings, forwards;
static gentity_t events[128];
static gentity_t *lastActivator;
static char keys[32][64], values[32][MAX_STRING_CHARS];
static int numKeys, nextController = MAX_GENTITIES;

void QDECL Com_Printf(const char *fmt, ...) { (void)fmt; ++warnings; }
void QDECL Com_Error(int level, const char *fmt, ...) {
  va_list ap;
  (void)level;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  exit(1);
}
static void SendServerCommand(int clientNum, const char *text) {
  CHECK(clientNum >= 0 && clientNum < level.maxclients); // No broadcast commands.
  CHECK(messageCount < ARRAY_LEN(messages));
  recipients[messageCount] = clientNum;
  Q_strncpyz(messages[messageCount++], text, sizeof(messages[0]));
}
static void GetConfigstring(int index, char *out, int size) {
  CHECK(index == CS_MUSIC);
  Q_strncpyz(out, mapMusic, size);
}
qboolean G_SpawnString(const char *key, const char *defaultString, char **out) {
  int i;
  for (i = 0; i < numKeys; ++i) {
    if (!strcmp(keys[i], key)) { *out = values[i]; return qtrue; }
  }
  *out = (char *)defaultString;
  return qfalse;
}
gentity_t *G_Find(gentity_t *from, int fieldofs, const char *match) {
  int i = from ? (int)(from - g_entities) + 1 : 0;
  CHECK(fieldofs == FOFS(targetname));
  for (; i < MAX_ENTITIESTOTAL; ++i) {
    if (g_entities[i].inuse && g_entities[i].targetname && !strcmp(g_entities[i].targetname, match))
      return &g_entities[i];
  }
  return NULL;
}
void G_UseTargets(gentity_t *ent, gentity_t *activator) {
  if (ent->target) { ++forwards; lastActivator = activator; }
}
void G_FreeEntity(gentity_t *ent) {
  G_MediaEntityFree(ent);
  ent->inuse = qfalse;
  ent->use = NULL;
}
int G_SoundIndex(const char *name) { CHECK(name && *name); return 11; }
int G_EffectIndex(const char *name) { CHECK(name && *name); return 22; }
gentity_t *G_TempEntity(vec3_t origin, int event) {
  gentity_t *ent;
  CHECK(eventCount < ARRAY_LEN(events));
  ent = &events[eventCount++];
  memset(ent, 0, sizeof(*ent));
  ent->s.eType = ET_EVENTS + event;
  VectorCopy(origin, ent->s.origin);
  return ent;
}
static gentity_t *Spawn(const char *name, const char *settings) {
  gentity_t *ent = &g_entities[nextController++];
  numKeys = 0;
  while (1) {
    const char *token = COM_ParseExt(&settings, qtrue);
    if (!token[0]) break;
    CHECK(numKeys < ARRAY_LEN(keys));
    Q_strncpyz(keys[numKeys], token, sizeof(keys[0]));
    token = COM_ParseExt(&settings, qtrue);
    Q_strncpyz(values[numKeys++], token, sizeof(values[0]));
  }
  memset(ent, 0, sizeof(*ent));
  ent->inuse = qtrue;
  ent->isLogical = qtrue;
  ent->s.number = (int)(ent - g_entities);
  ent->targetname = (char *)name;
  SP_target_media(ent);
  return ent;
}
static void Use(gentity_t *ent, gentity_t *activator) {
  CHECK(ent->inuse && ent->use && ent->isLogical && (ent->r.svFlags & SVF_NOCLIENT));
  ent->use(ent, NULL, activator);
}
static void ExpectMusic(int index, int clientNum, const char *music) {
  CHECK(index < messageCount && recipients[index] == clientNum);
  CHECK(!strcmp(messages[index], va("cs %d \"%s\"", CS_MUSIC, music)));
}
static void Encounter(void) {
  gentity_t *start = Spawn("fight", "type music file music/fight.mp3");
  gentity_t *end = Spawn("fight_end", "action restore controller fight");
  gentity_t *npc = &g_entities[100];
  int i;
  start->target = "swap_npc";
  Use(start, &g_entities[3]);
  CHECK(messageCount == 1 && forwards == 1 && lastActivator == &g_entities[3]);
  ExpectMusic(0, 3, "music/fight.mp3 music/fight.mp3");
  CHECK(!strcmp(mapMusic, "music/map/intro.mp3 music/map/loop.mp3"));
  for (i = 0; i < 12000; ++i) G_MediaRunFrame();
  CHECK(messageCount == 1); // The client loops; no frame-by-frame commands.
  Use(start, &g_entities[3]);
  CHECK(messageCount == 2);
  ExpectMusic(1, 3, "music/fight.mp3 music/fight.mp3"); // Identical use still forces a restart.
  npc->inuse = qtrue;
  npc->health = 50; // NPC script calls the end target when it reaches its threshold.
  Use(end, npc);
  CHECK(messageCount == 3);
  ExpectMusic(2, 3, mapMusic);
  Use(end, NULL);
  CHECK(messageCount == 3); // Idempotent end, without needing the player activator.
  CHECK(eventCount == 0); // Music/controllers never allocate a network entity.
}
static void Ownership(void) {
  gentity_t *a = Spawn("a", "file music/a.mp3");
  gentity_t *b = Spawn("b", "file music/b.mp3 loop music/b_loop.mp3");
  gentity_t *stopA = Spawn("stop_a", "action stop controller a");
  gentity_t *stopB = Spawn("stop_b", "action stop controller b");
  Use(a, &g_entities[1]);
  Use(a, &g_entities[2]);
  Use(b, &g_entities[1]);
  Use(stopA, NULL);
  CHECK(messageCount == 4);
  ExpectMusic(3, 2, mapMusic); // A cannot stop B's newer override on player 1.
  Q_strncpyz(mapMusic, "music/new_map.mp3", sizeof(mapMusic));
  G_MediaMapMusicChanged();
  CHECK(messageCount == 5);
  ExpectMusic(4, 1, "music/b.mp3 music/b_loop.mp3");
  Use(stopB, NULL);
  ExpectMusic(5, 1, mapMusic); // Restore the current map track, not an old cached one.
  Use(a, &g_entities[0]);
  mapMusic[0] = '\0';
  Use(stopA, NULL);
  ExpectMusic(7, 0, "none"); // Legacy empty configstrings do not stop a running stream.
}
static void Recipients(void) {
  gentity_t *red = Spawn("red", "file music/red.mp3 recipients red");
  gentity_t *blue = Spawn("blue", "file music/blue.mp3 recipients blue");
  gentity_t *all = Spawn("all", "file music/all.mp3 recipients all");
  gentity_t *slots = Spawn("slots", "file music/slots.mp3 recipients clients clients \"0 3 3\"");
  gentity_t *stop = Spawn("stop", "action stop controller slots recipients activator");
  clients[0].sess.sessionTeam = clients[2].sess.sessionTeam = TEAM_RED;
  clients[1].sess.sessionTeam = TEAM_BLUE;
  Use(red, NULL);
  CHECK(messageCount == 2 && recipients[0] == 0 && recipients[1] == 2);
  Use(blue, NULL);
  CHECK(messageCount == 3 && recipients[2] == 1);
  clients[1].pers.connected = CON_CONNECTING;
  Use(all, NULL);
  CHECK(messageCount == 6 && recipients[3] == 0 && recipients[4] == 2 && recipients[5] == 3);
  Use(slots, NULL);
  CHECK(messageCount == 8 && recipients[6] == 0 && recipients[7] == 3);
  Use(stop, &g_entities[3]);
  CHECK(messageCount == 9 && recipients[8] == 3);
  Use(stop, NULL);
  CHECK(messageCount == 9);
  Use(stop, &g_entities[0]);
  CHECK(messageCount == 10 && recipients[9] == 0);
}
static void Events(void) {
  gentity_t *sound = Spawn("sound", "type sound file sound/test.wav");
  gentity_t *voice = Spawn("voice", "type voice file sound/test.mp3");
  gentity_t *stopSound = Spawn("stop_sound", "action stop controller sound");
  gentity_t *effect = Spawn("effect", "type effect file effects/test.efx position player");
  gentity_t *worldEffect = Spawn("world", "type effect file effects/test.efx");
  int i;
  VectorSet(g_entities[3].r.currentOrigin, 10, 20, 30);
  Use(sound, &g_entities[3]);
  Use(sound, &g_entities[3]);
  CHECK(eventCount == 2);
  for (i = 0; i < 2; ++i) {
    CHECK(events[i].s.eType == ET_EVENTS + EV_ENTITY_SOUND);
    CHECK(events[i].s.clientNum == 3 && events[i].s.trickedentindex == CHAN_MENU1);
    CHECK(events[i].s.eventParm == 11);
  }
  Use(voice, &g_entities[3]);
  CHECK(events[2].s.trickedentindex == CHAN_VOICE);
  Use(stopSound, NULL);
  CHECK(events[3].s.eType == ET_EVENTS + EV_MUTE_SOUND);
  CHECK(events[3].s.trickedentindex2 == 3 && events[3].s.trickedentindex == CHAN_MENU1);
  Use(effect, &g_entities[3]);
  CHECK(events[4].s.eType == ET_EVENTS + EV_PLAY_EFFECT_ID && events[4].s.eventParm == 22);
  CHECK(VectorCompare(events[4].s.origin, g_entities[3].r.currentOrigin));
  VectorSet(worldEffect->s.origin, 40, 50, 60);
  Use(worldEffect, &g_entities[3]);
  CHECK(VectorCompare(events[5].s.origin, worldEffect->s.origin));
  for (i = 0; i < eventCount; ++i) {
    CHECK((events[i].r.svFlags & (SVF_SINGLECLIENT | SVF_BROADCAST)) == (SVF_SINGLECLIENT | SVF_BROADCAST));
    CHECK(events[i].r.singleClient == 3);
  }
  CHECK(messageCount == 0);
}
static void Cleanup(void) {
  gentity_t *start = Spawn("fight", "file music/fight.mp3");
  gentity_t *stop = Spawn("end", "action stop controller fight");
  Use(start, &g_entities[3]);
  G_MediaClientDisconnect(3);
  Use(stop, NULL);
  CHECK(messageCount == 1); // A reused slot must not inherit the old recipient.
  Use(start, &g_entities[3]);
  g_entities[3].health = 0;
  G_MediaRunFrame();
  G_MediaRunFrame();
  CHECK(messageCount == 3);
  ExpectMusic(2, 3, mapMusic);
  g_entities[3].health = 100;
  Use(start, &g_entities[3]);
  clients[3].sess.sessionTeam = TEAM_SPECTATOR;
  G_MediaRunFrame();
  CHECK(messageCount == 5);
  clients[3].sess.sessionTeam = TEAM_FREE;
  Use(start, &g_entities[3]);
  G_FreeEntity(start);
  CHECK(messageCount == 7);
  ExpectMusic(6, 3, mapMusic);
  Use(stop, NULL);
  CHECK(messageCount == 7);
  start = Spawn("fight", "file music/new_fight.mp3");
  Use(start, &g_entities[3]);
  clients[3].pers.connected = CON_DISCONNECTED;
  G_MediaRunFrame();
  clients[3].pers.connected = CON_CONNECTED;
  Use(stop, NULL);
  CHECK(messageCount == 8);
  Use(start, &g_entities[3]);
  G_MediaShutdown();
  CHECK(messageCount == 10);
  ExpectMusic(9, 3, mapMusic); // Fast restart can retain unchanged global configstrings.
  G_MediaInit();
  G_MediaMapMusicChanged();
  CHECK(messageCount == 10);
}
static void Validation(void) {
  const char *bad[] = {
    "file ../music/test.mp3", "file /music/test.mp3", "file *jump1.wav",
    "file \"music/test.mp3\ncommand\"", "file \"music/test;command.mp3\"",
    "file music/test.mp3 loop ../bad.mp3", "file music/test.mp3 action toggle",
    "file music/test.mp3 recipients saved", "file music/test.mp3 type unknown",
    "file music/test.mp3 recipients clients clients 32",
    "file music/test.mp3 recipients clients clients -1",
    "file music/test.mp3 recipients clients clients 1junk",
    "file music/test.mp3 recipients clients clients 99999999999999999999999",
    "file music/test.mp3 recipients clients", "action stop", "type music"
  };
  int i;
  for (i = 0; i < ARRAY_LEN(bad); ++i) CHECK(!Spawn("bad", bad[i])->inuse);
  CHECK(warnings == ARRAY_LEN(bad) && !messageCount && !eventCount);
  CHECK(Spawn("good", "type MUSIC file music/test.mp3")->inuse);
}
int main(int argc, char **argv) {
  int i;
  CHECK(argc == 2);
  imports.Print = Com_Printf;
  imports.GetConfigstring = GetConfigstring;
  imports.SendServerCommand = SendServerCommand;
  level.maxclients = 4;
  level.clients = clients;
  for (i = 0; i < level.maxclients; ++i) {
    g_entities[i].inuse = qtrue;
    g_entities[i].client = &clients[i];
    g_entities[i].health = 100;
    clients[i].pers.connected = CON_CONNECTED;
  }
  G_MediaInit();
  if (!strcmp(argv[1], "encounter")) Encounter();
  else if (!strcmp(argv[1], "ownership")) Ownership();
  else if (!strcmp(argv[1], "recipients")) Recipients();
  else if (!strcmp(argv[1], "events")) Events();
  else if (!strcmp(argv[1], "cleanup")) Cleanup();
  else if (!strcmp(argv[1], "validation")) Validation();
  else CHECK(0);
  puts("target_media checks passed");
  return 0;
}
