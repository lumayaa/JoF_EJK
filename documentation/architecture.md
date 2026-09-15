# Architecture overview

Jedi Academy multiplayer separates the engine from loadable game modules. That boundary preserves compatibility with existing mods while allowing the client, server rules, UI, and renderer to evolve independently.

## Main build outputs

| Component | Typical output | Responsibility |
| --- | --- | --- |
| Multiplayer engine | `eternaljk.<architecture>` | Platform, filesystem, networking, console, module loading, sound, and renderer orchestration. |
| Dedicated server | `eternaljkded.<architecture>` | Headless multiplayer server process. |
| Client game | `cgame<architecture>` | HUD, prediction, player presentation, movement tools, and client-side game behavior. |
| Server game | `jampgame<architecture>` | Authoritative gameplay rules and server commands. |
| UI | `ui<architecture>` | Menus and interface logic. |
| Classic renderer | `rd-eternaljk_<architecture>` | Traditional rendering backend. |
| Rend2 | `rd-rend2-etjk_<architecture>` | Experimental modern rendering path. |

## Read next

- [Library loading](developer/libraries.md) explains native module discovery and the engine/module API boundary.
- [Renderer architecture](developer/renderer-architecture.md) outlines the renderer front end and back end.
- [Language strings](developer/language%20strings.md) documents the string-file format.
- [Sound code notes](developer/sound-code-declutter.md) records deeper sound-system maintenance work.

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

