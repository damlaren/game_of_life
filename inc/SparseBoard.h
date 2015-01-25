#ifndef GOL_SPARSE_BOARD_H
#define GOL_SPARSE_BOARD_H

#include "Board.h"
#include <map>
#include <set>
#include <utility>

/// A single row of the board. For each index, record if that cell is alive.
typedef std::set<CellIndex> BoardRow;

/// Representation of the entire board.
typedef std::map<CellIndex, BoardRow> BoardRep;

/// Counts of the number of neighbors of a cell.
typedef std::map<CellIndex, std::map<CellIndex, int> > NeighborCount;

/**
 * A more memory-efficient implementation of the Board API
 * which accepts coordinates anywhere in the signed
 * 64-bit range.
 */
class SparseBoard : public Board
{
protected:
    BoardRep mBoard;

	/// Update neighbor count by looking at cells in row i.
	void updateNeighborCount(CellIndex i, NeighborCount& nbrs) const;

	/// Create new cells based on neighbor count in row i.
	void birthCells(CellIndex i, const NeighborCount& nbrs);

public:
    SparseBoard();

    bool getCell(CellIndex i, CellIndex j) const;

    void setCell(CellIndex i, CellIndex j, bool alive);

    void clearBoard();

    void update();

    const int8_t* getBitmap(CellIndex iOffset, CellIndex jOffset, int &width, int& height);

    bool getFirstLiveCell(CellIndex& i, CellIndex& j) const;

    bool getNextLiveCell(CellIndex& i, CellIndex& j) const;
};

#endif