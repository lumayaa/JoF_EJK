# Features

JoF EternalJK combines an EternalJK/OpenJK engine base, jaPRO-derived client and server features, community renderer work, and JoF-specific maintenance. Availability depends on which part of the project supplies a feature.

!!! tip "Client feature or server rule?"
    HUD, rendering, demo, and local presentation settings travel with the client. Movement rules, accounts, administration, and authoritative gameplay require a server running compatible game code.

## Player experience

- Customizable HUD elements, scoreboard options, chat presentation, player-name controls, and saber-style graphics.
- Movement keys, configurable speedometer and acceleration displays, race timing, strafe helpers, and strafe trails.
- Demo recording helpers, end-of-round screenshots, spectator camera options, and shortcuts for following racers or flag carriers.
- Ally/enemy model forcing, holstered sabers, cosmetics, configurable effects, and visual remaps.
- Client-side prediction and networking controls maintained for high-frame-rate multiplayer.
- Discord Rich Presence in builds compiled with the integration enabled.

## Renderers and platforms

The source tree builds the classic OpenGL renderer and the experimental Rend2 renderer. Windows release archives also package the external Vulkan renderer selected by the release workflow. Renderer availability therefore varies by platform and archive.

Release automation currently publishes:

- Windows x86 and x86_64
- Linux x86 and x86_64
- macOS x86_64 and arm64

## Server game systems

When the server runs the JoF game module, operators can configure:

- race mode, checkpoints, timing, movement styles, and grappling;
- dueling rules, Elo ranking, accounts, and administrative levels;
- CTF behavior, emotes, voting, team limits, and compatibility switches; and
- saber, Force, weapon, knockback, projectile, and unlagged behavior.

These systems are opt-in and server-authoritative. Connecting with JoF EternalJK does not change the rules of an unrelated server.

## Engine and maintenance work

The repository also carries the multiplayer engine, dedicated server, UI, and native game modules. Ongoing work includes platform builds, renderer integration, filesystem and loading behavior, NPC support, crash fixes, and compatibility with existing Jedi Academy content.

For exact changes in a build, use its generated GitHub release notes and `/modversion`; feature summaries intentionally avoid promising that every server enables every system.

[Read the release history](releases.md){ .md-button }
[Browse console controls](reference.md){ .md-button }

## Project lineage

JoF EternalJK is based on EternalJK and OpenJK and includes work derived from jaPRO and other community projects. Windows packaging also incorporates a Vulkan renderer maintained outside this repository. See the repository history, license notices, and [credits](development.md#credits) for attribution.

