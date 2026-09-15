# Releases

Stable and beta builds are published through GitHub Releases for Windows, Linux, and macOS. Each release records the source tag and provides one archive per supported architecture.

[Download the latest stable release](https://github.com/JediofFreedom/JoF_EJK/releases/latest){ .md-button .md-button--primary }
[Browse every release](https://github.com/JediofFreedom/JoF_EJK/releases){ .md-button }

## Release channels

| Channel | Branch | Intended use |
| --- | --- | --- |
| Stable | `master` | Recommended for normal play. |
| Beta | `beta` | Prerelease testing for changes headed toward stable. |

Beta builds may change more frequently and can introduce regressions. Include the complete `/modversion` output in beta feedback.

## Published archives

| Archive | Contents |
| --- | --- |
| `JoF-windows-x86.zip` | 32-bit Windows client, modules, renderers, and assets. |
| `JoF-windows-x86_64.zip` | 64-bit Windows client, modules, renderers, and assets. |
| `JoF-linux-x86.tar.gz` | 32-bit Linux client, modules, renderers, and assets. |
| `JoF-linux-x86_64.tar.gz` | 64-bit Linux client, modules, renderers, and assets. |
| `JoF-macos-x86_64.tar.gz` | Intel macOS app bundle, modules, renderers, and assets. |
| `JoF-macos-arm64.tar.gz` | Apple Silicon macOS app bundle, modules, renderers, and assets. |

The Windows archives additionally contain the externally maintained Vulkan renderer selected by the release workflow. Linux and macOS archives do not currently package that renderer.

## How releases are produced

- A source-code push to `beta` builds every supported platform and publishes a numbered prerelease.
- Stable builds run from `master` through the scheduled or manually dispatched release workflow when relevant source changes exist.
- Documentation-only changes build and deploy separately; they do not create a game release.
- GitHub-generated notes provide the commit and pull-request history for each new release.

!!! note
    Passing CI means the project compiled and packaged successfully on the release runners. It does not guarantee that every renderer, server configuration, or third-party map was exercised in game.

## Release history in the repository

Older package notes live under [`ReleaseReadmes/`](https://github.com/JediofFreedom/JoF_EJK/tree/master/ReleaseReadmes). Current GitHub release pages are authoritative for downloadable assets and generated change lists.

## Integrity and provenance

Download archives from this repository's Releases page. Avoid executables redistributed without a matching tag and `/modversion` value. The repository workflow builds releases from a recorded commit, and each release page links its source tag.

