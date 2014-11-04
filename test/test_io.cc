#include "../src/io.h"

int main(int argc, char* argv[]) {
  board b;
  if (read_board(b)) {
    print_board(b);
    return 0;
  }
  return 1;
}
