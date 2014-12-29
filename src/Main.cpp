#include "BasicBoard.h"
#include <iostream>

using namespace std;

/**
 * GOL entry point.
 */
int main(const int argc, const char* argv[])
{
  Board *board = new BasicBoard(10, 10);
  cout << "Yay" << endl;
  return 0;
}
