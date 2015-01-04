#include "BasicBoard.h"
#include <assert.h>

using namespace std;

BasicBoard::BasicBoard(CellIndex rows, CellIndex columns) :
  Board()
{
  mRows = rows;
  mColumns = columns;

  mBoard.resize(rows, vector<bool>());
  for (vector<bool>& v : mBoard)
  {
    v.resize(columns, false);
  }
}

bool BasicBoard::getCell(CellIndex i, CellIndex j) const
{
    if ((i >= 0) && (j >= 0) && (i < mRows) && (j < mColumns))
    {
        return mBoard[i][j];
    }
    return false;
}

void BasicBoard::setCell(CellIndex i, CellIndex j, bool alive)
{
    if ((i >= 0) && (j >= 0) && (i < mRows) && (j < mColumns))
    {
        mBoard[i][j] = alive;
    }
}

void BasicBoard::update()
{
  // Do the dumbest thing possible: make a copy of the board and update that.
  // This is not an efficient way of doing things, but a simple implementation
  // is still useful as a baseline output.
  std::vector< std::vector<bool> > oldBoard = mBoard;

  // Just count up neighbors of each cell one by one.
  for (CellIndex i = 0; i < mRows; i++)
  {
    for (CellIndex j = 0; j < mColumns; j++)
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

          CellIndex iNbr = i + di;
          CellIndex jNbr = j + dj;
	      if ((iNbr < 0) || (jNbr < 0) || (iNbr >= mRows) || (jNbr >= mColumns))
	      {
	        continue;
	      }

	      nbrCount += (oldBoard[iNbr][jNbr] == true);
	    }
      }

      // Update cell according to neighbor count.
      if (oldBoard[i][j])
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
}

bool BasicBoard::getFirstLiveCell(CellIndex& i, CellIndex& j) const
{
    i = 0;
    j = -1;
    return getNextLiveCell(i, j);
}

bool BasicBoard::getNextLiveCell(CellIndex& i, CellIndex& j) const
{
    j = j + 1;
    for (; i < mRows; i++)
    {
        for (; j < mColumns; j++)
        {
            if (mBoard[i][j])
            {
                return true;
            }
        }
        j = 0;
    }

    return false;
}

void BasicBoard::clearBoard()
{
    for (CellIndex i = 0; i < mRows; i++)
    {
        for (CellIndex j = 0; j < mColumns; j++)
        {
            mBoard[i][j] = false;
        }
    }
}