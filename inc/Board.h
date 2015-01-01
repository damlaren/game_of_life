#ifndef GOL_BOARD_H
#define GOL_BOARD_H

#include <stdint.h>
#include <string>

typedef int64_t CellIndex;

/**
 * Represents a GOL board that can update to a new state based on GOL rules.
 * Cells are specified starting with (0, 0) at the upper-left of the board.
 */
class Board
{
 protected:
  CellIndex mRows; /// Number of rows in game board.
  CellIndex mColumns; /// Number of columns in game board.

 public:
  Board(); /// Constructor.
  virtual ~Board(); /// Oops. Don't forget this.

  /// Returns whether cell (i, j) is alive.
  virtual bool getCell(CellIndex i, CellIndex j) const = 0;

  /// Set whether cell (i, j) is alive.
  /// (i, j) must be a valid cell within the board.
  virtual void setCell(CellIndex i, CellIndex j, bool alive) = 0;

  /// Update entire board to next simulation state.
  virtual void update() = 0;

  /**
    * Get a bit representation that can be packed into a bitmap
    * for display purposes.
    * TODO: probably want to show a small window
    * @param width - width of returned bitmap
    * @param height - height of returned bitmap
    */
  virtual const char* getBitmap(int &width, int& height) = 0;

  /// Set all cells to dead state.
  void clearBoard();

  /// Load live cells from file one by one into the board.
  /// The board must already be constructed.
  /// @return whether cells could be read from file to completion.
  bool loadBoard(const std::string& fileName);

  /// Returns whether cell configuration on this board matches the other.
  bool matches(const Board& other) const;
};

#endif