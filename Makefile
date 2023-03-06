lexer: 
	flex -o lexer.cpp -i lexer.l
test: 
	bash tests.sh
parser:
	bison -d parser.ypp -o parser.cpp
clean:
	rm lexer.out lexer.cpp