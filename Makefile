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
	flex -o ./build/lexer.cpp -i ./lexical_analysis/lexer.l
	bison -d ./parser/parser.ypp -o ./build/parser.cpp
	g++ -g main.cpp ./build/lexer.cpp ./build/parser.cpp ./driver/driver.cpp ./printer/astPrinter.cpp ./printer/prettyPrinter.cpp ./semantic/semantic.cpp ./code_generator/code_generator.cpp -o output.out 
	./output.out < test.crbc
# Tests 
test: build
	bash ./test.sh

test/ctest: build
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
	./build/carbonic_c < example.crbc 2> output.ll
	lli output.ll