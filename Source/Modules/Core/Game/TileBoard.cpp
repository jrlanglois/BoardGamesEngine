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
TileBoard::TileBoard (const int id, const int c, const int r) :
    numRows (r),
    numColumns (c),
    uniqueId (id)
{
    rassert (uniqueId != 0); //That wouldn't be very unique!
    rassert (numColumns > 2 && numRows > 2); //This would make for a really poor game to play!
    initialise();
}

TileBoard::~TileBoard()
{
}

//==============================================================================
void TileBoard::initialise()
{
    tiles.clear();
    tiles.resize (getTotalNumTiles());
}

//==============================================================================
void TileBoard::setTileStateWithoutChecking (const int index, const int newState)
{
    rassert (isPositiveAndBelow (index, getTotalNumTiles()));
    tiles[index] = Tile (newState);
}

void TileBoard::setTileStateWithoutChecking (const int column, const int row, const int newState)
{
    return setTileStateWithoutChecking (getTileIndexAt (column, row), newState);
}

//==============================================================================
int TileBoard::getTileIndexAt (const int column, const int row) const noexcept
{
    return getIndexOfTile (column, row, numColumns);
}

Tile TileBoard::getTile (const int index) const
{
    rassert (isPositiveAndBelow (index, getTotalNumTiles()));
    return tiles[index];
}

Tile TileBoard::getTile (const int column, const int row) const
{
    return getTile (getTileIndexAt (column, row));
}

int TileBoard::getNumTilesOfCertainState (const int stateToCheckFor) const noexcept
{
    int count = 0;

    for (const auto& t : tiles)
        if (t.state == stateToCheckFor)
            ++count;

    return count;
}
