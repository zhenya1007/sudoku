#include <array>
#include <algorithm>
#include <iterator>
#include <tuple>

using std::array;
using std::iterator;
using std::forward_iterator_tag;

const int regn = 3;
const int size = 9;
const int count = size * size;
const int index_max = count - 1;

typedef array<int, count> board;
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

region_iterator region_begin(board& b, row r, column c) {return region_iterator(b, r, c);}
region_iterator region_end(board& b, row r, column c) {return region_iterator(b, to_div(r,regn)+regn, c);}

tuple<row, column> start_of_region(region r) {return make_tuple(to_div(r, regn), regn * (r % regn));}

tuple<column, column> region_columns(region r) {
  tuple<row, column>t = start_of_region(r);
  return make_tuple(get<1>(t), get<1>(t) + regn - 1);
}

tuple<row, row> region_rows(region r) {
  tuple<row, column>t = start_of_region(r);
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

