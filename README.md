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

## Linux:

- Linux:

  - `flex` **2.6.4**
  - `g++` **11.2.0**
  - `make` **4.3**
  - `llvm` **14.0.0**

  Installing dependencies:

        sudo apt install make=4.3-4.1build1
        sudo apt install flex=2.6.4-8build2
        sudo apt install g++=4:11.2.0-1ubuntu1
        sudo apt install llvm-14

