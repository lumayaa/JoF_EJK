# Install JoF EternalJK

JoF EternalJK is distributed as a ready-to-run archive for each supported platform. You need an existing installation of *Star Wars Jedi Knight: Jedi Academy*; the release does not contain the original game data.

[Download the latest release](https://github.com/JediofFreedom/JoF_EJK/releases/latest){ .md-button .md-button--primary }
[View stable and beta releases](https://github.com/JediofFreedom/JoF_EJK/releases){ .md-button }

## Before you begin

1. Close Jedi Academy and any dedicated server using the same files.
2. Locate the game's installation directory.
3. Back up customized files, especially launchers and anything under `GameData/EternalJK/`.

!!! warning "Keep the original game data"
    Do not delete or replace Jedi Academy's original `GameData/base/assets*.pk3` files. JoF EternalJK supplies executables, game modules, and supporting assets; it does not replace the licensed base game.

## Choose the right archive

| Platform | 32-bit Intel | 64-bit Intel/AMD | Apple Silicon |
| --- | --- | --- | --- |
| Windows | `JoF-windows-x86.zip` | `JoF-windows-x86_64.zip` | — |
| Linux | `JoF-linux-x86.tar.gz` | `JoF-linux-x86_64.tar.gz` | — |
| macOS | — | `JoF-macos-x86_64.tar.gz` | `JoF-macos-arm64.tar.gz` |

Most Windows and Linux players should choose the 64-bit archive. On macOS, choose `arm64` for Apple Silicon (M1 or newer) and `x86_64` for an Intel Mac.

## Windows

1. Open the downloaded ZIP. Inside its top-level release folder, locate `GameData`.
2. Copy that `GameData` folder into the Jedi Academy installation directory so it merges with the existing `GameData` folder.
3. Allow replacement of older JoF EternalJK files when updating.
4. Start `GameData/eternaljk.x86_64.exe` for the 64-bit build or `GameData/eternaljk.x86.exe` for the 32-bit build.

A default Steam installation is commonly found at:

```text
C:\Program Files (x86)\Steam\steamapps\common\Jedi Academy\GameData\
```

Steam normally starts `jamp.exe`. To make Steam launch JoF EternalJK, back up the original `jamp.exe`, then rename the JoF executable for your architecture to `jamp.exe`.

## Linux

1. Extract the archive into the directory that contains your Jedi Academy data, preserving the packaged `base/` and `EternalJK/` directories.
2. Make the launcher executable: `chmod +x eternaljk.x86_64` for 64-bit, or `chmod +x eternaljk.i386` for 32-bit.
3. Run the executable from that directory.

If the executable reports a missing shared library, use `ldd ./eternaljk.x86_64` to identify it and install the corresponding package from your distribution. SDL 2, OpenGL, libjpeg, libpng, and zlib are typical runtime dependencies.

## macOS

1. Extract the archive into your Jedi Academy directory, preserving the packaged `base/` and `EternalJK/` directories.
2. Open `eternaljk.arm64.app` on Apple Silicon or `eternaljk.x86_64.app` on an Intel Mac.
3. If macOS blocks the first launch, use **System Settings → Privacy & Security → Open Anyway** after confirming that the archive came from this repository's release page.

## Updating an older installation

Install the new release over the existing one and replace conflicting JoF EternalJK files. Do not replace unrelated custom maps, models, or configuration files.

Very old Windows installations may still contain `GameData/EternalJK/japro-win-x86.pk3`. Remove that obsolete module archive after making a backup; current releases use architecture-specific `jofclient-win-*.pk3` files.

## Expected layout

After installation, the relevant files should resemble the following. The outer directory is `GameData` on Windows; Linux and macOS archives place this content at their archive root.

```text
<game data directory>/
├── base/
│   ├── assets0.pk3          # original game data
│   └── JoF_*.pk3            # JoF supporting assets
├── EternalJK/
│   ├── japro-assets.pk3
│   ├── jofclient-assets.pk3
│   └── jofclient-win-*.pk3  # Windows game modules
└── eternaljk.*              # executable or macOS app bundle
```

The exact native library names vary by operating system and architecture.

## Verify the client

Open the in-game console and run:

```text
/modversion
```

The printed branch, version, and commit identify the exact build. Keep that output with any bug report.

If the client does not launch or assets are missing, continue with the [troubleshooting guide](troubleshooting.md).

