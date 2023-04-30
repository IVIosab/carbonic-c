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
test:
	bash test.sh

# Cleaning
clean:
	rm lexer.cpp parser.cpp parser.hpp output.out
	rm -rf tests/outputs

compile:
	mkdir build
	cd build && cmake .. && make

clean_cmake:
	rm -rf build