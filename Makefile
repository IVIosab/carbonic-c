test_parser_test:
	flex -o lexer.cpp -i lexer.l
	bison -d parser.ypp -o parser.cpp
	g++ -g main.cpp lexer.cpp parser.cpp driver.cpp
	./a.out < ./test.crbc

# Stages 
lexer: 
	flex -o lexer.cpp -i lexer.l
parser:
	bison -d parser.ypp -o parser.cpp

# Full execution
full_lexer:
	flex -o lexer.cpp -i lexer.l
	g++ lexer.cpp -o a.out
full_parser:
	flex -o lexer.cpp -i lexer.l
	bison -d parser.ypp -o parser.cpp
	g++ -g main.cpp lexer.cpp parser.cpp driver.cpp -o a.out
	./a.out < ./test.crbc #remove later

# Tests 
test_lexer: 
	bash test_lexer.sh
test_parser:
	bash test_parser.sh

# Cleaning
clean:
	rm lexer.cpp parser.cpp parser.hpp a.out