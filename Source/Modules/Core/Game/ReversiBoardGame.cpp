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
ReversiBoardGame::ReversiBoardGame (ReversiTileBoard* const tileBoard) :
    BoardGame (tileBoard)
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
    Comparator() { }
    virtual ~Comparator() { }

    virtual bool isValid (int nextIndex) const noexcept = 0;

private:
    BGE_DECLARE_NON_COPYABLE (Comparator)
};

class ReversiBoardGame::ColumnComparator : public Comparator
{
public:
    ColumnComparator (int srcColumn, int columns) :
        sourceColumn (srcColumn),
        numColumns (columns)
    {
    }

    bool isValid (int nextIndex) const noexcept override
    {
        return findColumnOfTile (nextIndex, numColumns) == sourceColumn;
    }

private:
    const int sourceColumn;
    const int numColumns;

    BGE_DECLARE_NON_COPYABLE (ColumnComparator)
};

class ReversiBoardGame::RowComparator : public Comparator
{
public:
    RowComparator (int srcRow, int rows) noexcept :
        sourceRow (srcRow),
        numRows (rows)
    {
    }

    bool isValid (int nextIndex) const noexcept override
    {
        return findRowOfTile (nextIndex, numRows) == sourceRow;
    }

private:
    const int sourceRow;
    const int numRows;

    BGE_DECLARE_NON_COPYABLE (RowComparator)
};

//==============================================================================
int ReversiBoardGame::findNextIndex (const int sourceIndex,
                                     const int stepAmount,
                                     const int totalNumTiles,
                                     const Comparator* const comparator) const
{
    const int nextIndex = sourceIndex + stepAmount;

    if (isPositiveAndBelow (nextIndex, totalNumTiles))
    {
        if (comparator != nullptr)
        {
            if (comparator->isValid (nextIndex))
                return nextIndex;
        }
        else
        {
            return nextIndex;
        }
    }

    return -1;
}

void ReversiBoardGame::findMoveSequence (std::vector<int>& sequence,
                                         const bool forFirstPlayer,
                                         const int sourceIndex,
                                         const int stepAmount,
                                         const int totalNumTiles,
                                         const Comparator* const comparator) const
{
    Tile nextTile (tileBoard->getTile (sourceIndex));
    if (! nextTile.isBlank())
        return; //This tile cannot be played on directly since it already contains a player

    int nextIndex = sourceIndex;
    int numTokens = 0;

    for (;;)
    {
        nextIndex = findNextIndex (nextIndex, stepAmount,
                                   totalNumTiles, comparator);

        if (! isPositiveAndBelow (nextIndex, totalNumTiles))
        {
            sequence.clear();
            break;
        }

        const Tile previousTile (nextTile);
        nextTile = tileBoard->getTile (nextIndex);

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
                                           const bool forFirstPlayer,
                                           const int sourceIndex,
                                           const int stepAmount,
                                           const int totalNumTiles,
                                           const Comparator* const comparator) const
{
    std::vector<int> sequence;
    findMoveSequence (sequence, forFirstPlayer, sourceIndex,
                      stepAmount, totalNumTiles, comparator);

    if (sequence.size() > 0)
        destination.insert (destination.end(), sequence.begin(), sequence.end());
}

//==============================================================================
bool ReversiBoardGame::isPossibleMove (const bool forFirstPlayer,
                                       const int sourceIndex,
                                       const int stepAmount,
                                       const int totalNumTiles,
                                       const Comparator* const comparator) const
{
    std::vector<int> sequence;
    findMoveSequence (sequence, forFirstPlayer, sourceIndex,
                      stepAmount, totalNumTiles, comparator);
    return sequence.size() > 0;
}

bool ReversiBoardGame::isPossibleMove (const bool forFirstPlayer,
                                       const int index) const
{
    if (! tileBoard->getTile (index).isBlank())
        return false; //A tile that isn't blank already means that it cannot be directly changed

    const int total         = tileBoard->getTotalNumTiles();
    const int numColumns    = tileBoard->getNumberOfColumns();
    const int numRows       = tileBoard->getNumberOfRows();

    const ColumnComparator cc (findColumnOfTile (index, numColumns), numColumns);
    const RowComparator rc (findRowOfTile (index, numRows), numRows);

    //Search for all possible sequences of tiles a player can overtake in all directions:
    return isPossibleMove (forFirstPlayer, index, -numColumns, total, &cc)      //Upwards
        || isPossibleMove (forFirstPlayer, index, numColumns, total, &cc)       //Downwards
        || isPossibleMove (forFirstPlayer, index, -1, total, &rc)               //Leftwards
        || isPossibleMove (forFirstPlayer, index, 1, total, &rc)                //Rightwards
        || isPossibleMove (forFirstPlayer, index, -(numColumns + 1), total)     //Up & leftwards
        || isPossibleMove (forFirstPlayer, index, -(numColumns - 1), total)     //Up & rightwards
        || isPossibleMove (forFirstPlayer, index, numColumns - 1, total)        //Down & leftwards
        || isPossibleMove (forFirstPlayer, index, numColumns + 1, total);       //Down & rightwards
}

//==============================================================================
void ReversiBoardGame::tileChanged (const int index, int state)
{
    const int total             = tileBoard->getTotalNumTiles();
    const int numColumns        = tileBoard->getNumberOfColumns();
    const int numRows           = tileBoard->getNumberOfRows();
    const bool forFirstPlayer   = isFirstPlayerTurn();

    const ColumnComparator cc (findColumnOfTile (index, numColumns), numColumns);
    const RowComparator rc (findRowOfTile (index, numRows), numRows);
    std::vector<int> sequence;

    //Find the sequence of tiles to flip:
    appendMoveSequence (sequence, forFirstPlayer, index, -numColumns, total, &cc);
    appendMoveSequence (sequence, forFirstPlayer, index, numColumns, total, &cc);
    appendMoveSequence (sequence, forFirstPlayer, index, -1, total, &rc);
    appendMoveSequence (sequence, forFirstPlayer, index, 1, total, &rc);
    appendMoveSequence (sequence, forFirstPlayer, index, -(numColumns + 1), total);
    appendMoveSequence (sequence, forFirstPlayer, index, -(numColumns - 1), total);
    appendMoveSequence (sequence, forFirstPlayer, index, numColumns - 1, total);
    appendMoveSequence (sequence, forFirstPlayer, index, numColumns + 1, total);

    //Sort the sequence, and remove duplicates:
    std::sort (sequence.begin(), sequence.end());
    sequence.erase (std::unique (sequence.begin(), sequence.end()), sequence.end());

    //Flip the sequence:
    state = getCurrentPlayerAsTileState();

    for (int i = sequence.size(); --i >= 0;)
        tileBoard->setTileStateWithoutChecking (sequence[i], state);
}