#ifndef GOL_BOARD_H
#define GOL_BOARD_H

#include <stdint.h>
#include <string>

typedef int64_t CellIndex;

/**
 * Represents a GOL board that can update to a new state based on GOL rules.
 * Cells are internally specified in (row, column) format, starting with
 * (0, 0) at the upper-left of the board.
 * (Oops, this is different from the input/output file format, which is
 *  (column, row).).
 */
class Board
{
 public:
  Board(); /// Constructor.
  virtual ~Board(); /// Oops. Don't forget this.

  /// Returns whether cell (i, j) is alive.
  /// Returns false if (i, j) is outside range represented by board.
  virtual bool getCell(CellIndex i, CellIndex j) const = 0;

  /// Sets whether cell (i, j) is alive.
  /// Does nothing if (i, j) is outside range represented by board.
  virtual void setCell(CellIndex i, CellIndex j, bool alive) = 0;

  /// Update entire board to next simulation state.
  virtual void update() = 0;

  /// Get first live cell, if any, and return its indices.
  /// Return false if not found.
  virtual bool getFirstLiveCell(CellIndex& i, CellIndex& j) const = 0;

  /// Get next live cell after (i, j). TODO (ij)
  /// Return false if no more live cells.
  virtual bool getNextLiveCell(CellIndex& i, CellIndex& j) const = 0;

  /// Set all cells to dead state.
  virtual void clearBoard() = 0;

  /** Load live cells from file one by one into the board.
   * The board must already be constructed.
   * @return whether file could be opened for reading.
   */
  bool loadBoard(const std::string& fileName);

  /// Write all live cells to the board.
  /// @return whether cells could be written to file.
  bool writeBoard(const std::string& fileName);

  /// Returns whether cell liveness on this board matches the other.
  bool matches(const Board& other) const;

  /**
  * Get a bit representation that can be packed into a bitmap
  * for display purposes. This function allocates new memory
  * which must be deleted by the caller.
  *
  * @param iOffset - row at which to start retrieving data
  * @param jOffset - column at which to start retrieving
  * @param width - requested width of bitmap, possibly adjusted by function call
  * @param height - requested height
  */
  virtual const int8_t* getBitmap(CellIndex iOffset, CellIndex jOffset, int &width, int& height);
};

#endif