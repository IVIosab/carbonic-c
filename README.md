# Carbonic-C

## Using the lexer:
The process of testing the lexer has been automated with a 'makefile'. 

The make file automates 3 processes:

- `make lexer`: Performs 2 operations in order. It first generates a `C++` code from `lexer.l`, and then compiles that code into an executable lexer.

- `make test`: Runs a set of tests via a bash script and saves the output into `./tests/lexer_outputs`.

- `make clean`: Removes the `C++` code and the executable lexer. 

To compile the lexer an run the tests, run the following in your terminal:

    make lexer
    make test
    make clean

## Dependencies:
- `flex` **2.6.4**
- `g++` **11.3.0**