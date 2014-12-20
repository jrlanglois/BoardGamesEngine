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
#ifndef BGE_REVERSI_TILE_BOARD_H
#define BGE_REVERSI_TILE_BOARD_H

/** A base class for designing and setting up a tile board for Reversi.

    @see TileBoard, StandardReversiTileBoard, SmallReversiTileBoard, WideReversiTileBoard
*/
class ReversiTileBoard : public TileBoard
{
public:
    //==============================================================================
    /** Unique identification tags for the Reversi subclasses */
    enum UniqueId
    {
        reversiStandard = 43081707,
        reversiSmall    = 69040536,
        reversiWide     = 15228313
    };

    //==============================================================================
    /** @internal */
    std::string saveState() const override;
    /** @internal */
    bool loadFromState (const std::string& state) override;

protected:
    //==============================================================================
    /** Constructor

        @param[in] id           The unique identification tag to use for saving and loading states of your subclass
        @param[in] numColumns   The number of columns your subclass' edition of Reversi will have
        @param[in] numRows      The number of rows your subclass' edition of Reversi will have
    */
    ReversiTileBoard (int id, int numColumns, int numRows);

private:
    //==============================================================================
    ReversiTileBoard() BGE_DELETED_FUNCTION;
    BGE_DECLARE_NON_COPYABLE (ReversiTileBoard)
};

//==============================================================================
/** A board game representing the regular 8x8 edition of Reversi

    @see ReversiBoardGame
*/
class StandardReversiTileBoard : public ReversiTileBoard
{
public:
    /** Constructor */
    StandardReversiTileBoard();

    //==============================================================================
    /** @internal */
    void reset() override;

private:
    //==============================================================================
    BGE_DECLARE_NON_COPYABLE (StandardReversiTileBoard)
};

//==============================================================================
/** A tile board at half-size of the regular edition of Reversi.

    @see ReversiBoardGame
*/
class SmallReversiTileBoard : public ReversiTileBoard
{
public:
    /** Constructor */
    SmallReversiTileBoard();

    //==============================================================================
    /** @internal */
    void reset() override;

private:
    //==============================================================================
    BGE_DECLARE_NON_COPYABLE (SmallReversiTileBoard)
};

//==============================================================================
/** A tile board at double-width of the regular edition of Reversi.

    @see ReversiBoardGame
*/
class WideReversiTileBoard : public ReversiTileBoard
{
public:
    /** Constructor */
    WideReversiTileBoard();

    //==============================================================================
    /** @internal */
    void reset() override;

private:
    //==============================================================================
    BGE_DECLARE_NON_COPYABLE (WideReversiTileBoard)
};

#endif //BGE_REVERSI_TILE_BOARD_H