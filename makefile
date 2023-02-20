lexer: 
	flex -o lexer.cpp -i lexer.l
	g++ lexer.cpp -o lexer.out

test: 
	bash tests.sh

clean:
	rm lexer.out lexer.cpp