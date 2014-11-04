#include <iostream>
#include <array>
#include <algorithm>

#include "../src/sudoku.h"
#include "../src/sudoku_private.h"

using std::cout;
using std::endl;
using std::cerr;
using std::array;
using std::fill;
using std::all_of;

template<class Iter>
bool
is_valid(Iter b, Iter e) {
  array<int, 10> a;
  fill(a.begin(), a.end(), 0);
  for (; b != e; ++b)
    if (9 < ignore_sign(*b))
      return false;
    else
      ++a[ignore_sign(*b)];
  a[0] = 1;
  return all_of(a.begin(), a.end(), [](int i){return 1 == i;});
}


// just print things out so I can eyeball them, and see "if it looks right"
int main(int argc, char* argv[]) {
  board b;
  for (int i = 0; i < count; ++i) {
    b[i] = i;
  }
  int j = 0;
  int r = 0;
  for (region_iterator p = region_begin(b, 2); p != region_end(b,2); ++p)
      *p = ++j;
  if (is_valid(region_begin(b, 2), region_end(b,2))) {
    for (region_iterator p = region_begin(b, 2); p != region_end(b,2); ++p)
      cout << *p << ", ";
    cout << endl;
    r |= 0;
  }
  else {
    cerr << "Region expected to be valid:";
    for (region_iterator p = region_begin(b, 2); p != region_end(b,2); ++p)
      cerr << *p << ", ";    
    r |= 1;
  }
  j = 0;
  for (region_iterator p = region_begin(b, 2); p != region_end(b,2); ++p)
      *p = j++;
  if (!is_valid(region_begin(b, 2), region_end(b,2))) {
    for (region_iterator p = region_begin(b, 2); p != region_end(b,2); ++p)
      cout << *p << ", ";
    cout << endl;
    r |= 0;
  }
  else {
    cerr << "Region expected to be invalid (9 missing):";
    for (region_iterator p = region_begin(b, 2); p != region_end(b,2); ++p)
      cerr << *p << ", ";    
    r |= 2;
  }
  return r;
}

