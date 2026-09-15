# Install JoF EternalJK

JoF EJK is distributed as a ready-to-run archive for each supported platform. You need an existing installation of *Star Wars Jedi Knight: Jedi Academy*.

[Download the latest release](https://github.com/JediofFreedom/JoF_EJK/releases/latest){ .md-button .md-button--primary }

## Choose the right archive

| Platform | 32-bit Intel | 64-bit Intel/AMD | Apple Silicon |
| --- | --- | --- | --- |
| Windows | `JoF-windows-x86.zip` | `JoF-windows-x86_64.zip` | — |
| Linux | `JoF-linux-x86.tar.gz` | `JoF-linux-x86_64.tar.gz` | — |
| macOS | — | `JoF-macos-x86_64.tar.gz` | `JoF-macos-arm64.tar.gz` |

Most Windows and Linux players should choose the 64-bit archive. On macOS, choose `arm64` for Apple Silicon (M1 or newer) and `x86_64` for an Intel Mac.

## Windows

1. Find the Jedi Academy `GameData` directory. A default Steam install is usually under `steamapps/common/Jedi Academy/GameData/`.
2. Extract the archive so its `GameData` contents merge with your existing `GameData` directory.
3. Allow replacement of older JoF EJK files when updating.
4. Start `eternaljk.x86_64.exe` for the 64-bit build or `eternaljk.x86.exe` for the 32-bit build.

Renaming the chosen executable to `jamp.exe` can improve Steam launch integration. Keep a backup of the original launcher first.

## Linux

1. Extract the archive into your Jedi Academy installation.
2. Make sure the launcher is executable: `chmod +x eternaljk.x86_64` (or `eternaljk.i386` for a 32-bit build).
3. Run it from the game directory.

The Linux archive is packaged from the `JediAcademy` install tree and includes the JoF client assets used by the release build.

## macOS

1. Extract the archive into your Jedi Academy installation.
2. Open the included `eternaljk.<architecture>.app` bundle.
3. If macOS blocks the first launch, use **System Settings → Privacy & Security → Open Anyway** after confirming that the archive came from this repository's release page.

## Updating an older installation

Extract the new release over the old one and replace conflicting JoF EJK files. Very old installations may still contain `GameData/EternalJK/japro-win-x86.pk3`; remove that obsolete file because it was replaced by `jofclient-win-x86.pk3`.

!!! warning "Keep the base game data"
    Do not delete Jedi Academy's original `base/assets*.pk3` files. JoF EJK supplies a client and supporting assets, not the licensed base game data.

## Verify the client

Open the in-game console and run:

```text
/modversion
```

The printed branch, version, and commit help maintainers identify the exact build when you report a problem.

