#include <array>
#include <algorithm>
#include <iterator>
#include <tuple>

#include "sudoku.hh"

using std::array;
using std::iterator;
using std::forward_iterator_tag;

typedef int row;
typedef int column;
typedef int region;

class row_iterator : public iterator<forward_iterator_tag, int> {
  board& b;
  int i;
public:
  row_iterator(board& bb, row r) : b(bb), i(size*r) {}
  row_iterator(const row_iterator& ri) : b(ri.b), i(ri.i) {}
  row_iterator& operator++() {++i; return *this;}
  row_iterator operator++(int) {row_iterator t(*this); operator++(); return t;}
  bool operator==(const row_iterator& ri) {return b == ri.b && i == ri.i;}
  bool operator!=(const row_iterator& ri) {return b != ri.b || i != ri.i;}
  int& operator*() {return b[i];}
};

row_iterator row_begin(board& b, row r) {return row_iterator(b, r);}
row_iterator row_end(board& b, row r) {return row_iterator(b, r+1);}

class column_iterator : public iterator<forward_iterator_tag, int> {
  board& b;
  int i;
public:
  column_iterator(board& bb, column c) : b(bb), i(c) {}
  column_iterator(const column_iterator& ci) : b(ci.b), i(ci.i) {}
  column_iterator& operator++() {i += size; i %= index_max; return *this;}
  column_iterator operator++(int) {column_iterator t(*this); operator++(); return t;}
  bool operator==(const column_iterator& ci) {return b == ci.b && i == ci.i;}
  bool operator!=(const column_iterator& ci) {return b != ci.b || i != ci.i;}
  int& operator*() {return b[i];}
};

column_iterator column_begin(board& b, column c) {return column_iterator(b, c);}
column_iterator column_end(board& b, column c) {return column_iterator(b, c+1);}

// the multiple of k that is less than i
int to_div(int i, int k) {return k * (i/k)} 

class region_iterator : public iterator<forward_iterator_tag, int> {
  board& b;
  int i;
public:
  region_iterator(board& bb, row r, column c) : b(bb), i(size*to_div(r, regn) + to_div(c, regn)) {}
  region_iterator(const region_iterator& ri) : b(ri.b), i(ri.i) {}
  region_iterator& operator++() {++i; 
    if (0 == i % regn) i += (size - regn);
    return *this;}
  region_iterator operator++(int) {region_iterator t(*this); operator++(); return t;}
  bool operator==(const region_iterator& ci) {return b == ci.b && i == ci.i;}
  bool operator!=(const region_iterator& ci) {return b != ci.b || i != ci.i;}
  int& operator*() {return b[i];}
};

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
  for (b != e; ++b)
    ++a[abs(*b)];
  a[0] = 1;
  return all_of(a.begin(), a.end(), [](int i){return 1 == i;});
}

template<class Iter>
array<int, 10>::iterator 
available_numbers(Iter b, Iter e, array<int, 10>& a) {
  for (int i=0; i <10; ++i) a[i] = i;
  for (; b != e; ++b) a[abs(*b)] = 0;
  return remove(a.begin(), a.end(), 0);
}

template<class NumIter> 
void 
fill_region(region_iterator& rb, region_iterator& re, NumIter& b) {
  for (; rb != re; ++rb)
    if (*rb <= 0)
      *rb = -*b++;
}

class Region {
  board& b;
  region r;
  array<int, 10> nums;
  array<int, 10>::iterator nums_end;
public:
  Region(board& bb, region rr} 
  : b(bb), r(rr), nums_end(available_numbers(region_begin(bb, rr), region_end(bb, rr), nums)) {}
  Region(const Region& rr) : b(rr.b), r(rr.r), nums(rr.nums), nums_end(rr.nums_end) {}
  region region_number() const {return r;}
  bool next() {
    while (true) {
      fill_region(region_begin(b, r), region_end(b, r), nums.begin()); // todo: this is std::copy
      if (is_valid(region_begin(b, r), region_end(b, r))
        return true;
      if (!next_permutation(nums.begin(), nums_end))
        return false;
    }
  }
};

tuple<tuple<row,row>, tuple<column,column>>
affected_area(const array<Region, 9> a, int k) {
  row max_row = 0, min_row = 8;
  column max_col = 0, min_col = 8;
  for (int i=0; i < k; ++i) {
    tuple<row,row> t = region_rows(a[i].region_number());
    max_row = max(get<1>(t), max_row);
    min_row = min(get<0>(t), min_row);
    tuple<column,column> t = region_columns(a[i].region_number());
    max_col = max(get<1>(t), max_col);
    min_col = min(get<0>(t), min_col);
  }
  return make_tuple(make_tuple(min_row,max_row), make_tuple(min_col, max_col));
}

bool area_valid(board& b, tuple<row,row>, tuple<column,column>> t) {
  tuple <row, row> rows = get<0>(t);
  tuple <column, column> columns = get<1>(t);
  for (row r = get<0>(rows); r <= get<1>(rows); ++r)
    if (!is_valid(row_begin(b, r), row_end(b, r)))
      return false;
  for (column c = get<0>(columns); c <= get<1>(columns); ++c)
    if (is_valid(column_begin(b, c), column_end(b, c)))
      return false;
  return true;
}

bool solve(board& b) {
  array<Region, 9> a;
  for (int i = 0; i < 9; ++i) a[i] = Region(b, i);
  int k = 0;
  while (true) {
    if (k < 0) return false;
    if (8 < k) return true;
    if (!a[k].next()) --k;
    if area_valid(affected_area(a, k)) ++k;
  }
}
