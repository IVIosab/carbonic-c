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

build: build/all
	cd build && make

build/all:
	mkdir -p build
	cd build && cmake .. && make

run: build
	./build/carbonic_c > output.txt