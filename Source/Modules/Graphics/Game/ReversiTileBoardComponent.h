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
#ifndef BGE_REVERSI_TILE_BOARD_COMPONENT_H
#define BGE_REVERSI_TILE_BOARD_COMPONENT_H

class ReversiTileBoardComponent : public TileBoardComponent
{
public:
    ReversiTileBoardComponent (BoardGame& boardGame,
                               SoundManager& soundManager,
                               bool showMoveHints,
                               bool showTileIndices);

    //==============================================================================
    /** @internal */
    TileComponent* createTileComponent (int index) override;

protected:
    //==============================================================================
    bool showMoveHints, showTileIndices;
    void stopShowingMoveHints();

private:
    //==============================================================================
    class ReversiTileComponent;

    //==============================================================================
    ReversiTileBoardComponent() BGE_DELETED_FUNCTION;
    BGE_DECLARE_NON_COPYABLE (ReversiTileBoardComponent)
};

#endif //BGE_REVERSI_TILE_BOARD_COMPONENT_H