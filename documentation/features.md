# Features

JoF EJK combines an EternalJK/OpenJK engine base, jaPRO-derived client and game features, community renderer work, and JoF-specific polish.

## Player experience

- Customizable HUD elements, saber-style graphics, scoreboard improvements, and player/NPC health display options.
- Movement keys, speedometer, accelerometer, jump measurements, race timer, and strafe-helper displays.
- Demo recording helpers, automatic screenshots, spectator camera damping, and follow shortcuts.
- Bright skins, ally/enemy model forcing, holstered sabers, custom Force effects, and configurable visual remaps.
- Predicted weapon effects, high-FPS command-rate controls, improved sound buffering, and crash fixes.
- Discord Rich Presence support in supported builds.

## Renderers and platforms

The project builds the classic renderer and the experimental Rend2 renderer. Windows release packaging also includes the community Vulkan renderer used by the project. Renderer behavior and availability vary by platform and build.

Release automation currently publishes:

- Windows x86 and x86_64
- Linux x86 and x86_64
- macOS x86_64 and arm64

## Multiplayer and movement

JoF EJK carries client and server game modules. Depending on the server configuration, available systems include race mode, movement styles, grappling, dueling controls, admin commands, accounts, Elo ranking, emotes, CTF refinements, weapon/Force tuning, and compatibility switches.

!!! note "The server remains authoritative"
    Client-side display and prediction features travel with you. Server-side rules only apply when the connected server runs compatible game code and enables them.

## Recent JoF work

The 1.6 series included HUD graphics for additional saber styles, larger NPC and client limits, renderer menu improvements, Linux and macOS release builds, radial-menu work, hidden-skin controls, filesystem threading for the UI, weapon-animation prediction, Force Sense NPC health bars, and multiple crash fixes.

[Read the release history](releases.md){ .md-button }
[Browse console controls](reference.md){ .md-button }

## Project lineage

JoF EJK is based on EternalJK and OpenJK and includes work derived from jaPRO and other community projects. Renderer packaging also incorporates work maintained outside this repository. See the repository history, license notices, and [credits](development.md#credits) for attribution.

