# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Run Commands

The root `makefile` only orchestrates the three platform makefiles; running `make` with no target just prints usage. Pick a platform makefile for real work:

```sh
# macOS ARM (M-series) — primary dev target
make -f makefile.mac run      # clean + build + run
make -f makefile.mac asan     # clean + build with ASan/UBSan + run (no LeakSanitizer on macOS)
make -f makefile.mac lint     # static analysis only (cppcheck + clang-tidy), no compile

# Linux x86_64 (Ubuntu / WSL2) — same targets, LeakSanitizer included in `asan`
make -f makefile.linuxAMDx64 run | asan | lint

# RG35XXH handheld (aarch64 / muOS) — cross-compiled inside Docker
make -f makefile.rg35xxh docker-build   # builds the muos-builder-aarch64 image on first run

# Aggregates (root makefile)
make buildOnMacOs        # docker-build (RG35XXH) + macOS build
make checkOnMacOs        # asan + lint on macOS
make buildOnLinuxAMDx64  # docker-build + Linux build
make checkOnLinuxAMDx64  # asan + lint on Linux
make clean               # cleans all three platforms' dist/ trees
```

`lint` and `asan` are the quality gates — there is no unit-test framework. The CI/self-test path is `SELFTEST=<n>` (env var read in `main.c`): when set, the program runs a fixed number of frames through the full init→loop→teardown path then exits cleanly, instead of looping until quit. Pair it with `SDL_VIDEODRIVER=dummy` for a headless ASan/UBSan run in CI (see `.github/workflows/ci.yml`). `lint` requires `cppcheck` and `clang-tidy` (macOS: clang-tidy is expected at `/opt/homebrew/opt/llvm/bin/clang-tidy`; override with `CLANG_TIDY=...`).

## The per-frame triad (core architecture)

This is a single-source, multi-target C/SDL2 game. `main.c` is a thin shell: it calls `init_all_system()` to get a `PixContext *`, then loops three calls in a fixed order until `ctx->quit`:

```
global_main_event_handler(ctx)   → drain SDL events
global_main_action_per_tick(ctx) → fixed-step game logic (gated to 32 ms)
global_main_draw(ctx)            → render, single pix_draw() present per frame
```

**This `event_handler / action_per_tick / draw` triad is the universal unit of behaviour at every tier of the program.** It is codified in `PhaseVTable` (`include/game/core/phase_vtable.h`):

```c
typedef struct {
    void (*event_handler)(const SDL_Event *e);
    void (*action_per_tick)(PixContext *ctx);
    void (*draw)(PixContext *ctx);
} PhaseVTable;
```

Dispatch is table-driven, never `switch`-on-state:

- **App tier** — `src/game/core/state_table.c` holds `STATES[]`, one `PhaseVTable` row per `AppState` (`STATE_MENU`, `STATE_GAME`, `STATE_PAUSE`, `STATE_MAPPING`). The three `global_main_*` functions all look up the row via `app_state_vtable(app_state)` and call the matching phase. `STATE_QUIT` is terminal: it has **no row**, `app_state_vtable` returns `NULL` for it, and any sub-handler requests exit by setting `app_state = STATE_QUIT` (the event handler then flips `ctx->quit`).
- **Level tier** — `src/game/states/game/state_game.c` repeats the exact pattern one level down: a `LEVELS[]` table of `PhaseVTable` rows indexed by `LevelId`, dispatched through `current_level`.

**Consequence for adding behaviour:** a new app state is one enum value in `globalAppState.h` + one row in `state_table.c` (plus the state's own module). A new level is one `LevelId` value + one row in `state_game.c`. You cannot register a unit without supplying all three phases — an omitted phase shows up as a visible `NULL` in the table. Do not reintroduce per-state `switch` statements in the global loop.

### State / level module layout

Each app state is a module under `src/game/states/<name>/` exposing `<name>_event_handler`, `<name>_action_per_tick`, `<name>_draw` (declared in `include/game/states/state_<name>.h`). Levels live under `src/game/states/game/levels/` with the same triad named `level_NN_*`. Private transition helpers are `static` and named `on<Action>` (e.g. `onBackToMenu`, `onPauseGame`). State transitions must call `global_input_state_resetInputState()` before flipping `app_state`, to stop stale directional input leaking across the transition.

## Input: two layers, both DRY

1. **Resolved direction** — `global_input_state` (`InputState` = `up/down/left/right` bools) is synthesised from *all* sources (keyboard arrows + D-pad hat + both analog sticks) by `global_input_state_update_pressed_keys()`, which `global_main_event_handler` pumps **once per event** for every state. States read the resolved booleans; they never re-derive direction.
2. **Logical button mapping** — `input_mapping.*` maps physical inputs (button / hat / axis / keycode) to logical `MappingLabel`s (`MAPPING_LABEL_A`, `..._START`, `..._STICK_G_HAUT`, …). Game code asks `input_mapping_is_press(e, MAPPING_LABEL_X)` rather than checking raw SDL codes, so user remapping is respected everywhere. Bindings live in `global_console_mapping[]`, are persisted to `console_mapping.log`, and are loaded at boot by `input_mapping_load()` (factory defaults first, saved file overlaid on top, so there is never an unbound label). The `STATE_MAPPING` screen is the remap UI.

Per RULES.md DRY: keyboard and joystick branches for the same action must call the **same** `static on<Action>` helper — see `game_event_handler` for the canonical example (SPACE and mapped A both reach the same path; BACKSPACE and Y both call `onBackToMenu`). Jump goes through `player_jump_pressed()` in the shared `Player` entity (`src/game/entities/player/`) so all levels share one jump/gravity implementation.

## libpixtools — the rendering/utility library

Everything under `src/libpixtools/` compiles into `libpixtools.a` (auto-linked via `-lpixtools`; see makefile notes below). It owns the software framebuffer and all drawing. `PixContext` (`include/libpixtools/pixtools.h`) wraps the SDL window/renderer/texture/joystick plus a `uint32_t *pixels` buffer; `pix_init` creates a **640×480** virtual screen (`SCREEN_WIDTH`/`SCREEN_HEIGHT` — do not change), drawing primitives (`pix_add_*`) write into the buffer, and `pix_draw` blits once per frame. Sprites are 8-bit grayscale arrays where **0 = transparent** (`pix_add_sprite8*`); global sprites are centralized in `src/variables/globalSprites.c` (e.g. the 16×16 `spritePlayer_256`, runtime-filled `spriteBackground01_19200`). Submodules: `pixletters/` (8×8 bitmap font), `world_scrolling_system/`, `global_input_state/`.

## Global state (`src/variables/`)

`extern` globals are *declared* in `include/variables/*.h` and *defined once* in `src/variables/*.c` (RULES.md §3): `app_state` (globalAppState), `global_input_state` (globalInputState), `global_console_mapping[]` (globalConsoleMapping), and the sprite arrays (globalSprites). `init_all_system()` is the single place that initialises all of them in order at boot.

## Build system invariants (all three makefiles)

**The three platform makefiles are near-identical by design.** When changing build behavior (warning flags, source discovery, lib logic), apply the change to all relevant makefiles, not just one (the comments cite RULES.md §2 "adopt the stricter rule everywhere" — divergence is a smell).

- **Sources are auto-discovered** via `find $(SRC_DIR) -name '*.c'`. Adding a `.c` file needs no makefile edit. Object/dep files mirror the `src/` layout under `dist/<platform>/compile/`.
- **`libpixtools` is conditionally linked.** Any `.c` under `src/libpixtools/` goes into `libpixtools.a` and is linked via `-lpixtools`; everything else is application code. When `src/libpixtools/` has no `.c` files, both the archive step and `-lpixtools` are skipped automatically (`$(if $(LIB_OBJS),...)`). Don't hardcode object lists.
- **Identical strict warnings** (`WARN_FLAGS`), `-Werror` everywhere. `-Wpedantic` is deliberately omitted (code uses GNU `0b...` binary literals); `-Wconversion` omitted as too noisy for pixel-coord math.
- `BINARY_NAME` defaults to `myApplication` in each platform makefile (the root makefile's default is not inherited — recursive makes use each file's own `?=`). `-MMD -MP` makes header edits trigger rebuilds. Parallelism (`-j$(JOBS)`, from `settings.mk`) is passed explicitly on recursive invocations only; `MAKEFLAGS` is never set globally.

Platform specifics: **RG35XXH** cross-compiles with `aarch64-linux-gnu-*` inside a persistent Docker image (`muos-builder-aarch64`), SDL2 flags from `pkg-config` when `IN_DOCKER=1` else `sdl2-config`, output a stripped binary + generated `.sh` launcher under `dist/rg35xxh/final/$(BINARY_NAME)/`. **macOS** pins `-mmacosx-version-min=15.0`; **Linux** `asan` enables LeakSanitizer (macOS does not — use `leaks` or Docker). All build output is under `dist/` (gitignored), partitioned per platform.

## Static analysis (`.clang-tidy`)

`.clang-tidy` (repo root) drives `lint`: `bugprone-*`, `cert-*`, `clang-analyzer-*`, `misc-*`, `performance-*`, `portability-*`, `readability-*` with curated opt-outs (magic numbers, identifier length, cognitive complexity, style nits). **`WarningsAsErrors` is `'*'`**, so clang-tidy exits non-zero on any finding; `lint` is a hard gate on top — both `cppcheck` (`--error-exitcode=1`) and `clang-tidy` propagate exit codes, and missing tools fail hard. `HeaderFilterRegex` (`.*/include/[^/]+/.*\.h$`) only analyzes headers under `include/<module>/` — a header placed directly in `include/` root is skipped.

## Conventions (RULES.md)

`RULES.md` (in French) is the authoritative coding standard — read it before adding modules. Load-bearing rules:

- **Static allocation by default** — `malloc`/`calloc` only when data must outlive its scope or is too large for the stack; every `malloc` needs a matching `free`. Prefer `initXyz(&obj, ...)` over `obj = createXyz(...)` (no hidden malloc).
- **Strict `include/` ↔ `src/` symmetry** — headers in `include/`, implementation in `src/`, mirrored subdir for subdir. Adding `src/x/x.c` + `include/x/x.h` is enough (no makefile edit) as long as some compiled `.c` `#include`s it.
- **`const` on read-only pointer params**; header and `.c` signatures must match. **No defensive `NULL` checks** on pointers from internal invariants — validate only at boundaries (user input, env vars, file/external API, `malloc`/`fopen`); use `assert` for paranoia, never a silent `if`.
- **Module layout**: system includes first, then own header, `// STATICS` then `// FUNCTIONS` section markers, private helpers `static` and named `on<Action>`, public functions prefixed with the module name (`game_`, `menu_`, `input_mapping_`, `pix_`, `level_NN_`). `switch` always has `default: break;`.
- **DRY across input sources** — keyboard and joystick branches must call the *same* `static` helper, never duplicate action logic. Rule of three for extraction.
- **Cross-platform parity** — behavior must be identical on macOS and RG35XXH; adopt the stricter platform's constraint everywhere (the "RULES.md §2" the makefiles cite).
- **Timing**: VSync-driven loop; never `SDL_Delay` in the main loop; use `SDL_GetTicks()` + last-action-time gating (see `ACTION_TICK_INTERVAL_MS`).
- **Sprites**: 16×16 as 256-element `uint8_t` arrays, `0` reserved for transparency, centralized in `src/variables/globalSprites.c`.

## Reference docs

`docs/` (French) contains learning/reference material, not project spec: `docs/Langage_C/` (C language notes), `docs/SDL2/` (SDL2 API notes), `docs/RG35XXH/MAPPING_KEYS.md` (the handheld's physical button/axis/hat layout — useful when editing `input_mapping` defaults).
