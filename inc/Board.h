#ifndef GOL_BOARD_H
#define GOL_BOARD_H

#include <iterator>
#include <utility>

/**
 * Represents a GOL board that can update to a new state based on GOL rules.
 * Cells are specified starting with (0, 0) at the upper-left of the board.
 */
class Board
{
 protected:
  int mRows; /// Number of rows in game board.
  int mColumns; /// Number of columns in game board.

 public:
  Board(int rows, int columns); /// Constructor.
  virtual ~Board(); /// Oops. Don't forget this.

  /// Returns whether cell (i, j) is alive.
  virtual bool getCell(int i, int j) const = 0;

  /// Set whether cell (i, j) is alive.
  /// (i, j) must be a valid cell within the board.
  virtual void setCell(int i, int j, bool alive) = 0;

  /// Update entire board to next simulation state.
  virtual void update() = 0;

  /// Set all cells to dead state.
  void clearBoard();

  /// Set live members on board by passing a pair of iterators to pairs
  /// specifying (row, column) of live cells.
  template <typename Iterator>
  void initialize(const Iterator first, const Iterator last)
  {
    clearBoard();

    for (Iterator it = first; it != last; it++)
    {
      const std::pair<int, int> cell = *it;
      setCell(cell.first, cell.second, true);
    }
  }

  /// Returns whether cell configuration on this board matches the other.
  bool matches(const Board& other) const;
};

#endif