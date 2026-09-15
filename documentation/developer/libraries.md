# Native module loading

The multiplayer engine and game logic are separate native binaries. This boundary lets the engine load a client game, server game, and UI implementation without compiling them into the executable.

## Runtime pieces

| Component | Responsibility | Typical output stem |
| --- | --- | --- |
| Engine client | Platform, networking, filesystem, renderer loading | `eternaljk` |
| Dedicated server | Headless server runtime | `eternaljkded` |
| Client game | Snapshots, prediction, HUD, client-side effects | `cgame` |
| Server game | Rules, entities, commands, authoritative state | `jampgame` |
| UI | Menus and user-interface scripting | `ui` |

The exact suffix depends on the platform and architecture. Windows releases place the game modules in an architecture-specific `jofclient-win-*.pk3`; other platforms ship native shared libraries alongside the installed game files.

## Discovery

The engine searches its configured filesystem paths for a module matching the active game and architecture. `fs_game` selects a mod directory; the JoF client otherwise uses `EternalJK` as its base game directory. Install paths and per-user paths can both participate, depending on whether the build is portable.

When diagnosing a load failure, check:

1. `/modversion` reports the executable you intended to launch.
2. `/fs_game` names the intended mod.
3. The module architecture matches the executable architecture.
4. The release archive was extracted without changing its directory structure.
5. An older module archive is not shadowing the current one.

## Engine/module interfaces

The legacy Quake 3 interface uses two exported entry points:

- `vmMain(int command, ...)` dispatches calls from the engine into a module.
- `dllEntry(intptr_t (*syscallptr)(intptr_t arg, ...))` gives the module a callback for engine services.

JoF EternalJK also supports the OpenJK native API pattern:

```c
<module>Export_t *GetModuleAPI(int apiVersion, <module>Import_t *imports);
```

This exchanges typed import and export tables, reducing the indirect command dispatch needed by the legacy interface. The API version remains part of the contract: changing shared structures or function tables requires coordinated changes on both sides.

Relevant implementations live in `codemp/client/cl_cgameapi.cpp`, `codemp/client/cl_uiapi.cpp`, and `codemp/server/sv_gameapi.cpp`; module entry points live under `codemp/cgame`, `codemp/ui`, and `codemp/game`.
