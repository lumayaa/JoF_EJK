# Build and contribute

JoF EternalJK uses CMake to build the multiplayer engine, dedicated server, client game, server game, UI, classic renderer, and Rend2 renderer from one source tree.

## Branch model

Open pull requests against `beta`. A source-code push to `beta` produces a prerelease build; maintainers promote tested work to `master` for stable releases.

```bash
git clone https://github.com/JediofFreedom/JoF_EJK.git
cd JoF_EJK
git switch beta
```

Create a topic branch rather than committing directly to `beta`:

```bash
git switch -c feature/short-description
```

## Prerequisites

All platforms need Git, CMake 3.10 or newer, and a C/C++ toolchain supported by CMake.

- **Windows:** Visual Studio 2022 with the Desktop development with C++ workload is the closest match to CI. Bundled libraries are enabled by default.
- **Linux:** install a compiler plus development packages for SDL 2, OpenGL, libjpeg, libpng, and zlib. The CI workflow contains the current Ubuntu package list, including its 32-bit multilib setup.
- **macOS:** install Xcode command-line tools and CMake. Release CI builds a native SDL 2 library before configuring the project.

## Configure and build

A standard out-of-tree release build is:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
```

For Visual Studio, select the architecture explicitly when configuring:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release --parallel
```

Use `-A Win32` for a 32-bit Windows build.

Install into a staging directory when you want the directory layout used for packaging:

```bash
cmake --install build --config Release --prefix install
```

The repository's [build workflow](https://github.com/JediofFreedom/JoF_EJK/blob/beta/.github/workflows/build.yml) is the executable reference for release compilers, dependencies, architecture flags, and packaging.

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
| `UseInternalOpenAL` | platform-dependent | Use the bundled OpenAL library. |
| `UseInternalSDL2` | platform-dependent | Use the bundled SDL 2 library. |
| `UseInternalJPEG` | platform-dependent | Use the bundled JPEG library. |
| `UseInternalPNG` | platform-dependent | Use the bundled PNG library. |
| `UseInternalZlib` | platform-dependent | Use the bundled zlib library. |

Pass options during configuration, for example `cmake -S . -B build -DBuildTests=ON -DBuildMPDed=OFF`.

## Run tests

Boost is required when `BuildTests` is enabled.

```bash
cmake -S . -B build-tests -DCMAKE_BUILD_TYPE=Release -DBuildTests=ON
cmake --build build-tests --config Release --parallel
ctest --test-dir build-tests -C Release --output-on-failure
```

The suite is small, so a successful build is not a substitute for exercising player-visible changes in game.

## Documentation

The site uses Material for MkDocs. Work in a virtual environment so documentation packages do not modify the system Python installation.

=== "Windows"

    ```powershell
    py -m venv .venv
    .venv\Scripts\python -m pip install -r requirements-docs.txt
    .venv\Scripts\mkdocs serve
    ```

=== "Linux and macOS"

    ```bash
    python3 -m venv .venv
    .venv/bin/python -m pip install -r requirements-docs.txt
    .venv/bin/mkdocs serve
    ```

Open the local address printed by MkDocs. Before committing, run `.venv/bin/mkdocs build --strict` (or `.venv\Scripts\mkdocs build --strict` on Windows). CI performs the same strict build for relevant pull requests; only `master` can deploy the public Pages site.

## Pull-request checklist

1. Branch from the current `beta` head and keep the change focused.
2. Match the surrounding code style; avoid unrelated formatting churn.
3. Build every affected target and run relevant automated tests.
4. Exercise player- or server-visible behavior in game when possible.
5. Update documentation for commands, cvars, configuration, packaging, or build behavior.
6. Explain the behavior change, verification performed, and any remaining platform limitations in the pull request.
7. Open the pull request against `beta`.

Do not commit generated build directories, the MkDocs `site/` output, downloaded game data, credentials, or local configuration.

## Credits

JoF maintainers and contributors include Milamber, Daniel, Jediman, lumayaa, Sol-Vulpes, and Clix (looZ149), with additional contributions recorded in Git history and releases. The project builds on work from OpenJK, EternalJK, jaPRO, and community renderer maintainers including SomaZ, Sunny, and Tayst.

