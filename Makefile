lexer: 
	flex -o lexer.cpp -i lexer.l
test: 
	bash tests.sh
parser:
	bison -d parser.ypp -o parser.cpp
test_parser:
	flex -o lexer.cpp -i lexer.l
	bison -d parser.ypp -o parser.cpp
	g++ -g main.cpp lexer.cpp parser.cpp driver.cpp -o a.out
	./a.out < ./test.crbc
clean:
	rm lexer.out lexer.cpp