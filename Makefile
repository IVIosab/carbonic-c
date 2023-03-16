test_parser_test:
	flex -o lexer.cpp -i lexer.l
	bison -d parser.ypp -o parser.cpp
	g++ -g main.cpp lexer.cpp parser.cpp driver.cpp
	./output.out < ./test.crbc

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
	g++ -g main.cpp lexer.cpp parser.cpp driver.cpp -o output.out
	./output.out < ./test.crbc #remove later

# Tests 
test:
	bash test.sh

# Cleaning
clean:
	rm lexer.cpp parser.cpp parser.hpp output.out