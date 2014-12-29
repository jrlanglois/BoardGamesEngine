/*
    ==============================================================================

    This file is part of the Board Games Engine library.
    Copyright (c) 2014 - Joel R. Langlois

    Permission is granted to use this software under the terms of GPL v2 (or any later version).

    Details of these licenses can be found at: www.gnu.org/licenses

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the GNU General Public License for more details.

    ------------------------------------------------------------------------------

    To release a closed-source product which uses the Board Games Engine,
    please contact Joel at joel.r.langlois@gmail.com for additional details.

    ==============================================================================
*/
//==============================================================================
StandardReversiBoardGame::StandardReversiBoardGame() : ReversiBoardGame (new StandardReversiTileBoard()) { }
SmallReversiBoardGame::SmallReversiBoardGame() : ReversiBoardGame (new SmallReversiTileBoard()) { }
WideReversiBoardGame::WideReversiBoardGame() : ReversiBoardGame (new WideReversiTileBoard()) { }

//==============================================================================
ReversiBoardGame::ReversiBoardGame (ReversiTileBoard* const tb) :
    BoardGame (tb)
{
}

//==============================================================================
int ReversiBoardGame::getScore (const bool forPlayerOne) const
{
    return tileBoard->getNumTilesOfCertainState (forPlayerOne ? Tile::playerOne : Tile::playerTwo);
}

//==============================================================================
class ReversiBoardGame::Comparator
{
public:
    Comparator() noexcept { }
    virtual ~Comparator() { }

    virtual bool isValid (int nextIndex) const noexcept = 0;

private:
    BGE_DECLARE_NON_COPYABLE (Comparator)
};

class ReversiBoardGame::SingleDirectionComparator : public ReversiBoardGame::Comparator
{
public:
    SingleDirectionComparator (int s, int d) noexcept :
        source (s),
        dest (d)
    {
    }

protected:
    const int source, dest;

private:
    SingleDirectionComparator() BGE_DELETED_FUNCTION;
    BGE_DECLARE_NON_COPYABLE (SingleDirectionComparator)
};

class ReversiBoardGame::SameColumnComparator : public SingleDirectionComparator
{
public:
    SameColumnComparator (int srcColumn, int columns) noexcept :
        SingleDirectionComparator (srcColumn, columns)
    {
    }

    bool isValid (int nextIndex) const noexcept override
    {
        return findColumnOfTile (nextIndex, dest) == source;
    }

private:
    SameColumnComparator() BGE_DELETED_FUNCTION;
    BGE_DECLARE_NON_COPYABLE (SameColumnComparator)
};

class ReversiBoardGame::SameRowComparator : public SingleDirectionComparator
{
public:
    SameRowComparator (int srcRow, int rows) noexcept :
        SingleDirectionComparator (srcRow, rows)
    {
    }

    bool isValid (int nextIndex) const noexcept override
    {
        return findRowOfTile (nextIndex, dest) == source;
    }

private:
    SameRowComparator() BGE_DELETED_FUNCTION;
    BGE_DECLARE_NON_COPYABLE (SameRowComparator)
};

class ReversiBoardGame::DiagonalComparator : public Comparator
{
public:
    DiagonalComparator (int sourceIndex_,
                        int numColumns,
                        int numRows) noexcept :
        sourceIndex (sourceIndex_),
        nc (numColumns),
        nr (numRows),
        lastIndex (sourceIndex_),
        incColumns (false),
        incRows (true)
    {
    }

    void resetSources (bool incrementingColumns,
                       bool incrementingRows)
    {
        lastIndex = sourceIndex;
        incColumns = incrementingColumns;
        incRows = incrementingRows;
    }

    bool isValid (int nextIndex) const noexcept override
    {
        return isValid (lastIndex, nextIndex, nc, incColumns, &findColumnOfTile)
            && isValid (lastIndex, nextIndex, nr, incRows, &findRowOfTile);
    }

    void tick (int step)
    {
        lastIndex += step;
    }

private:
    const int sourceIndex;
    const int nc, nr;
    int lastIndex;
    bool incColumns, incRows;

    typedef int (*findForTileFunction)(int, int);
    static bool isValid (int lastIndex, int nextIndex, int range, bool checkIfLarger, findForTileFunction findForTile) noexcept
    {
        rassert (findForTile != nullptr);
        const int last = findForTile (lastIndex, range);
        const int next = findForTile (nextIndex, range);

        if (last != next)
        {
            if (checkIfLarger)
                return next > last;

            return next < last;
        }

        return false;
    }

    DiagonalComparator() BGE_DELETED_FUNCTION;
    BGE_DECLARE_NON_COPYABLE (DiagonalComparator)
};

//==============================================================================
int ReversiBoardGame::findNextIndex (const int sourceIndex,
                                     const int stepAmount,
                                     const int totalNumTiles,
                                     const Comparator& comparator)
{
    const int nextIndex = sourceIndex + stepAmount;

    if (isPositiveAndBelow (nextIndex, totalNumTiles))
        if (comparator.isValid (nextIndex))
            return nextIndex;

    return -1;
}

void ReversiBoardGame::findMoveSequence (std::vector<int>& sequence,
                                         TileBoard& tb,
                                         const bool forFirstPlayer,
                                         const int sourceIndex,
                                         const int stepAmount,
                                         const int totalNumTiles,
                                         Comparator& comparator)
{
    Tile nextTile (tb.getTile (sourceIndex));
    if (! nextTile.isBlank())
        return; //This tile cannot be played on directly since it already contains a player

    int nextIndex = sourceIndex;
    int numTokens = 0;
    DiagonalComparator* const diagonalComparator = dynamic_cast<DiagonalComparator*> (&comparator);

    for (;;)
    {
        nextIndex = findNextIndex (nextIndex, stepAmount,
                                   totalNumTiles, comparator);

        if (! isPositiveAndBelow (nextIndex, totalNumTiles))
        {
            sequence.clear();
            break;
        }

        if (diagonalComparator != nullptr)
            diagonalComparator->tick (stepAmount);

        const Tile previousTile (nextTile);
        nextTile = tb.getTile (nextIndex);

        if (nextTile.isBlank())
        {
            if (previousTile.hasPlayer() && forFirstPlayer != previousTile.hasPlayerOne())
                sequence.clear();
            if (numTokens <= 0)
                sequence.clear();

            break;
        }

        if (forFirstPlayer != nextTile.hasPlayerOne())
        {
            ++numTokens;
            sequence.push_back (nextIndex);
        }
        else
        {
            break;
        }

        if (numTokens == 0)
        {
            sequence.clear();
            break;
        }
    }
}

void ReversiBoardGame::appendMoveSequence (std::vector<int>& destination,
                                           TileBoard& tb,
                                           const bool forFirstPlayer,
                                           const int sourceIndex,
                                           const int stepAmount,
                                           const int totalNumTiles,
                                           Comparator& comparator)
{
    std::vector<int> sequence;
    findMoveSequence (sequence, tb, forFirstPlayer, sourceIndex,
                      stepAmount, totalNumTiles, comparator);

    if (sequence.size() > 0)
        destination.insert (destination.end(), sequence.begin(), sequence.end());
}

//==============================================================================
void ReversiBoardGame::searchVerticalMoveSequences (std::vector<int>& sequence,
                                                    TileBoard& tb,
                                                    const bool forFirstPlayer,
                                                    const int index,
                                                    const int numColumns,
                                                    const int total)
{
    SameColumnComparator scc (findColumnOfTile (index, numColumns), numColumns);
    appendMoveSequence (sequence, tb, forFirstPlayer, index, -numColumns, total, scc);  //Upwards
    appendMoveSequence (sequence, tb, forFirstPlayer, index, numColumns, total, scc);   //Downwards
}

void ReversiBoardGame::searchHorizontalMoveSequences (std::vector<int>& sequence,
                                                      TileBoard& tb,
                                                      const bool forFirstPlayer,
                                                      const int index,
                                                      const int numRows,
                                                      const int total)
{
    SameRowComparator src (findRowOfTile (index, numRows), numRows);
    appendMoveSequence (sequence, tb, forFirstPlayer, index, -1, total, src);   //Leftwards
    appendMoveSequence (sequence, tb, forFirstPlayer, index, 1, total, src);    //Rightwards
}

void ReversiBoardGame::searchDiagonalMoveSequences (std::vector<int>& sequence,
                                                    TileBoard& tb,
                                                    const bool forFirstPlayer,
                                                    const int index,
                                                    const int numColumns,
                                                    const int numRows,
                                                    const int total)
{
    DiagonalComparator diag (index, numColumns, numRows);

    //Up & leftwards:
    diag.resetSources (false, false);
    appendMoveSequence (sequence, tb, forFirstPlayer, index, -numColumns - 1, total, diag);

    //Up & rightwards:
    diag.resetSources (true, false);
    appendMoveSequence (sequence, tb, forFirstPlayer, index, -numColumns + 1, total, diag);

    //Down & leftwards:
    diag.resetSources (false, true);
    appendMoveSequence (sequence, tb, forFirstPlayer, index, numColumns - 1, total, diag);

    //Down & rightwards:
    diag.resetSources (true, true);
    appendMoveSequence (sequence, tb, forFirstPlayer, index, numColumns + 1, total, diag);
}

void ReversiBoardGame::generateMoveSequence (std::vector<int>& sequence,
                                             TileBoard& tb,
                                             const bool forFirstPlayer,
                                             const int index)
{
    const int total         = tb.getTotalNumTiles();
    const int numColumns    = tb.getNumberOfColumns();
    const int numRows       = tb.getNumberOfRows();

    //Search for all possible sequences of tiles, in all directions, that a player can overtake:
    searchVerticalMoveSequences (sequence, tb, forFirstPlayer, index, numColumns, total);
    searchHorizontalMoveSequences (sequence, tb, forFirstPlayer, index, numRows, total);
    searchDiagonalMoveSequences (sequence, tb, forFirstPlayer, index, numColumns, numRows, total);

    //Sort the sequence, and remove duplicates:
    std::sort (sequence.begin(), sequence.end());
    sequence.erase (std::unique (sequence.begin(), sequence.end()), sequence.end());
}

//==============================================================================
bool ReversiBoardGame::isPossibleMove (const bool fp, const int index) const
{
    if (! tileBoard->getTile (index).isBlank())
        return false; //A tile that isn't blank already means that it cannot be directly changed

    std::vector<int> sequence;
    generateMoveSequence (sequence, *tileBoard, fp, index);
    return sequence.size() > 0;
}

void ReversiBoardGame::tileChanged (const int index, int state)
{
    std::vector<int> sequence;
    generateMoveSequence (sequence, *tileBoard, isFirstPlayerTurn(), index);

    //Perform all the token flips in the sequence, if there are any tokens to flip:
    state = getCurrentPlayerAsTileState();

    for (int i = (int) sequence.size(); --i >= 0;)
        tileBoard->setTileStateWithoutChecking (sequence[i], state);
}