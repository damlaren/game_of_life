#include "Board.h"

Board::Board(int rows, int columns) :
  mRows(rows), mColumns(columns)
{
}

bool Board::matches(const Board& other) const
{
  if ((mRows != other.mRows) || (mColumns != other.mColumns))
  {
    return false;
  }

  for (int i = 0; i < mRows; i++)
  {
    for (int j = 0; j < mColumns; j++)
    {
      if (getCell(i, j) != other.getCell(i, j))
      {
	return false;
      }
    }
  }
    
  return true;
}
