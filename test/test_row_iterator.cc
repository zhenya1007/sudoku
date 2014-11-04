#include <iostream>

#include "../src/sudoku.h"
#include "../src/sudoku_private.h"

using std::cout;
using std::endl;

// just print things out so I can eyeball them, and see "if it looks right"
int main(int argc, char* argv[]) {
  board b;
  for (int i = 0; i < count; ++i) {
    b[i] = i;
  }
  for (int i = 0; i < size; ++i) {
    for (row_iterator p = row_begin(b, i); p != row_end(b,i); ++p)
      cout << *p << ", ";
    cout << endl;
  }
  return 0;
}
