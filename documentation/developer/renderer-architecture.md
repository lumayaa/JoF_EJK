# Renderer architecture

The client loads its renderer as a native library. `cl_renderer` selects the library stem and is latched, so a renderer change takes effect after a video restart or client restart.

| Renderer | CMake option | Output stem | Status |
| --- | --- | --- | --- |
| Classic | `BuildMPRdVanilla` | `rd-eternaljk_<arch>` | Default |
| Rend2 | `BuildMPRend2` | `rd-rend2-etjk_<arch>` | Experimental |

If a requested renderer cannot be loaded, the client resets to its default renderer. To force that recovery from the command line, add `+set cl_renderer rd-eternaljk`.

## Front end

The front end accepts rendering requests from the client game and UI. It collects the view definition, world surfaces, models, lights, and effects for a frame, then sorts and records work for execution. This stage defines *what* should be drawn and avoids tying game modules to graphics API calls.

## Back end

The back end consumes the recorded render commands and performs the graphics work: state changes, buffer and texture use, shader execution, and draw calls. Keeping submission separate from execution limits graphics-specific concerns to the renderer implementation.

## Source map

- `codemp/rd-vanilla/` contains the classic renderer.
- `codemp/rd-rend2/` contains the experimental Rend2 renderer.
- `codemp/client/cl_main.cpp` registers `cl_renderer` and loads the selected native library.
- Shared renderer contracts are defined in the engine-facing renderer headers.

Renderer work should preserve the public import/export boundary. A change that alters an interface structure must update both its producer and consumer, and should be tested with both renderer targets enabled.

!!! note "Vulkan runtime file"
    Windows release packaging may include a third-party Vulkan renderer DLL downloaded by CI. That packaged file is separate from the two renderer targets built from this repository.
