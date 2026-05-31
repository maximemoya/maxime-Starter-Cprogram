# Commands

## macOS ARM (M-series)
```sh
make -f makefile.mac run
```

## Linux x86_64 / Ubuntu (incl. WSL2)
```sh
make -f makefile.linuxAMDx64 run
```

## RG35XXH (muOS)
```sh
make -f makefile.rg35xxh docker-build
```

---

# Workflow

## macOS — all targets should pass
```sh
make clean
make buildOnMacOs
make checkOnMacOs
```

## Linux x86_64 — all targets should pass
```sh
make clean
make buildOnLinuxAMDx64
make checkOnLinuxAMDx64
```
