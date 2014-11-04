#ifndef SUDOKU_DOT_H
#define SUDOKU_DOT_H

#include <array>

const int regn = 3;
const int size = 9;
const int count = size * size;
const int max_index = count - 1;

typedef std::array<int, count> board;

bool solve(board& b);

#endif
