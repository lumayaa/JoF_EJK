# Console controls

JoF EternalJK exposes client controls for presentation and movement tools, plus server controls for gameplay, races, administration, and compatibility. Open the in-game console with ++shift+grave++ and enter commands with a leading `/`.

To inspect a cvar without changing it, enter only its name—for example, `/cg_speedometer`. The console prints its current and default values.

## Frequently used client cvars

| Cvar | Default | Purpose |
| --- | ---: | --- |
| `cg_movementKeys` | `0` | Show movement-key input. |
| `cg_speedometer` | `0` | Configure speed, ground-speed, acceleration, and jump displays as a bit value; use `/speedometer`. |
| `cg_raceTimer` | `2` | Configure the on-screen race timer. |
| `cg_strafeHelper` | `3008` | Configure strafe-helper modes as a bit value; use `/strafeHelper`. |
| `cg_hitsounds` | `0` | Select a hit-sound mode. |
| `cg_forceAllyModel` | `none` | Force a model for allies. |
| `cg_forceEnemyModel` | `none` | Force a model for enemies. |
| `cg_drawPlayerNames` | `0` | Configure names drawn above players. |
| `cg_cosmetics` | `1` | Show supported player cosmetics. |
| `cg_autoRecordDemo` | `0` | Automatically record demos. |
| `cg_autoScreenshot` | `0` | Take a screenshot at the end of a round. |
| `cg_zoomSensitivity` | `0` | Override zoom sensitivity; `0` uses the calculated default. |
| `cg_logChat` | `1` | Configure chat logging as a bit value. |
| `cg_predictKnockback` | `0` | Toggle client-side knockback prediction. |
| `cl_maxPackets` | `125` | Set the outgoing packet cap. |
| `cl_timeNudge` | `0` | Adjust snapshot interpolation timing. |

!!! note "Defaults come from the current source tree"
    Older jaPRO documents list different defaults and a few cvars that this client no longer registers. The values above are taken from `codemp/cgame/cg_xcvar.h` in the current branch.

## Useful client commands

| Command | Purpose |
| --- | --- |
| `/modversion` | Print the build version and revision. |
| `/serverconfig` | Inspect compatible server configuration information. |
| `/autoLogin` | Attempt login using the configured `cg_autoLoginServer*` and `cg_autoLoginPass*` slots. |
| `/speedometer` | List or toggle speedometer bit-value options. |
| `/strafeHelper` | List or toggle strafe-helper bit-value options. |
| `/addCheckpoint` | Add a local checkpoint from two corner coordinates. |
| `/listCheckpoints` | List local checkpoints. |
| `/teleToCheckpoint <number>` | Request teleportation to a local checkpoint where supported. |
| `/clearTrail <client>` | Clear a client's displayed strafe trail. |
| `/loadTrail <file> [client]` | Load a saved strafe trail. |
| `/followRedFlag` | Follow the red-flag carrier while spectating. |
| `/followBlueFlag` | Follow the blue-flag carrier while spectating. |
| `/followFastest` | Follow the fastest racer while spectating. |

!!! warning "Auto-login passwords are local configuration values"
    Treat `cg_autoLoginPass1`, `cg_autoLoginPass2`, and `cg_autoLoginPass3` as sensitive. Do not paste configuration files containing them into a public issue.

## Server controls

Server operators can tune CTF, saber and Force behavior, weapons, movement, duels, administration, race/accounts, bots, Elo ranking, and compatibility behavior. Common entry points include:

| Control | Default | Purpose |
| --- | ---: | --- |
| `g_raceMode` | `2` | Disable, force, or let players toggle race mode. |
| `g_movementStyle` | `1` | Select the server movement style. |
| `g_allowGrapple` | `0` | Enable supported grappling styles. |
| `g_flipKick` | `0` | Configure flip-kick behavior. |
| `g_unlagged` | `0` | Enable selected unlagged systems as a bit value; changes are latched. |
| `g_tweakSaber` | `131072` | Configure saber behavior as a bit value with `tweakSaber`. |
| `g_tweakForce` | `0` | Configure Force behavior as a bit value with `tweakForce`. |
| `g_tweakWeapons` | `0` | Configure weapon behavior as a bit value with `tweakWeapons`. |
| `g_allowRegistration` | `0` | Configure account registration and clan permissions. |
| `g_eloRanking` | `0` | Enable built-in duel Elo ranking; changes are latched. |

Common server and administrative commands include `pause`, `resetScores`, `amlogin`, `ammap`, and `amtele`. Their availability depends on server configuration, permissions, and current game state.

!!! caution "Check ownership before changing a setting"
    A `cg_` control is normally client-side, `g_` controls usually belong to game code, and `sv_` controls belong to the server engine. Server cvars must be changed in the server console or through RCON, and some take effect only after a map restart.

## Complete inherited reference

The repository also carries a longer jaPRO-origin reference covering server cvars, RCON commands, game commands, emotes, client cvars, map entities, and bit-value options.

[Open the full jaPRO reference](japro-reference.md){ .md-button .md-button--primary }

That inherited document is useful background, but parts of it are stale. When a name, default, or description disagrees with the current client, treat the source code and the console output from your exact build as authoritative.

