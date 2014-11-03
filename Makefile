sudoku: sudoku.o
	clang -o $@ $^

.SUFFIXES: .cc .o
.cc.o:
	clang -c -x c++ -std=c++11 -stdlib=libc++ $< -o $@

