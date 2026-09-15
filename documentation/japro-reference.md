# Full jaPRO reference

The repository retains an inherited [`japro_docs.md`](https://github.com/JediofFreedom/JoF_EJK/blob/master/japro_docs.md) reference. It covers:

- server cvars grouped by gameplay system;
- RCON, game, admin, and emote commands;
- client HUD, movement, sound, visual, network, and demo cvars;
- strafe-helper and plugin bit values; and
- supported map-entity additions.

[Browse the inherited reference](https://github.com/JediofFreedom/JoF_EJK/blob/master/japro_docs.md){ .md-button .md-button--primary }

For the most commonly used controls with concise descriptions, use the [searchable quick reference](reference.md).

!!! warning "Historical reference"
    This document was inherited from an earlier jaPRO codebase and is not a generated inventory. Some names, defaults, descriptions, and features no longer match JoF EternalJK. For example, current defaults are defined by `codemp/cgame/cg_xcvar.h` and `codemp/game/g_xcvar.h`, not by this Markdown file.

Use it to discover a system, then verify the result in the current build:

1. Enter a cvar name without a value to print its current and default values.
2. Use a configuration command such as `/speedometer` or `/strafeHelper` to list supported bit options.
3. Check `/serverconfig` for settings advertised by a compatible server.
4. Treat the current source and `/modversion` as authoritative when behavior differs.

