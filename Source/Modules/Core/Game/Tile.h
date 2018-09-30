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
#ifndef BGE_TILE_H
#define BGE_TILE_H

/** A unique space on a tile board

    @see TileBoard
*/
struct Tile
{
    /** Default states a tile can have */
    enum State
    {
        blank       = 0,        //< A reserved value for having no states present at all in a tile
        playerOne   = 1 << 0,   //< A bit to represent that a tile contains the first player
        playerTwo   = 1 << 1    //< A bit to represent that a tile contains the second player
    };

    /** Constructor */
    Tile() noexcept {}

    /** Constructor

        @param[in] initialState The initial state of the tile
    */
    Tile (int initialState) noexcept :
        state (initialState)
    {
    }

    //==============================================================================
    /** Find out if this tile contains no player or other custom states at all */
    bool isBlank() const noexcept       { return state == blank; }

    /** Find out if this tile contains the first player */
    bool hasPlayerOne() const noexcept  { return hasBitSet (playerOne); }

    /** Find out if this tile contains the second player */
    bool hasPlayerTwo() const noexcept  { return hasBitSet (playerTwo); }

    /** Find out if this tile contains the first and/or second player */
    bool hasPlayer() const noexcept     { return hasPlayerOne() || hasPlayerTwo(); }

    //==============================================================================
    /** Find out if the state has the specified bit toggled */
    bool hasBitSet (int bitIndex) const noexcept { return (state & (1 << bitIndex)) != 0; }

    //==============================================================================
    int state = (int) blank; //< The state of this tile, comprised of bit flags.
};

#endif //BGE_TILE_H
