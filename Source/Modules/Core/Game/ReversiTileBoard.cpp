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
ReversiTileBoard::ReversiTileBoard (const int id,
                                    const int numColumns,
                                    const int numRows) :
    TileBoard (id, numColumns, numRows)
{
}

//==============================================================================
std::string ReversiTileBoard::saveState() const
{
    std::string state;

    append (state, getUniqueId());
    state += " ";

    const int numTiles = getTotalNumTiles();
    append (state, numTiles);
    state += " ";

    {
        const std::vector<Tile>& t = getTiles();
        std::ostringstream s;

        for (int i = 0; i < numTiles; ++i) 
            s << (int) t[i].state;

        state.append (s.str());
    }

    return state;
}

#if BGE_MSVC
 #pragma warning (push)
 #pragma warning (disable: 4239)
#endif

bool ReversiTileBoard::loadFromState (const std::string& state)
{
    const size_t firstSpace = state.find_first_of (" ");
    const size_t lastSpace = state.find_last_of (" ");

    if (firstSpace == std::string::npos
        || lastSpace == std::string::npos)
        return false;

    const int resultId = toValue<int> (state.substr (0, firstSpace));
    if (resultId != getUniqueId())
        return false;

    const int totalNumTiles = toValue<int> (state.substr (firstSpace, lastSpace));
    if (totalNumTiles <= 0)
        return false;

    for (int i = 0; i < totalNumTiles; ++i)
        setTileStateWithoutChecking (i, toValue<int> (state[i + lastSpace + 1]));

    return true;
}

#if BGE_MSVC
 #pragma warning (pop)
#endif

//==============================================================================
StandardReversiTileBoard::StandardReversiTileBoard() :
    ReversiTileBoard (reversiStandard, 8, 8)
{
}

void StandardReversiTileBoard::reset()
{
    initialise();
    setTileStateWithoutChecking (27, Tile::playerOne);
    setTileStateWithoutChecking (28, Tile::playerTwo);
    setTileStateWithoutChecking (35, Tile::playerTwo);
    setTileStateWithoutChecking (36, Tile::playerOne);
}

//==============================================================================
SmallReversiTileBoard::SmallReversiTileBoard() :
    ReversiTileBoard (reversiSmall, 4, 4)
{
}

void SmallReversiTileBoard::reset()
{
    initialise();
    setTileStateWithoutChecking (5, Tile::playerOne);
    setTileStateWithoutChecking (6, Tile::playerTwo);
    setTileStateWithoutChecking (9, Tile::playerTwo);
    setTileStateWithoutChecking (10, Tile::playerOne);
}

//==============================================================================
WideReversiTileBoard::WideReversiTileBoard() :
    ReversiTileBoard (reversiWide, 16, 8)
{
}

void WideReversiTileBoard::reset()
{
    initialise();
    setTileStateWithoutChecking (55, Tile::playerOne);
    setTileStateWithoutChecking (56, Tile::playerTwo);
    setTileStateWithoutChecking (71, Tile::playerTwo);
    setTileStateWithoutChecking (72, Tile::playerOne);
}