# Project executable name
BINARY_NAME ?= maxime_Cprogram

# Parallel jobs (default to number of CPU cores minus one)
JOBS ?= $(shell n=$$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 1); if [ $$n -le 1 ]; then echo 1; else echo $$((n-1)); fi)
# Do not set MAKEFLAGS globally — pass -j$(JOBS) explicitly to recursive make when needed

# Default rule: show usage instructions
.DEFAULT_GOAL := help
help:
	@echo "Compilation Instructions"
	@echo "------------------------"
	@echo "This project supports multiple platforms. Use the following commands:"
	@echo ""
	@echo "For all platforms:"
	@echo "  make clean        		Clean all build artifacts"
	@echo "  make buildOnMacOs  		Build for macOS + RG35XXH"
	@echo "  make checkOnMacOs		Run asan + lint on macOS"
	@echo "  make buildOnLinuxAMDx64	Build for linuxAMDx64 + RG35XXH"
	@echo "  make checkOnLinuxAMDx64	Run asan + lint on linuxAMDx64"
	@echo ""
	@echo "For macOS ARM Mx:"
	@echo "  make -f makefile.mac run"
	@echo "  make -f makefile.mac asan"
	@echo "  make -f makefile.mac lint"
	@echo ""
	@echo "For Linux AMDx64 / Ubuntu (incl. WSL2, native x86_64 build):"
	@echo "  make -f makefile.linuxAMDx64 run"
	@echo "  make -f makefile.linuxAMDx64 asan"
	@echo "  make -f makefile.linuxAMDx64 lint"
	@echo ""
	@echo "For RG35XXH (muOS):"
	@echo "  make -f makefile.rg35xxh docker-build"
	@echo ""

buildOnMacOs:
	$(MAKE) -j$(JOBS) -f makefile.rg35xxh docker-build
	$(MAKE) -j$(JOBS) -f makefile.mac

checkOnMacOs:
	$(MAKE) -f makefile.mac asan
	$(MAKE) -f makefile.mac lint

buildOnLinuxAMDx64:
	$(MAKE) -j$(JOBS) -f makefile.rg35xxh docker-build
	$(MAKE) -j$(JOBS) -f makefile.linuxAMDx64

checkOnLinuxAMDx64:
	$(MAKE) -f makefile.linuxAMDx64 asan
	$(MAKE) -f makefile.linuxAMDx64 lint

clean:
	$(MAKE) -f makefile.mac clean
	$(MAKE) -f makefile.rg35xxh clean
	$(MAKE) -f makefile.linuxAMDx64 clean

.PHONY: help buildOnMacOs checkOnMacOs buildOnLinuxAMDx64 checkOnLinuxAMDx64 clean
