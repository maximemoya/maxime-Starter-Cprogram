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

There is no test framework yet. `lint` and `asan` are the quality gates. `lint` requires `cppcheck` and `clang-tidy` (macOS: clang-tidy is expected at `/opt/homebrew/opt/llvm/bin/clang-tidy`; override with `CLANG_TIDY=...`).

## Architecture

Single-source, multi-target C/SDL2 project: one `src/` tree builds for macOS ARM, Linux x86_64, and the RG35XXH (aarch64/muOS) handheld. `src/main.c` is currently a minimal SDL2 window loop; `src/libpixtools/` is the (currently empty) pixel/sprite utility library.

**The three platform makefiles are near-identical by design** — when changing build behavior (warning flags, source discovery, lib logic), apply the change to all relevant makefiles, not just one. The comments cite a `RULES.md §2` principle of "adopt the stricter rule everywhere," so divergence is a smell.

Shared invariants across all makefiles:
- **Sources are auto-discovered** via `find $(SRC_DIR) -name '*.c'`. Adding a `.c` file needs no makefile edit. Object/dep files mirror the `src/` subdir layout under `dist/<platform>/compile/`.
- **`libpixtools` is conditionally linked.** Any `.c` under `src/libpixtools/` is compiled into `libpixtools.a` and linked via `-lpixtools`; everything else is application code. When `src/libpixtools/` has no `.c` files, the library archive step and the `-lpixtools` flag are both skipped automatically (via `$(if $(LIB_OBJS),...)`). Don't hardcode object lists.
- **Identical strict warnings** (`WARN_FLAGS`): `-Werror` is on everywhere. `-Wpedantic` is deliberately omitted because the code intends to use GNU `0b...` binary literals; `-Wconversion` is omitted as too noisy for pixel-coord math. Both are noted as "add later, file by file."
- **`BINARY_NAME`** defaults to `myApplication` in each platform makefile (the root makefile's `maxime_Cprogram` default is not inherited — recursive makes use each file's own `?=`).
- **`-MMD -MP`** generates `.d` files so headers trigger rebuilds automatically.

Platform-specific notes:
- **RG35XXH** (`makefile.rg35xxh`): cross-compiles with `aarch64-linux-gnu-*` inside a persistent Docker image (`muos-builder-aarch64`, Debian bookworm + `crossbuild-essential-arm64` + `libsdl2-dev:arm64`), built on demand by the `setup-docker-image` rule. SDL2 flags come from `pkg-config` when `IN_DOCKER=1`, else `sdl2-config`. Output is a stripped binary plus a generated `.sh` launcher, laid out for muOS under `dist/rg35xxh/final/$(BINARY_NAME)/`.
- **macOS** pins `-mmacosx-version-min=15.0`; **Linux** `asan` enables LeakSanitizer (`detect_leaks=1`), macOS does not (unsupported — use `leaks` or run under Docker).

**Build outputs live under `dist/`** (gitignored), partitioned per platform: `dist/macos/`, `dist/linux/`, `dist/rg35xxh/`.

## Conventions (RULES.md)

`RULES.md` (in French) is the authoritative coding-standard for this project. Read it before adding modules. The load-bearing rules:
- **Static allocation by default** — `malloc`/`calloc` only when data must outlive its scope or is too large for the stack; every `malloc` needs a matching `free`. Prefer `initXyz(&obj, ...)` over `obj = createXyz(...)` (no hidden malloc).
- **Strict `include/` ↔ `src/` symmetry** — headers in `include/`, implementation in `src/`, mirrored subdir for subdir. Adding `src/x/x.c` + `include/x/x.h` is enough (no makefile edit) as long as some existing `.c` `#include`s it.
- **`const` on read-only pointer params**, header and `.c` signatures must match. **No defensive `NULL` checks** on pointers from internal invariants — validate only at boundaries (user input, external API, `malloc`/`fopen`); use `assert` for paranoia, never a silent `if`.
- **Module layout is prescribed**: system includes first, then own header, `// STATICS` then `// FUNCTIONS` section markers, private helpers `static` and named `on<Action>`, public functions prefixed with the module name (`game_`, `input_state_`, `pix_`). `switch` always has `default: break;`.
- **DRY across input sources** — keyboard and joystick branches must call the *same* `static` helper, never duplicate action logic. Rule of three for extraction.
- **Cross-platform parity** — behavior must be identical on macOS and RG35XXH; adopt the stricter platform's constraint everywhere (this is the "RULES.md §2" the makefiles cite).
- **Timing**: VSync-driven loop; never `SDL_Delay` inside the main loop; use `SDL_GetTicks()` + last-action-time timers (delta-time for motion).
- **Sprites**: 16×16 as 256-element `uint8_t` arrays, `0` reserved for transparency, centralized in `src/variables/globalSprites.c`.

Note: RULES.md references modules that don't exist yet in this starter (`pixtools.h`, `input_state`, `PixContext`, `pix_init`, `globalSprites`, the `states/` tree) — they describe the intended structure, not current code.

## Static analysis (.clang-tidy)

`.clang-tidy` (repo root) drives the `lint` target. It enables `bugprone-*`, `cert-*`, `clang-analyzer-*`, `misc-*`, `performance-*`, `portability-*`, `readability-*` with a curated set of opt-outs (magic numbers, identifier length, cognitive complexity, brace/style nits). **`WarningsAsErrors` is empty — lint is advisory, it never fails the build** (unlike compilation, which is `-Werror`). `HeaderFilterRegex` only analyzes headers under `include/<module>/`, so a header placed directly in `include/` root is skipped.

## Other notes

- Parallelism: makefiles intentionally do **not** set `MAKEFLAGS` globally; `-j$(JOBS)` is passed explicitly on recursive invocations only.
