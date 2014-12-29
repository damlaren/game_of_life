#include "BasicBoard.h"
#include <assert.h>

using namespace std;

BasicBoard::BasicBoard(int rows, int columns) :
  Board(rows, columns)
{
  mBoard.resize(rows, vector<bool>());
  for (vector<bool> v : mBoard)
  {
    v.resize(columns, false);
  }
}

void BasicBoard::setCell(int i, int j, bool alive)
{
  assert(i < mRows);
  assert(j < mColumns);
  mBoard[i][j] = alive;
}

void BasicBoard::update()
{
  // TODO
}
