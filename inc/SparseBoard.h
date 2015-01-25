#ifndef GOL_SPARSE_BOARD_H
#define GOL_SPARSE_BOARD_H

#include "Board.h"
#include <map>

/// A single row of the board. For each index, record if that cell is alive.
typedef std::map<CellIndex, bool> BoardRow;

/// Representation of the entire board.
typedef std::map<CellIndex, BoardRow> BoardRep;

/**
 * A memory-efficient implementation of the Board API
 * which accepts coordinates anywhere in the signed
 * 64-bit range.
 */
class SparseBoard : public Board
{
protected:
    BoardRep mBoard;

public:
    SparseBoard(); /// Constructor.

    bool getCell(CellIndex i, CellIndex j) const;

    void setCell(CellIndex i, CellIndex j, bool alive);

    void clearBoard();

    void update();

    virtual const int8_t* getBitmap(CellIndex iOffset, CellIndex jOffset, int &width, int& height);

    bool getFirstLiveCell(CellIndex& i, CellIndex& j) const;

    bool getNextLiveCell(CellIndex& i, CellIndex& j) const;
};

#endif