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

# Cleaning
clean:
	rm -rf build
	rm -rf tests/outputs
	rm -f output.txt

build:
	mkdir build
	cd build && cmake .. && make

run: build
	./build/carbonic_c < example.crbc 2> output.ll
	lli output.ll
