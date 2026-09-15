# Console controls

JoF EJK exposes client controls for presentation and movement tools plus server controls for gameplay, races, administration, and compatibility. Enter cvars and commands in the in-game console with a leading `/`.

## Frequently used client cvars

| Cvar | Default | Purpose |
| --- | ---: | --- |
| `cg_movementKeys` | `0` | Show movement-key input. |
| `cg_speedometer` | `0` | Show the speedometer. |
| `cg_raceTimer` | `2` | Configure the race timer display. |
| `cg_strafeHelper` | `0` | Enable and configure strafe-helper modes as a bit value. |
| `cg_groundspeedometer` | `0` | Show horizontal ground speed. |
| `cg_drawJumpHeight` | `0` | Display measured jump height. |
| `cg_drawJumpDistance` | `0` | Display measured jump distance. |
| `cg_hitsounds` | `0` | Select a hit-sound mode. |
| `cg_brightSkins` | `0` | Configure bright player skins. |
| `cg_forceAllyModel` | `none` | Force a model for allies. |
| `cg_forceEnemyModel` | `none` | Force a model for enemies. |
| `cg_autoRecordDemo` | `0` | Automatically record demos. |
| `cg_autoScreenshot` | `0` | Take a screenshot at the end of a round. |
| `cg_zoomSensitivity` | `2.5` | Set sensitivity while zoomed. |
| `cg_logChat` | `1` | Write chat messages to a log. |
| `cg_predictKnockback` | `0` | Toggle client-side knockback prediction. |
| `cl_maxPackets` | `30` | Set the outgoing packet cap. |
| `cl_timeNudge` | `0` | Adjust snapshot interpolation timing. |

## Useful client commands

| Command | Purpose |
| --- | --- |
| `/modversion` | Print the build version and revision. |
| `/serverconfig` | Inspect compatible server configuration information. |
| `/autoLogin` | Attempt a configured server-specific login. |
| `/strafeHelper` | Configure the strafe helper. |
| `/addCheckpoint` | Add a local checkpoint. |
| `/listCheckpoints` | List local checkpoints. |
| `/teleToCheckpoint` | Teleport to a local checkpoint where supported. |
| `/clearTrail` | Clear the current strafe trail. |
| `/loadTrail` | Load a saved strafe trail. |
| `/followRedFlag` | Follow the red-flag carrier while spectating. |
| `/followBlueFlag` | Follow the blue-flag carrier while spectating. |
| `/followFastest` | Follow the fastest racer while spectating. |

## Server controls

Server operators can tune CTF, saber and Force behavior, weapons, movement, duels, administration, race/accounts, bots, Elo ranking, and compatibility behavior. Common entry points include:

| Control | Purpose |
| --- | --- |
| `g_raceMode` | Disable, force, or let players toggle race mode. |
| `g_movementStyle` | Select the server movement style. |
| `g_allowGrapple` | Enable supported grappling styles. |
| `g_flipKick` | Configure flip-kick behavior. |
| `g_unlagged` | Enable selected unlagged systems as a bit value. |
| `g_tweakSaber` | Enable saber behavior tweaks configured with `/tweakSaber`. |
| `g_tweakForce` | Enable Force behavior tweaks configured with `/tweakForce`. |
| `g_tweakWeapons` | Enable weapon behavior tweaks configured with `/tweakWeapons`. |
| `g_allowRegistration` | Configure account registration and clan creation. |
| `g_eloRanking` | Enable built-in duel Elo ranking. |
| `/pause` | Pause or unpause a match. |
| `/resetScores` | Reset scores without changing the map. |
| `/amlogin` | Log in with an administrative level. |
| `/ammap` | Change the map through the admin system. |
| `/amtele` | Use the supported admin teleport command. |

!!! caution "Check ownership before changing a setting"
    A `cg_` control is normally client-side, `g_` controls usually belong to game code, and `sv_` controls belong to the server engine. A server may override, restrict, or omit game-module features.

## Complete inherited reference

The repository also carries a longer jaPRO-origin reference covering all documented server cvars, RCON commands, game commands, emotes, client cvars, map entities, and bit-value options.

[Open the full jaPRO reference](japro-reference.md){ .md-button .md-button--primary }

When a description disagrees with current behavior, treat the source code for your exact build as authoritative and open an issue with the output of `/modversion`.

