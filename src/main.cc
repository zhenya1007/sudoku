#include <iostream>

#include "sudoku.h"
#include "io.h"

using std::cerr;
using std::endl;

int main(int argc, char** argv) {
  board b;
  if (read_board(b))
    if (solve(b))
      print_board(b);
    else
      cerr << "could not find a solution" << endl;
  else
    cerr << "could not read board data" << endl;
    
}
