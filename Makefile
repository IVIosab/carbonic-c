# Tests 
test: build
	cd build && ctest

# Cleaning
clean:
	rm -rf build
	rm -rf tests/outputs
	rm -f output.txt

.PHONY: build

BUILD_ALL_RUN := $(shell if [ -f build/CMakeCache.txt ]; then echo "1"; else echo "0"; fi)

build:
    ifeq ($(BUILD_ALL_RUN),1)
	$(info build/all has been run before. Running partial build...)
	cd build && make
    else 
	$(info build/all has not been run before. Running build/all...)
	$(MAKE) build/all
    endif

build/all: clean
	mkdir -p build
	cd build && cmake .. && make

run: build
	./build/carbonic_c 2> output.out

run/file: build
	./build/carbonic_c < input.crbc 2> output.ll
	lli output.ll > output.out
	mv -f output.ll build