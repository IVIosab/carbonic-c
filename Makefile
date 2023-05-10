# Stages 
lexer: 
	flex -o lexer.cpp -i lexer.l
parser:
	bison -d parser.ypp -o parser.cpp

# Full execution
full_lexer:
	flex -o lexer.cpp -i lexer.l
	g++ lexer.cpp -o output.out
	
full_parser:
	flex -o lexer.cpp -i lexer.l
	bison -d parser.ypp -o parser.cpp
	g++ -g main.cpp lexer.cpp parser.cpp driver.cpp astPrinter.cpp prettyPrinter.cpp semantic.cpp -o output.out 
	./output.out < test.crbc
# Tests 
test: build
	bash ./test.sh

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
	./build/carbonic_c > output.txt