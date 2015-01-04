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
  CellIndex mRows; /// Number of rows in game board.
  CellIndex mColumns; /// Number of columns in game board.

  /// Board representation using std::vector's bool specialization, which uses
  /// one bit per cell.
  std::vector< std::vector<bool> > mBoard;

 public:
  BasicBoard(CellIndex rows, CellIndex columns); /// Constructor.

  bool getCell(CellIndex i, CellIndex j) const;
  
  void setCell(CellIndex i, CellIndex j, bool alive);

  void clearBoard();
  
  void update();

  bool getFirstLiveCell(CellIndex& i, CellIndex& j) const;

  bool getNextLiveCell(CellIndex& i, CellIndex& j) const;
};

#endif