# Server-only media checks

```text
cmake -S tests/target-media -B build/target-media-check
cmake --build build/target-media-check --config Release
ctest --test-dir build/target-media-check -C Release --output-on-failure
```

The harness compiles `g_media.c` directly with the real game headers and shared
parser. Engine imports, spawning, target dispatch and network delivery are mocked.
It checks the player-start/NPC-end encounter, repeated play, remembered recipients,
overlapping controllers, current map music restoration, selector filtering,
sound/voice/effect event fields, disconnect/slot reuse, death, spectator changes,
controller removal, map reset, invalid settings and command-safe asset paths.

Live verification still requires an unmodified client and matching assets:

1. With two connected players, start the example encounter as player A; only A
   should hear looping fight music. Following spectators must keep their own music.
2. Trigger start again while the track is playing; hear it restart from the beginning.
3. Have the combat NPC script call the end target at half health before despawning;
   A should return to map music, without requiring the NPC to die.
4. Repeat with simultaneous controllers, repeated stop, player death, spectating,
   reconnect/slot reuse, and map restart. An old end trigger must not stop newer music.
5. Test intro/loop tracks, dynamic map music, changing global map music during a
   fight, and a map with no music. Verify sound, voice and effect assets separately.

Automated tests verify server behavior and native protocol payloads, not actual
audio decoding, buffering, audibility, or effect rendering.
