# Architecture overview

Jedi Academy multiplayer separates the host engine from loadable game modules and renderers. That boundary preserves compatibility with existing mods while allowing client presentation, authoritative server rules, menus, and rendering to evolve independently.

```text
eternaljk executable
├── cgame module ───── local prediction, HUD, client presentation
├── ui module ──────── menus and front-end UI
├── renderer ───────── classic OpenGL or Rend2
└── network connection
    └── game module ── authoritative rules inside the server process
```

The dedicated-server executable hosts the game module without the interactive client, sound, or graphical renderer.

## Main build outputs

| Component | CMake target/output stem | Responsibility |
| --- | --- | --- |
| Multiplayer engine | `eternaljk.<architecture>` | Platform, filesystem, networking, console, module loading, sound, and renderer orchestration. |
| Dedicated server | `eternaljkded.<architecture>` | Headless multiplayer server process. |
| Client game | `cgame<architecture>` | HUD, prediction, snapshots, player presentation, movement tools, and client-side effects. |
| Server game | `jampgame<architecture>` | Authoritative gameplay, NPCs, accounts, administration, and server commands. |
| UI | `ui<architecture>` | Menus, settings, server browser, and front-end interface logic. |
| Classic renderer | `rd-eternaljk_<architecture>` | Traditional rendering backend. |
| Rend2 | `rd-rend2-etjk_<architecture>` | Experimental modern rendering path. |

Platform extensions differ: Windows uses `.exe` and `.dll`, Linux uses executables and `.so` libraries, and macOS release builds package the client in an `.app` bundle with native libraries.

## Runtime ownership

Understanding ownership is important when debugging a feature:

| Symptom | Start looking in |
| --- | --- |
| Window, input, filesystem, networking, download, or module loading | `codemp/client/`, `codemp/qcommon/`, `codemp/server/`, `shared/` |
| HUD, prediction, local effects, spectator view, or client console command | `codemp/cgame/` |
| Damage, movement rules, NPC behavior, accounts, or admin command | `codemp/game/` |
| Menus or settings screens | `codemp/ui/` and `assets/*/ui/` |
| OpenGL rendering | `codemp/rd-vanilla/` or `codemp/rd-rend2/` |

Shared movement and saber code lives under `codemp/game/` but is compiled into both client and server modules where prediction must match authoritative simulation.

## Filesystem and packaged assets

The engine reads original game data from `base/`, JoF client data from `EternalJK/`, and an active mod directory selected through `fs_game`. Native module lookup and packaged PK3 names vary by platform. Windows installs the game modules in an architecture-specific `jofclient-win-*.pk3`; assets are built separately as `japro-assets.pk3` and `jofclient-assets.pk3`.

Release automation adds large JoF asset packages that are intentionally not stored directly in this Git repository. Do not assume a local source checkout contains every file found in a published release.

## Read next

- [Library loading](developer/libraries.md) explains native module discovery and the engine/module API boundary.
- [Renderer architecture](developer/renderer-architecture.md) outlines the renderer front end and back end.
- [Language strings](developer/language%20strings.md) documents the string-file format.

## Source layout

| Path | Area |
| --- | --- |
| `codemp/client/` | Client engine and input-facing systems. |
| `codemp/server/` | Server engine. |
| `codemp/cgame/` | Client game module. |
| `codemp/game/` | Server game module. |
| `codemp/ui/` | UI module. |
| `codemp/rd-vanilla/` | Classic renderer. |
| `codemp/rd-rend2/` | Rend2 renderer. |
| `shared/` | Code shared across targets. |
| `assets/` | Packaged client and jaPRO-derived assets. |
| `lib/` | Bundled third-party libraries. |

The top-level `CMakeLists.txt` defines build options and output names; component-specific source lists and linking live in `codemp/*/CMakeLists.txt`.

