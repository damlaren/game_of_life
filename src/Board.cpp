#include "Board.h"

#include <fstream>
#include <iostream>

Board::Board(int rows, int columns) :
  mRows(rows), mColumns(columns)
{
}

Board::~Board()
{
}

void Board::clearBoard()
{
  for (int i = 0; i < mRows; i++)
  {
    for (int j = 0; j < mColumns; j++)
    {
      setCell(i, j, false);
    }
  }
}

bool Board::loadBoard(const std::string& fileName)
{
    clearBoard();

    std::ifstream inFile(fileName.c_str());
    if (!inFile.is_open())
    {
        std::cerr << "Failed to open " << fileName << std::endl;
        return false;
    }

    // assuming line isn't malformed
    char buf[255]; // TODO check exact size against getline
    while (!inFile.eof())
    {
        inFile.getline(buf, 255);
        std::string line = buf;
        if (line.empty())
        {
            continue;
        }

        const char *WHITESPACE = " \t\r\n"; // TODO: must be something in std for this
        std::size_t end1 = line.find(",");
        std::size_t openParen = line.find("(");
        std::size_t start1 = line.find_first_not_of(WHITESPACE, openParen + 1);
        std::size_t start2 = line.find_first_not_of(WHITESPACE, end1 + 1);

        std::string xStr = line.substr(start1, end1 - start1);
        std::string yStr = line.substr(start2);
        CellIndex x = _atoi64(xStr.c_str()); // TODO: std
        CellIndex y = _atoi64(yStr.c_str());

        setCell(y, x, true);
    }

    inFile.close();

    return true;
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
