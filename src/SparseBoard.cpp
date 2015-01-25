#include "SparseBoard.h"
#include <assert.h>

using namespace std;

SparseBoard::SparseBoard() :
    Board()
{
    mBoard = BoardRep();
}

bool SparseBoard::getCell(CellIndex i, CellIndex j) const
{
    auto iIter = mBoard.find(i);
    if (iIter != mBoard.end())
    {
		return (iIter->second.find(j) != iIter->second.end());
    }
    return false;
}

void SparseBoard::setCell(CellIndex i, CellIndex j, bool alive)
{
	if (alive)
	{
		mBoard[i].insert(j);
	}
	else
	{
		auto iIter = mBoard.find(i);
		if (iIter != mBoard.end())
		{
			iIter->second.erase(j);
			if (iIter->second.empty())
			{
				mBoard.erase(i);
			}
		}
	}
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
			CellIndex j = *jIter;

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
                alive = (iOldIter->second.find(j) != iOldIter->second.end());
            }

            if (alive)
            {
                if ((nbrCount >= 2) && (nbrCount <= 3))
                {
                    // live cell with good neigbor count stays alive
					setCell(i, j, true);
                }
            }
            else
            {
                if (nbrCount == 3)
                {
                    // dead cell with 3 neighbors becomes alive
					setCell(i, j, true);
                }
            }
        }
    }
}

const int8_t* SparseBoard::getBitmap(CellIndex iOffset, CellIndex jOffset, int &width, int& height)
{
    // round width up to nearest 8 (bits)
    if (width % 8 != 0)
    {
        width += 8 - (width % 8);
    }
    int bw = width / 8;

    int8_t* bitmap = new int8_t[height * bw];
    memset(bitmap, 0, height * bw * sizeof(int8_t));

    CellIndex maxRow = iOffset + height;
    CellIndex maxColumn = jOffset + width;
    auto maxRowIter = mBoard.lower_bound(maxRow);
    for (auto iIter = mBoard.lower_bound(iOffset); iIter != maxRowIter; iIter++)
    {
        CellIndex i = iIter->first;
        int iCount = i - iOffset;
        assert((iCount >= 0) && (iCount < height));
        auto maxColumnIter = iIter->second.lower_bound(maxColumn);
        for (auto jIter = iIter->second.lower_bound(jOffset); jIter != maxColumnIter; jIter++)
        {
            CellIndex j = *jIter;
            int jCount = j - jOffset;
            if (getCell(i, j))
            {
                int8_t &c = bitmap[iCount * bw + (jCount / 8)];
                c |= 1 << (jCount % 8);
            }
        }
    }

    return bitmap;
}

bool SparseBoard::getFirstLiveCell(CellIndex& i, CellIndex& j) const
{
    for (auto iIter = mBoard.begin(); iIter != mBoard.end(); iIter++)
    {
        for (auto jIter = iIter->second.begin(); jIter != iIter->second.end(); jIter++)
        {
			i = iIter->first;
			j = *jIter;
            return true;
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
            i = iIter->first;
			j = *jIter;
            return true;
        }
        firstIteration = false;
    }
    return false;
}

void SparseBoard::clearBoard()
{
    mBoard.clear();
}