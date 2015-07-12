/*
    ==============================================================================

    This file is part of the Board Games Engine library.
    Copyright (c) 2014 to present - Joel R. Langlois

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
#ifndef BGE_CHECKERS_TILE_BOARD_H
#define BGE_CHECKERS_TILE_BOARD_H

/** A base class for designing and setting up a tile board for Reversi.

    @see TileBoard, StandardReversiTileBoard, SmallReversiTileBoard, WideReversiTileBoard
*/
class CheckersTileBoard : public TileBoard
{
public:
    //==============================================================================
    /** Constructor

        @param[in] id           The unique identification tag to use for saving and loading states of your subclass
        @param[in] numColumns   The number of columns your subclass' edition of Reversi will have
        @param[in] numRows      The number of rows your subclass' edition of Reversi will have
    */
    CheckersTileBoard();

    //==============================================================================
    /** Extra tile states */
    enum ExtraTileState
    {
        kinged = 1 << 2
    };

    //==============================================================================
    /** Unique identification tags for the Checkers subclasses */
    enum UniqueId
    {
        checkersStandard = 38956466
    };

    //==============================================================================
    /** @internal */
    std::string saveState() const override;
    /** @internal */
    bool loadFromState (const std::string& state) override;

private:
    //==============================================================================
    BGE_DECLARE_NON_COPYABLE (CheckersTileBoard)
};

#endif //BGE_CHECKERS_TILE_BOARD_H
