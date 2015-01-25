#include "SparseBoard.h"
#include <assert.h>

using namespace std;

SparseBoard::SparseBoard() :
    Board()
{
    mBoard = BoardRep();
}

void SparseBoard::updateNeighborCount(CellIndex i, NeighborCount &nbrs) const
{
	auto iIter = mBoard.find(i);
	if (iIter != mBoard.end())
	{
		for (auto jIter = iIter->second.begin(); jIter != iIter->second.end(); jIter++)
		{
			CellIndex j = *jIter;
			nbrs[i - 1][j - 1]++;
			nbrs[i - 1][j]++;
			nbrs[i - 1][j + 1]++;
			nbrs[i][j - 1]++;
			nbrs[i][j + 1]++;
			nbrs[i + 1][j - 1]++;
			nbrs[i + 1][j]++;
			nbrs[i + 1][j + 1]++;
		}
	}
}

void SparseBoard::birthCells(CellIndex i, const NeighborCount& nbrs)
{
	auto iIter = nbrs.find(i);
	if (iIter != nbrs.end())
	{
		for (auto jIter = iIter->second.begin(); jIter != iIter->second.end(); jIter++)
		{
			if (jIter->second == NEIGHBOR_COUNT_BIRTH)
			{
				CellIndex j = jIter->first;
				mBoard[i].insert(j);
			}
		}
	}
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
	// The board is updated row by row. At all times the algorithm tracks the
	// prior, current, and next row from the last iteration of the board, and
	// then makes updates to the board in place.
	NeighborCount nbrs = NeighborCount();
    auto iIter = mBoard.begin();
	CellIndex highestRowCounted;
	bool firstRow = true;
	while (iIter != mBoard.end())
    {
		CellIndex i = iIter->first;
		auto iNextIter = iIter;
		iNextIter++;

		if (firstRow)
		{
			// Over or underflow with 64-bit integers could be a problem
			// if we don't want the lowest possible row to have neighbors
			// at the highest one. I guess this implementation will "wrap around".
			highestRowCounted = i - 2;
			firstRow = false;
		}

		// Process all rows up until just before the window starts (in row i - 1).
		// That is, birth new cells with the right neighbor count, and then clear
		// out rows of the neighbor count map that aren't needed anymore.
		CellIndex highestRowToProcess = i - 2;
		while (!nbrs.empty() && (nbrs.begin()->first <= highestRowToProcess))
		{
			auto iNbrIter = nbrs.begin();
			birthCells(iNbrIter->first, nbrs);
			nbrs.erase(iNbrIter);
		}
		
		// Update neighbor counts. Ensure that all rows up to the current row are
		// included in the count, in case there is a new row in the middle of
		// nowhere. Always add the next row into the count.
		if (highestRowCounted < i - 1)
		{
			updateNeighborCount(i - 1, nbrs);
		}
		if (highestRowCounted < i)
		{
			updateNeighborCount(i, nbrs);
		}
		updateNeighborCount(i + 1, nbrs);
		highestRowCounted = i + 1;

		// Update the current row based on the neighbor count for the same row.
		// First test for live cells in the current row that need to die.
		auto iNbrIter = nbrs.find(i);
		assert(iNbrIter != nbrs.end());
		auto jIter = iIter->second.begin();
		while (jIter != iIter->second.end())
		{
			auto jNextIter = jIter;
			jNextIter++;

			CellIndex j = *jIter;
			int liveNbrs = 0;
			auto jNbrIter = iNbrIter->second.find(j);
			if (jNbrIter != iNbrIter->second.end())
			{
				liveNbrs = jNbrIter->second;
			}
			if ((liveNbrs < NEIGHBOR_COUNT_MIN) || (liveNbrs > NEIGHBOR_COUNT_MAX))
			{
				iIter->second.erase(jIter);
			}

			jIter = jNextIter;
		}

		// Clear out row of board if it is now empty.
		if (iIter->second.empty())
		{
			mBoard.erase(iIter);
		}

		iIter = iNextIter;
    }

    // Process remaining neighbor counts.
	while (!nbrs.empty())
	{
		birthCells(nbrs.begin()->first, nbrs);
		nbrs.erase(nbrs.begin());
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
        int iCount = static_cast<int>(i - iOffset);
        assert((iCount >= 0) && (iCount < height));
        auto maxColumnIter = iIter->second.lower_bound(maxColumn);
        for (auto jIter = iIter->second.lower_bound(jOffset); jIter != maxColumnIter; jIter++)
        {
            CellIndex j = *jIter;
			int jCount = static_cast<int>(j - jOffset);
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