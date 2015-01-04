#include "SparseBoard.h"
#include <assert.h>

using namespace std;

SparseBoard::SparseBoard() :
    Board()
{
    // TODO
    //mRows = rows;
    //mColumns = columns;

    mBoard = BoardRep();
}

bool SparseBoard::getCell(CellIndex i, CellIndex j) const
{
    auto iIter = mBoard.find(i);
    if (iIter != mBoard.end())
    {
        auto jIter = iIter->second.find(j);
        if (jIter != iIter->second.end())
        {
            return jIter->second;
        }
    }
    return false;
}

void SparseBoard::setCell(CellIndex i, CellIndex j, bool alive)
{
    // TODO: later, we'll want to clear out unneeded map entries.
    // Move to a model where if the entry exists, the cell is alive.
    mBoard[i][j] = alive;
}

void SparseBoard::update()
{
    // Do this the stupid way for now. First, build a count of all neighbors of each cell.
    map< CellIndex, map<CellIndex, int8_t> > nbrs = map< CellIndex, map<CellIndex, int8_t> >();
    for (auto iIter = mBoard.begin(); iIter != mBoard.end(); iIter++)
    {
        CellIndex i = iIter->first;
        for (auto jIter = iIter->second.begin(); jIter != iIter->second.end(); jIter++)
        {
            CellIndex j = jIter->first;

            // Update neighbor count.
            for (int di = -1; di <= 1; di++)
            {
                CellIndex iNbr = i + di;
                map<CellIndex, int8_t>& nbrsRow = nbrs[iNbr];
                for (int dj = -1; dj <= 1; dj++)
                {
                    if ((di == 0) && (dj == 0))
                    {
                        continue;
                    }

                    CellIndex jNbr = j + dj;
                    auto nbrIter = nbrsRow.find(jNbr);
                    if (nbrIter == nbrsRow.end())
                    {
                        nbrIter = nbrsRow.insert(make_pair(jNbr, 0)).first;
                    }
                    assert(nbrIter != nbrsRow.end());

                    nbrIter->second++;
                }
            }
        }
    }

    // Then from neighbor count, update which cells are alive.
    BoardRep oldBoard = mBoard;
    mBoard.clear();
    for (auto iNbrIter = nbrs.begin(); iNbrIter != nbrs.end(); iNbrIter++)
    {
        CellIndex i = iNbrIter->first;
        for (auto jNbrIter = iNbrIter->second.begin(); jNbrIter != iNbrIter->second.end(); jNbrIter++)
        {
            CellIndex j = jNbrIter->first;
            int8_t nbrCount = jNbrIter->second;

            // Test if cell is alive in last version of board
            bool alive = false;
            auto iOldIter = oldBoard.find(i);
            if (iOldIter != oldBoard.end())
            {
                auto jOldIter = iOldIter->second.find(j);
                if (jOldIter != iOldIter->second.end())
                {
                    alive = jOldIter->second;
                }
            }

            if (alive)
            {
                if ((nbrCount >= 2) && (nbrCount <= 3))
                {
                    // live cell with good neigbor count stays alive
                    mBoard[i][j] = true;
                }
            }
            else
            {
                if (nbrCount == 3)
                {
                    // dead cell with 3 neighbors becomes alive
                    mBoard[i][j] = true;
                }
            }
        }
    }
}

const int8_t* SparseBoard::getBitmap(CellIndex iOffset, CellIndex jOffset, int &width, int& height)
{
    return Board::getBitmap(iOffset, jOffset, width, height);
}

bool SparseBoard::getFirstLiveCell(CellIndex& i, CellIndex& j) const
{
    // TODO update for live-cell-only model
    for (auto iIter = mBoard.begin(); iIter != mBoard.end(); iIter++)
    {
        for (auto jIter = iIter->second.begin(); jIter != iIter->second.end(); jIter++)
        {
            if (jIter->second)
            {
                i = iIter->first;
                j = jIter->first;
                return true;
            }
        }
    }
    return false;
}

bool SparseBoard::getNextLiveCell(CellIndex& i, CellIndex& j) const
{
    // TODO update for live-cell-only model
    auto iIter = mBoard.find(i);
    assert(iIter != mBoard.end());
    auto jIter = iIter->second.find(j);
    assert(jIter != iIter->second.end());
    jIter++;

    bool firstIteration = true;
    for (; iIter != mBoard.end(); iIter++)
    {
        if (!firstIteration)
        {
            jIter = iIter->second.begin();
        }
        for (; jIter != iIter->second.end(); jIter++)
        {
            if (jIter->second)
            {
                i = iIter->first;
                j = jIter->first;
                return true;
            }
        }
        firstIteration = false;
    }
    return false;
}

void SparseBoard::clearBoard()
{
    mBoard.clear();
}