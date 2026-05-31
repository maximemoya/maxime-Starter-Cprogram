# Parallel jobs (default to number of CPU cores minus one)
JOBS ?= $(shell n=$$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 1); if [ $$n -le 1 ]; then echo 1; else echo $$((n-1)); fi)
# Do not set MAKEFLAGS globally — pass -j$(JOBS) explicitly to recursive make when needed
