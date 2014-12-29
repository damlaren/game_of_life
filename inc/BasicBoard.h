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

  void setCell(int i, int j, bool alive);

 public:
  BasicBoard(int rows, int columns); /// Constructor.

  void update();

};
