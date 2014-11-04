# Choosing a language
The most straightforward way I can think of to solve the Sudoku puzzle is
a backtracking algorithm that just tries all legal values for each square.

Since we are going for speed, a compiled language would be preferable to an
interpreted language, with JIT-ted language being either slighly worse or
slightly better than compiling, depending on what one believes about JIT.

The most sophysticated data structure I am likely to need is an array of
numbers, so I cannot eliminate any languages based on that criteria.

While it would be nice to parallelize, I don't exactly see how to parallelize
the backtracking algorithm effectively.

Finally, all other things being equal, it would be nice to use a language I
have already used in the past.

Looking at the C++11 standard, it looks like futures are "close enough" to
thread pools for my purposes here.  So C++ it is.
