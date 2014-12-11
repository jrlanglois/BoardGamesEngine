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
#ifndef BGE_REVERSI_BOARD_GAME_H
#define BGE_REVERSI_BOARD_GAME_H

#include "ReversiTileBoard.h"

/** A base-class for designing and controlling a type of ReversiTileBoard.

    @see ReversiTileBoard, TileBoard, BoardGame
*/
class ReversiBoardGame : public BoardGame
{
public:
    //==============================================================================
    /** @internal */
    void tileChanged (int tileIndex, int newState) override;
    /** @internal */
    int getScore (bool forPlayerOne) const override;
    /** @internal */
    bool isPossibleMove (bool forFirstPlayer,
                         int sourceTileIndex) const override;

protected:
    //==============================================================================
    /** Constructor

        @param[in] tileBoard The Reversi-based tile board to control
    */
    ReversiBoardGame (ReversiTileBoard* tileBoard);

private:
    //==============================================================================
    /** Base class for bounds checking */
    class Comparator;
    /** Subclass that checks if an index given is in the specified column */
    class ColumnComparator;
    /** Subclass that checks if an index given is in the specified row */
    class RowComparator;

    //==============================================================================
    int findNextIndex (int sourceIndex,
                       int stepAmount,
                       int totalNumTiles,
                       const Comparator* comparator = nullptr) const;

    bool isPossibleMove (bool forFirstPlayer,
                         int sourceIndex,
                         int stepAmount,
                         int totalNumTiles,
                         const Comparator* comparator = nullptr) const;

    //==============================================================================
    void findMoveSequence (std::vector<int>& sequence,
                           bool forFirstPlayer,
                           int sourceIndex,
                           int stepAmount,
                           int totalNumTiles,
                           const Comparator* comparator = nullptr) const;

    void appendMoveSequence (std::vector<int>& sequence,
                             bool forFirstPlayer,
                             int sourceIndex,
                             int stepAmount,
                             int totalNumTiles,
                             const Comparator* comparator = nullptr) const;

    //==============================================================================
    ReversiBoardGame() BGE_DELETED_FUNCTION;
    BGE_DECLARE_NON_COPYABLE (ReversiBoardGame)
};

//==============================================================================
/** A board game representing the regular, 8x8, edition of Reversi.

    @see ReversiBoardGame, StandardReversiTileBoard
*/
class StandardReversiBoardGame : public ReversiBoardGame
{
public:
    /** Constructor */
    StandardReversiBoardGame();

private:
    //==============================================================================
    BGE_DECLARE_NON_COPYABLE (StandardReversiBoardGame)
};

//==============================================================================
/** A board game representing a half-size edition of Reversi, making it 4x4.

    @see ReversiBoardGame, SmallReversiTileBoard
*/
class SmallReversiBoardGame : public ReversiBoardGame
{
public:
    /** Constructor */
    SmallReversiBoardGame();

private:
    //==============================================================================
    BGE_DECLARE_NON_COPYABLE (SmallReversiBoardGame)
};

//==============================================================================
/** A board game representing a double-width edition of Reversi, making it 16x8.

    @see ReversiBoardGame, WideReversiTileBoard
*/
class WideReversiBoardGame : public ReversiBoardGame
{
public:
    /** Constructor */
    WideReversiBoardGame();

private:
    //==============================================================================
    BGE_DECLARE_NON_COPYABLE (WideReversiBoardGame)
};

#endif //BGE_REVERSI_BOARD_GAME_H
