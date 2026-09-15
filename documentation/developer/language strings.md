# Language string files

The stringed system loads key-based translations from `.str` files. Parsing and lookup are implemented in `codemp/qcommon/stringed_ingame.cpp` and `codemp/qcommon/stringed_interface.cpp`.

## File structure

A file begins with a version. Legacy editor metadata is accepted but is not required at runtime:

```text
VERSION   "1"
CONFIG    "stringed.cfg"
FILENOTES "In-game text for item pickups"
```

Each entry begins with `REFERENCE`, followed by one or more language values:

```text
REFERENCE    PICKUPLINE
NOTES        "Printed before the item name when it is obtained"
LANG_ENGLISH "Obtained:"
LANG_GERMAN  "Aufgenommen:"
```

Use `#same` when a translation intentionally matches the English value:

```text
LANG_FRENCH "#same"
```

An entry may also contain flags:

```text
FLAGS FLAG_CAPTION FLAG_TYPEMATIC
```

Terminate the file with:

```text
ENDMARKER
```

## Authoring guidance

- Keep `REFERENCE` identifiers stable; code and assets use them as keys.
- Quote user-facing values, especially when they contain spaces or punctuation.
- Preserve escapes expected by the parser rather than inserting literal control characters.
- Supply an English value for every new entry, then add translations without changing the key.
- Keep files in the directory structure expected by the game assets; the loader can discover string files recursively.

Test new strings in the UI or game path that consumes them. A parser-successful file can still be wrong if its key is never referenced or its asset path is incorrect.
