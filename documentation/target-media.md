# Player-specific media with `target_media`

`target_media` is a server-side logical entity for music, sound, voice and visual
effects. It uses existing Jedi Academy client messages and events. Only the game
module needs updating; players need the referenced assets in a loaded PK3, but
do not need a new cgame or engine. This feature is independent of the dialogue
`Sound` key.

Music uses the background music player and its music volume setting. It loops
until restored, independently of any NPC's lifetime or position. Every activation
restarts it from the beginning, including when the same track is already playing.
There is no toggle/pause behavior and no repeated server command needed to loop.

## Your sparring encounter

Place these two entities in the map or entity override:

```text
{
    "classname" "target_media"
    "targetname" "sparring_music_start"
    "type" "music"
    "file" "music/encounters/sparring.mp3"
    "recipients" "activator"
    "target" "sparring_swap_npc"
}
{
    "classname" "target_media"
    "targetname" "sparring_music_end"
    "action" "restore"
    "controller" "sparring_music_start"
}
```

`music/encounters/sparring.mp3` is an example asset path; package your own track.
`sparring_swap_npc` is your existing target/script that removes the neutral NPC
and spawns the combat NPC in its place. The NPC swap and the 50%-health condition
remain under your map/ICARUS logic's control.

1. When the player accepts the fight, activate `sparring_music_start` **with that
   player as activator**. A native dialogue node can do this with
   `fire sparring_music_start`. The media controller remembers that player,
   starts their looping music, then fires `sparring_swap_npc`, preserving the
   player activator.
2. When your combat NPC reaches 50% health, have its script `use` the targetname
   `sparring_music_end`, then remove the combat NPC. This does not require a death
   event. The end trigger can be fired by the NPC, a relay, or any other script;
   it does not need the original player as activator.
3. The end controller restores the current map music for the players remembered
   by `sparring_music_start`. Other players' music is untouched.

An ICARUS `use` initiated by an NPC passes the NPC as activator. That works for
the end controller, but cannot identify the player for the initial `activator`
selection. Start the music in the player-triggered path before handing control
to the NPC script.

Removing an NPC by itself does **not** end its encounter: call the end target in
all of your script's exit/abort paths. Player death, switching to spectator, and
removing the start controller automatically stop its owned media and restore
map music. Disconnects and map restarts clear the remembered player slots.

## Keys

| Key | Meaning |
| --- | --- |
| `targetname` | Name used by map targets, scripts, and a stop controller. Give each independently controlled encounter its own start targetname. |
| `action` | `play` (default), `stop`, or `restore`. `restore` is an alias for `stop`. |
| `type` | For play: `music` (default), `sound`, `voice`, or `effect`. |
| `file` | Required for play. Game-relative asset path, fewer than 64 characters. Use forward slashes, letters, numbers, `_`, `-`, and `.`; no spaces or `..`. |
| `loop` | For music: optional loop track. `file` plays first, then `loop` repeats. Defaults to `file`, repeating the same track. |
| `recipients` | `activator`, `all`, `red`, `blue`, `clients`, or `saved`. Play defaults to `activator`; stop defaults to `saved`. |
| `clients` | With `recipients clients`: space-separated client slots, e.g. `"0 3 7"`. Only connected slots are selected. |
| `controller` | Required for stop/restore. The start controller's `targetname`. |
| `position` | For effects: `origin` (default) or `player` (each recipient's current position and view direction). |
| `origin`, `angles` | Position and direction of an effect when `position origin` is used. |
| `target` | Optional map target to fire after the operation, preserving the caller's activator. |

`all`, `red`, `blue`, and explicit slots select players at activation time. They
do not automatically enroll players who join or change teams later. A start
controller may remember multiple players. Its default `saved` stop restores all
of them; use `recipients activator` on the stop controller when stopping only
the triggering player's participation. A stop never affects a player who is
not currently owned by its referenced start controller.

Each player has one managed music track, one sound channel and one voice channel.
Starting another controller of the same type replaces the previous one for that
player. A late stop from the old encounter cannot stop the newer encounter's
media. Stopping the newer music returns to map music; it does not stack/resume
older encounter tracks.

## Sound, voice and effects

```text
{
    "classname" "target_media"
    "targetname" "private_warning"
    "type" "voice"
    "file" "sound/voice/warning.mp3"
    "recipients" "activator"
}
{
    "classname" "target_media"
    "targetname" "red_team_spark"
    "type" "effect"
    "file" "effects/example/spark.efx"
    "recipients" "red"
    "position" "player"
}
```

These example paths also need corresponding assets. Sound and voice are
one-shots played at each recipient's location. Another play on the same type
replaces the previous sound from the beginning; a stop controller mutes that
type's channel. These use the native `CHAN_MENU1` and `CHAN_VOICE` channels,
which are also used by ordinary game sounds. Music is the continuously looping
mode; this entity does not use `target_speaker`'s looping sound mechanism.

Effects are one-shot emissions. Existing particles finish according to their
effect definition; a stop cannot retract them. Their position is sampled at
activation, not attached to a moving player. Sound/voice/effects use native
single-client snapshot filtering: spectators following the selected player may
also receive those events. Music is delivered to the actual selected connection
and is not shared with following spectators.

## Restoration and compatibility details

The authoritative map `CS_MUSIC` is never replaced by a private encounter. Restore
reads its current value, including changes made by `target_play_music`. Global
map music changes reapply active private tracks afterward, restarting those
tracks; they do not replace the encounter's ownership. Restoration restarts the
map track or dynamic music set, rather than recovering its old playback time.

On legacy JA engines, an empty music configstring does not stop a running stream.
For a map with no music, restoration sends `none`, which takes the native
missing-music stop path. Such engines may print a missing-music diagnostic while
returning to silence. Missing or undecodable actual media files are likewise
handled by the existing client; there is no playback-success acknowledgement.

The controller is registered in the logical entity pool and has no model,
collision or network presence. Music requires no temporary network entities.
Sound/voice/effects use short-lived standard event entities. Avoid
`script_targetname` and `nological 1` on the controller: the existing spawn system
forces those entities into normal slots. ICARUS can still activate it by its
ordinary `targetname`.
