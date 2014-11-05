#include <array>
#include <algorithm>
#include <iterator>
#include <tuple>
#include <cmath>
#include <vector>
#include <iostream>

#include "sudoku.h"
#include "sudoku_private.h"

using std::array;
using std::iterator;
using std::forward_iterator_tag;
using std::tuple;
using std::make_tuple;
using std::all_of;
using std::fill;
using std::next_permutation;
using std::get;
using std::max;
using std::min;
using std::remove;
using std::vector;
using std::endl;

typedef int row;
typedef int column;
typedef int region;

row_iterator::row_iterator(board& bb, row r) : b(bb), i(size*r) {}
row_iterator::row_iterator(row_iterator const& ri) : b(ri.b), i(ri.i) {}
row_iterator& row_iterator::operator++() {++i; return *this;}
row_iterator row_iterator::operator++(int) {row_iterator t(*this); operator++(); return t;}
bool row_iterator::operator==(const row_iterator& ri) {return b == ri.b && i == ri.i;}
bool row_iterator::operator!=(const row_iterator& ri) {return b != ri.b || i != ri.i;}
int& row_iterator::operator*() {return b[i];}

row_iterator row_begin(board& b, row r) {return row_iterator(b, r);}
row_iterator row_end(board& b, row r) {return row_iterator(b, r+1);}

column_iterator::column_iterator(board& bb, column c) : b(bb), i(c) {}
column_iterator::column_iterator(const column_iterator& ci) : b(ci.b), i(ci.i) {}
column_iterator& column_iterator::operator++() {
  i += size;
  if (max_index + size == i) i = 0;
  if (max_index != i) i %= max_index;
  return *this;}
column_iterator column_iterator::operator++(int) {column_iterator t(*this); operator++(); return t;}
bool column_iterator::operator==(const column_iterator& ci) {return b == ci.b && i == ci.i;}
bool column_iterator::operator!=(const column_iterator& ci) {return b != ci.b || i != ci.i;}
int& column_iterator::operator*() {return b[i];}

column_iterator column_begin(board& b, column c) {return column_iterator(b, c);}
column_iterator column_end(board& b, column c) {return column_iterator(b, (c+1) % size);}

// the multiple of k that is less than i
int to_div(int i, int k) {return k * (i/k);}
int ignore_sign(int i) { if (i < 0) return -i; else return i;}

region_iterator::region_iterator(board& bb, row r, column c) : b(bb), i(size*to_div(r, regn) + to_div(c, regn)) {}
region_iterator::region_iterator(const region_iterator& ri) : b(ri.b), i(ri.i) {}
region_iterator& region_iterator::operator++() {++i; 
  if (0 == i % regn) i += (size - regn);
  return *this;}
region_iterator region_iterator::operator++(int) {region_iterator t(*this); operator++(); return t;}
bool region_iterator::operator==(const region_iterator& ci) {return b == ci.b && i == ci.i;}
bool region_iterator::operator!=(const region_iterator& ci) {return b != ci.b || i != ci.i;}
int& region_iterator::operator*() {return b[i];}

region_iterator region_begin(board& b, row r, column c) {
  return region_iterator(b, r, c);
}

region_iterator region_end(board& b, row r, column c) {
  return region_iterator(b, to_div(r,regn)+regn, c);
}

tuple<row, column> start_of_region(region r) {
  return make_tuple(to_div(r, regn), regn * (r % regn));
}

region_iterator region_begin(board& b, region r) {
  tuple<row, column> t = start_of_region(r);
  return region_begin(b, get<0>(t), get<1>(t));
}

region_iterator region_end(board& b, region r) {
  tuple<row, column> t = start_of_region(r);
  return region_end(b, get<0>(t), get<1>(t));
}

tuple<column, column> region_columns(region r) {
  tuple<row, column> t = start_of_region(r);
  return make_tuple(get<1>(t), get<1>(t) + regn - 1);
}

tuple<row, row> region_rows(region r) {
  tuple<row, column> t = start_of_region(r);
  return make_tuple(get<0>(t), get<0>(t) + regn - 1);
}

region region_from_row_col(row r, column c) {return to_div(r, regn) + to_div(c, regn)/regn;}

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
  for (int i=0; i < 10; ++i) {
    if (a[i] > 1)
      return false;
  }
  return true;
}

int 
available_numbers(region_iterator b, region_iterator e, array<int, 10>& a) {
  for (int i=0; i <10; ++i) a[i] = i;
  for (; b != e; ++b)
  if (ignore_sign(*b) < 10)
    a[ignore_sign(*b)] = 0;
  array<int, 10>::iterator ae = remove(a.begin(), a.end(), 0);
  int k = 0;
  for (array<int, 10>::iterator p = a.begin(); p != ae; ++p)
    ++k;
  return k;
}

template<class NumIter> 
void 
fill_region(region_iterator rb, region_iterator re, NumIter b) {
  for (; rb != re; ++rb)
    if (*rb <= 0)
      *rb = -*b++;
}

class Region {
  board& b;
  region r;
  array<int, 10> nums;
  int available;
public:
  Region(board& bb, region rr)
  : b(bb), r(rr), available(available_numbers(region_begin(bb, rr), region_end(bb, rr), nums)) {}
  Region(const Region& rr) : b(rr.b), r(rr.r), nums(rr.nums), available(rr.available) {}
  region region_number() const {return r;}
  void fill() {
    fill_region(region_begin(b, r), region_end(b, r), nums.begin());
  }
  void empty() {
    array<int, 10> z = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    fill_region(region_begin(b, r), region_end(b, r), z.begin());
  }
  bool next() {
    return next_permutation(nums.begin(), nums.begin() + available);
  }
};

tuple<tuple<row,row>, tuple<column,column>>
affected_area(const vector<Region> a, int k) {
  row max_row = 0, min_row = 8;
  column max_col = 0, min_col = 8;
  for (int i=0; i < k; ++i) {
    tuple<row,row> rs = region_rows(a[i].region_number());
    max_row = max(get<1>(rs), max_row);
    min_row = min(get<0>(rs), min_row);
    tuple<column,column> cs = region_columns(a[i].region_number());
    max_col = max(get<1>(cs), max_col);
    min_col = min(get<0>(cs), min_col);
  }
  return make_tuple(make_tuple(min_row,max_row), make_tuple(min_col, max_col));
}

bool area_valid(board& b, tuple<tuple<row,row>, tuple<column,column>> t) {
  tuple <row, row> rows = get<0>(t);
  tuple <column, column> columns = get<1>(t);
  for (row r = get<0>(rows); r <= get<1>(rows); ++r)
    if (!is_valid(row_begin(b, r), row_end(b, r)))
      return false;
  for (column c = get<0>(columns); c <= get<1>(columns); ++c)
    if (!is_valid(column_begin(b, c), column_end(b, c)))
      return false;      
  return true;
}

bool solve(board& b) {
  vector<Region> a;
  for (int i = 0; i < 9; ++i) a.push_back(Region(b, i));
  int k = 0;
  while (true) {
    if (k < 0) return false;
    if (8 < k) return true;
    a[k].fill();
    if (area_valid(b, affected_area(a, k))) {
       ++k;
       continue;
    }
    if (a[k].next()) // do we have more options at the current level?
      continue;
    a[k].empty();
    while (0 < --k)  // backtrack to the level that has untried options
      if (a[k].next())
        break;
      else
        a[k].empty();
    if (0 < k)
      continue;
    if (!a[k].next())
      return false;
  }
}
