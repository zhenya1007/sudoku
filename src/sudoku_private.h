#ifndef SUDOKU_PRIVATE_H
#define SUDOKU_PRIVATE_H

#include <iterator>
#include <array>
#include "sudoku.h"

typedef int row;
typedef int column;
typedef int region;

class row_iterator : public std::iterator<std::forward_iterator_tag, int> {
  board& b;
  int i;
public:
  row_iterator(board& bb, row r);
  row_iterator(row_iterator const& ri);
  row_iterator& operator++();
  row_iterator operator++(int);
  bool operator==(const row_iterator& ri);
  bool operator!=(const row_iterator& ri);
  int& operator*();
};

row_iterator row_begin(board& b, row r);
row_iterator row_end(board& b, row r);

class column_iterator : public std::iterator<std::forward_iterator_tag, int> {
  board& b;
  int i;
public:
  column_iterator(board& bb, column c);
  column_iterator(const column_iterator& ci);
  column_iterator& operator++();
  column_iterator operator++(int);
  bool operator==(const column_iterator& ci);
  bool operator!=(const column_iterator& ci);
  int& operator*();
};

column_iterator column_begin(board& b, column c);
column_iterator column_end(board& b, column c);

class region_iterator : public std::iterator<std::forward_iterator_tag, int> {
  board& b;
  int i;
public:
  region_iterator(board& bb, row r, column c);
  region_iterator(const region_iterator& ri);
  region_iterator& operator++();
  region_iterator operator++(int);
  bool operator==(const region_iterator& ci);
  bool operator!=(const region_iterator& ci);
  int& operator*();
};

region_iterator region_begin(board& b, region r);
region_iterator region_end(board& b, region r);

int ignore_sign(int i);

std::array<int, 10>::iterator 
available_numbers(region_iterator b, region_iterator e, std::array<int, 10>& a);


#endif
