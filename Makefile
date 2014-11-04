sudoku: src/main.o src/io.o src/sudoku.o
	clang++ -std=c++11 -o $@ $^ -lc++

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
	
row_iterator_test: test/test_row_iterator
	./test/test_row_iterator

test/test_row_iterator: test/test_row_iterator.o src/sudoku.o
	clang++ -std=c++11 -o $@ $^ -lc++	
	
column_iterator_test: test/test_column_iterator
	./test/test_column_iterator

test/test_column_iterator: test/test_column_iterator.o src/sudoku.o
	clang++ -std=c++11 -o $@ $^ -lc++	

region_iterator_test: test/test_region_iterator
	./test/test_region_iterator

test/test_region_iterator: test/test_region_iterator.o src/sudoku.o
	clang++ -std=c++11 -o $@ $^ -lc++	
	
is_valid_test: test/test_is_valid
	./test/test_is_valid

test/test_is_valid: test/test_is_valid.o src/sudoku.o
	clang++ -std=c++11 -o $@ $^ -lc++	

available_numbers_test: test/test_available_numbers
	./test/test_available_numbers

test/test_available_numbers: test/test_available_numbers.o src/sudoku.o
	clang++ -std=c++11 -o $@ $^ -lc++		

.SUFFIXES: .cc .o
.cc.o:
	clang++ -c -std=c++11 -stdlib=libc++ $< -o $@

