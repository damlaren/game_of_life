#include "Board.h"

#include <assert.h>
#include <fstream>
#include <iostream>

Board::Board()
{
}

Board::~Board()
{
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

bool Board::writeBoard(const std::string& fileName)
{
    std::ofstream outFile(fileName.c_str());

    CellIndex i, j;
    if (getFirstLiveCell(i, j))
    {
        outFile << "(" << j << "," << i << ")" << std::endl;
    }
    while (getNextLiveCell(i, j))
    {
        outFile << "(" << j << "," << i << ")" << std::endl;
    }

    outFile.close();

    return true;
}

bool Board::matches(const Board& other) const
{
  CellIndex i1, j1, i2, j2;
  if (getFirstLiveCell(i1, j1) != other.getFirstLiveCell(i2, j2))
  {
      return false;
  }

  while ((i1 == i2) && (j1 == j2))
  {
      bool getThis = getNextLiveCell(i1, j1);
      bool getOther = other.getNextLiveCell(i2, j2);
      if (getThis != getOther)
      {
          return false;
      }

      // no more live cells in either one and all matched so far; they're good.
      if (!getThis)
      {
          assert(getThis == getOther);
          return true;
      }
  }

  // A mismatch in live cells exists
  return false;
}
