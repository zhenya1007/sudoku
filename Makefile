sudoku: src/sudoku.o
	clang -o $@ $^

io_test: test/test_io
	./test/test_io < test/io_test.1.input >test/io_test.1.output
	diff test/io_test.1.input test/io_test.1.output
	if ./test/test_io < test/io_test.2.input >test/io_test.2.output; then \
		echo "the test with io_test.2.input should have failed" 1>&2; \
		false; \
	else \
		true; \
	fi
	
test/test_io: test/test_io.o src/io.o
	clang++ -std=c++11 -o $@ $^ -lc++

.SUFFIXES: .cc .o
.cc.o:
	clang++ -c -std=c++11 -stdlib=libc++ $< -o $@

