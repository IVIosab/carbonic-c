say_hello:
		@echo "Hello World"

flex: 
	flex -o lexer.cpp -i lexer.l

clean:
	rm a.out lexer.cpp