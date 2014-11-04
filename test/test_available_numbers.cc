#include <iostream>
#include <array>
#include <algorithm>

#include "../src/sudoku.h"
#include "../src/sudoku_private.h"

using std::cout;
using std::endl;
using std::cerr;
using std::array;

// just print things out so I can eyeball them, and see "if it looks right"
int main(int argc, char* argv[]) {
  board b;
  for (int i = 0; i < count; ++i) {
    b[i] = i;
  }
  int j = 0;
  fill(region_begin(b, 5), region_end(b, 5), 0);
  for (region_iterator p = region_begin(b, 5); p != region_end(b, 5); ++p)
    if (++j % 2 == 0) 
      *p = j;
  array<int, 10> nums;
  region_iterator rb = region_begin(b, 5);
  region_iterator re = region_end(b, 5);
  int available = available_numbers(rb, re, nums);
  cout << "should see odd numbers in the interval [1, 9] below:" << endl;
  for (array<int, 10>::iterator p = nums.begin(); p != nums.begin() + available; ++p)
    cout << *p << ", ";
  cout << endl;
  return 0;
}

