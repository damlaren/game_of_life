#ifndef GOL_BASIC_BOARD_H
#define GOL_BASIC_BOARD_H

#include "Board.h"
#include <vector>

/**
 * A simple implementation of the Board API.
 */
class BasicBoard : public Board
{
 protected:
  /// Board representation using std::vector's bool specialization, which uses
  /// one bit per cell.
  std::vector< std::vector<bool> > mBoard;

 public:
  BasicBoard(int rows, int columns); /// Constructor.

  bool getCell(int i, int j) const;
  
  void setCell(int i, int j, bool alive);

  void clearBoard();
  
  void update();
};

#endif