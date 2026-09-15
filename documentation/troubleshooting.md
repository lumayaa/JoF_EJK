# Troubleshooting

Start with the symptom you see. When asking for help, include your operating system, archive architecture, renderer, server address if relevant, and the output of `/modversion`.

## The client does not start

- Confirm that the archive architecture matches the operating system.
- Extract the complete archive; do not copy only the executable.
- Make sure the original Jedi Academy game data is present.
- On Linux, set the executable bit with `chmod +x`.
- On macOS, approve the app under **Privacy & Security** only after verifying its release source.
- Try the classic renderer if a failure started after changing renderer settings.

## Missing menus, HUD art, or other assets

Re-extract the current release and allow its `base` and `EternalJK` content to merge with the game directory. Remove the obsolete `GameData/EternalJK/japro-win-x86.pk3` left by very old releases; current packages use `jofclient-win-x86.pk3`.

Do not remove the original `base/assets*.pk3` files supplied with Jedi Academy.

## The wrong executable starts from Steam

Steam normally launches `jamp.exe`. Back up the original file, then rename the JoF EJK executable for your architecture to `jamp.exe`, as described in the Windows installation guide.

## A feature works on one server but not another

The server controls authoritative gameplay. HUD and local presentation options can work everywhere, while race, admin, movement, account, and rule changes require compatible server game code and configuration.

## Reset a bad local setting

Set the cvar back to its documented default in the console and restart if it is latched. If the client cannot reach the menu, temporarily move your JoF EJK configuration file out of the user-data or portable game directory, then launch again to generate clean defaults.

!!! warning
    Moving a configuration resets binds and preferences. Keep the old file until you have recovered anything you need.

## Report a reproducible problem

Search [existing issues](https://github.com/JediofFreedom/JoF_EJK/issues) first. A useful report includes:

1. exact steps that trigger the problem;
2. expected and actual behavior;
3. `/modversion` output;
4. operating system and CPU architecture;
5. selected renderer;
6. console errors or a crash log; and
7. whether the problem also occurs with a clean configuration.

[Open a GitHub issue](https://github.com/JediofFreedom/JoF_EJK/issues/new){ .md-button .md-button--primary }

