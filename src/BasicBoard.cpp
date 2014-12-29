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

bool BasicBoard::getCell(int i, int j) const
{
  assert(i < mRows);
  assert(j < mColumns);
  return mBoard[i][j];
}

void BasicBoard::setCell(int i, int j, bool alive)
{
  assert(i < mRows);
  assert(j < mColumns);
  mBoard[i][j] = alive;
}

void BasicBoard::update()
{
  // Do the dumbest thing possible: make a copy of the board and update that.
  // This is not an efficient way of doing things, but a simple implementation
  // is still useful as a baseline output.
  std::vector< std::vector<bool> > oldBoard = mBoard;

  // Just count up neighbors of each cell one by one.
  for (int i = 0; i < mRows; i++)
  {
    for (int j = 0; j < mColumns; j++)
    {
      int nbrCount = 0;

      for (int di = -1; di <= 1; di++)
      {
	for (int dj = -1; dj <= 1; dj++)
	{
	  if ((di == 0) && (dj == 0))
	  {
	    continue;
	  }

	  int iNbr = i + di;
	  int jNbr = j + dj;
	  if ((iNbr < 0) || (jNbr < 0) ||
	      (iNbr >= mRows) || (jNbr >= mColumns))
	  {
	    continue;
	  }

	  nbrCount += (oldBoard[iNbr][jNbr] == true);
	}
      }

      // Update cell according to neighbor count.
      if (mBoard[i][j])
      {
	if ((nbrCount < 2) || (nbrCount > 3))
	{
	  mBoard[i][j] = false;
	}
      }
      else
      {
	if (nbrCount == 3)
	{
	  mBoard[i][j] = true;
	}
      }
    }
  }

  mBoard = oldBoard;
}
