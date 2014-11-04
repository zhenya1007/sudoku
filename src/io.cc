#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include "sudoku.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::istringstream;
using std::getline;
using std::replace;
using std::ios;
using std::cerr;

void print_board(const board& b) {
  for (int i=0; i < size; ++i) {
    for (int j=0; j < size; ++j) {
      cout << abs(b[i*size + j]);
      if (j < size - 1) cout << ",";
    }
    cout << endl;
  }
}

bool read_board(board& b) {
  string line;
  cin.exceptions(ios::failbit|ios::badbit);
  try {
    for (int i=0; i < size; ++i) {
      getline(cin, line);
      replace(line.begin(), line.end(), ',', ' ');
      istringstream is(line);
      is.exceptions(ios::failbit|ios::badbit);
      for (int j=0; j < size; ++j)
        is >> b[i*size + j];
    }
  } 
  catch (ios::failure e) {
    cerr << "I/O error reading the board data" << endl;
    return false;
  }
  return true;
}
