# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```sh
make                              # Show platform usage instructions
make build                        # Build for all platforms (macOS + RG35XXH via Docker)
make clean                        # Clean all build artifacts

make -f makefile.mac run          # Build and run on macOS (same as all + execute)
make -f makefile.mac asan         # Clean + rebuild with sanitizers + run (independent from all)
make -f makefile.mac lint         # Static analysis only, no compilation (independent from all)
make checkOnMacOs                 # Run asan + lint on macOS (combines both checks)

make -f makefile.rg35xxh docker-build  # Cross-compile for RG35XXH (ARM64) via Docker

make -f makefile.linuxAMDx64 run     # Build and run on Linux AMDx64 / Ubuntu / WSL2
make -f makefile.linuxAMDx64 asan    # Clean + rebuild with sanitizers + run (includes LeakSanitizer)
make -f makefile.linuxAMDx64 lint    # Static analysis only, no compilation
```

## Architecture

This is a dual-target C project (SDL2) that builds for macOS ARM (M-series) and the RG35XXH handheld (aarch64/muOS) from the same source.

**Source layout:**
- `src/` — all `.c` files, discovered automatically by `find` (no Makefile changes needed when adding files)
- `src/libpixtools/` — pixel/sprite utility library, compiled into a static `libpixtools.a`
- `include/` — shared headers, available via `-Iinclude` on all platforms

**Build outputs:**
- `dist/macos/final/` — macOS binary + `libpixtools.a`
- `dist/rg35xxh/final/myApplication/` — stripped ARM64 binary + `libpixtools.a`
- `dist/rg35xxh/final/myApplication.sh` — launcher script for muOS

**Key design decisions:**
- `libpixtools` is conditionally linked — if `src/libpixtools/` has no `.c` files, the library step and `-lpixtools` linker flag are skipped automatically.
- RG35XXH cross-compilation runs inside a persistent Docker image (`muos-builder-aarch64`) using Debian + `crossbuild-essential-arm64`.
- `BINARY_NAME` is defined once in `makefile` and inherited by sub-makefiles via `?=`.
- Warnings are treated as errors (`-Werror`) on both platforms with identical `WARN_FLAGS`.
