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
#ifndef BGE_CHECKERS_BOARD_GAME_H
#define BGE_CHECKERS_BOARD_GAME_H

#include "CheckersTileBoard.h"

/** A base-class for designing and controlling a type of CheckersTileBoard.

    @see CheckersTileBoard, TileBoard, BoardGame
*/
class CheckersBoardGame : public BoardGame
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

        @param[in] tileBoard A checkers based tile board to control
    */
    CheckersBoardGame (CheckersTileBoard* tileBoard);

private:
    //==============================================================================

    //==============================================================================
    CheckersBoardGame() BGE_DELETED_FUNCTION;
    BGE_DECLARE_NON_COPYABLE (CheckersBoardGame)
};

#endif //BGE_CHECKERS_BOARD_GAME_H
