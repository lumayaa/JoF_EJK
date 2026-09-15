# Build and contribute

JoF EJK uses CMake and builds the multiplayer engine, dedicated server, client game, server game, UI, classic renderer, and Rend2 renderer from one tree.

## Branch model

Open pull requests against `beta`. The automated release workflow builds `beta` pushes as prereleases; stable release work is merged to `master` by maintainers.

```bash
git clone https://github.com/JediofFreedom/JoF_EJK.git
cd JoF_EJK
git switch beta
```

## Configure and build

A standard local build starts with:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Install into a staging directory when you want the same layout used for packaging:

```bash
cmake --install build --prefix "$PWD/install"
```

Platform dependencies and generator details differ. The repository's [build workflow](https://github.com/JediofFreedom/JoF_EJK/blob/beta/.github/workflows/build.yml) is the executable reference for supported CI builds.

## Useful CMake options

| Option | Default | Effect |
| --- | --- | --- |
| `BuildPortableVersion` | `ON` | Keep user data beside the game rather than in the platform home directory. |
| `BuildMPEngine` | `ON` | Build the multiplayer engine. |
| `BuildMPDed` | `ON` | Build the dedicated server. |
| `BuildMPGame` | `ON` | Build server-side game code. |
| `BuildMPCGame` | `ON` | Build client-side game code. |
| `BuildMPUI` | `ON` | Build the UI module. |
| `BuildMPRdVanilla` | `ON` | Build the classic renderer. |
| `BuildMPRend2` | `ON` | Build the experimental Rend2 renderer. |
| `BuildDiscordRichPresence` | `ON` | Include Discord Rich Presence support. |
| `BuildTests` | `OFF` | Build the Boost-based unit tests. |

Pass an option with `-D`, for example `-DBuildTests=ON`.

## Documentation

The docs use Material for MkDocs. Preview changes locally with:

```bash
python -m pip install -r requirements-docs.txt
mkdocs serve
```

A strict documentation build runs for relevant pull requests. The public Pages site deploys when documentation changes reach `master`.

## Pull-request checklist

1. Branch from the current `beta` head.
2. Keep a change focused and explain visible player/server behavior.
3. Build the affected targets and exercise the change in game when possible.
4. Update documentation for new commands, cvars, configuration, or build behavior.
5. Open the pull request against `beta`.

## Credits

JoF maintainers and contributors include Milamber, Daniel, Jediman, lumayaa, Sol-Vulpes, and Clix (looZ149), with additional contributions recorded in Git history and releases. The project also builds on work from OpenJK, EternalJK, jaPRO, and community renderer maintainers including SomaZ, Sunny, and Tayst.

