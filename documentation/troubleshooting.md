# Troubleshooting

Start with the symptom you see. When asking for help, include your operating system, archive architecture, selected renderer, and the complete output of `/modversion`. Include a server address only when it is relevant and safe to share.

## Confirm the installed build

Open the console and run:

```text
/modversion
```

If that command is missing or reports an unexpected build, the wrong executable or client-game module is being loaded. Recheck the [expected installation layout](installation.md#expected-layout), remove obsolete JoF module archives after backing them up, and reinstall the current release.

## The client does not start

- Confirm that the archive architecture matches the operating system.
- Extract the complete archive. The executable alone is not a complete installation.
- Confirm that the original `base/assets*.pk3` game data is present.
- On Windows, check whether antivirus software quarantined an executable or DLL. Restore it only after verifying that the archive came from this project's release page.
- On Linux, set the executable bit with `chmod +x eternaljk.x86_64` and run `ldd ./eternaljk.x86_64` to look for missing libraries.
- On macOS, approve the app under **Privacy & Security** only after verifying the release source.

### Force the classic renderer

If launch failure began after changing renderers, pass the default renderer on the command line:

```text
+set cl_renderer rd-eternaljk
```

On Steam, place that text in the game's launch options. Once the client opens, `/cl_renderer rd-eternaljk` followed by `/vid_restart` makes the selection persistent.

## Missing menus, HUD art, or other assets

Re-extract the current release and allow its `base` and `EternalJK` content to merge with the game directory. Check the console for PK3 load errors.

Very old Windows installations may contain `GameData/EternalJK/japro-win-x86.pk3`. Back it up and remove it; current packages use architecture-specific `jofclient-win-*.pk3` files.

Do not remove the original `base/assets*.pk3` files supplied with Jedi Academy.

## The wrong executable starts from Steam

Steam normally launches `jamp.exe`. Back up the original file, then rename the JoF EternalJK executable for your architecture to `jamp.exe`, as described in the Windows installation guide. If Steam still launches another copy, verify the installation directory selected in Steam's **Installed Files** settings.

## A feature works on one server but not another

The server controls authoritative gameplay. HUD and local presentation options can work everywhere, while race, admin, movement, account, and gameplay-rule changes require compatible server game code and configuration. Use `/serverconfig` on supported servers to inspect advertised settings.

## Reset a bad local setting

Query the cvar in the console to see its current and default values, then set it back explicitly. Run `/vid_restart` for renderer settings or reconnect/restart as appropriate for latched settings.

If the client cannot reach the menu:

1. Close the game.
2. Locate the active `EternalJK` user-data directory. Portable Windows builds normally use `GameData/EternalJK`; non-portable builds use the path reported by `/fs_homepath`.
3. Rename `eternaljk.cfg` or `eternaljktc.cfg` to a backup name.
4. Start the client and let it generate a clean configuration.

!!! warning
    Resetting a configuration removes binds and preferences from the active setup. Keep the backup until you have recovered anything you need, and never publish it before checking it for saved passwords.

## A renderer works but another one does not

Renderer support depends on platform, driver, and release packaging. First confirm that the corresponding `rd-*` library exists beside the executable, then update the graphics driver and retry with the classic renderer. When reporting the issue, include the failing `cl_renderer` value and the first relevant console errors.

## Report a reproducible problem

Search [existing issues](https://github.com/JediofFreedom/JoF_EJK/issues) first. A useful report includes:

1. exact steps that trigger the problem;
2. expected and actual behavior;
3. complete `/modversion` output;
4. operating system and CPU architecture;
5. selected renderer and graphics hardware for rendering issues;
6. relevant console errors or a crash log;
7. whether the problem occurs with a clean configuration; and
8. whether it is limited to one server, map, or mod.

Remove passwords, private server addresses, authentication tokens, and personal paths before attaching logs or configuration files.

[Open a GitHub issue](https://github.com/JediofFreedom/JoF_EJK/issues/new){ .md-button .md-button--primary }

