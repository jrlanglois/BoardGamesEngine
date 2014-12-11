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
#ifndef BGE_TILE_BOARD_H
#define BGE_TILE_BOARD_H

#include "../Misc/Macros.h"
#include "Tile.h"

/** Implements the basic structure for designing strategy board games.
*/
class TileBoard
{
public:
    /** Constructor

        @param[in] uniqueId     The unique identification tag to represent your subclass.
        @param[in] numColumns   The number of columns your subclass' tileboard will be comprised.
        @param[in] numRows      The number of rows your subclass' tileboard will be comprised.
    */
    TileBoard (int uniqueId, int numColumns, int numRows);

    /** Destructor */
    virtual ~TileBoard();

    //==============================================================================
    /** Obtain the unique identification tag representing the type of tileboard.

        This is great for identifying the state your subclass can restore.
    */
    int getUniqueId() const noexcept { return uniqueId; }

    //==============================================================================
    /** Every subclass needs to setup the initial board state in this method.

        For example, in Reversi, you need to place 4 tokens in center of the board.

        @warning You are required to call initialise() in the implementation of this method.
        @see initialise()
    */
    virtual void reset() = 0;

    //==============================================================================
    /** Attempts directly changing the state of a tile at a given column and row
        without checking against the play book.

        @note The first tile starts at the top left of the board, with index 0, or column 0 & row 0.
    */
    virtual void setTileStateWithoutChecking (int column, int row, int newState);

    /** Attempts directly changing the state of a tile at a given index
        without checking against the rule book.

        @note The first tile starts at the top left of the board, with index 0, or column 0 & row 0.
    */
    virtual void setTileStateWithoutChecking (int index, int newState);

    //==============================================================================
    /** Get the tile at the specified index.

        If the index is out of range, a blank tile will be given.
    */
    Tile getTile (int index) const;

    /** Get the tile at the specified column and row.

        If the index is out of range, a blank tile will be given.
    */
    Tile getTile (int column, int row) const;

    /** Calculate the tile index at the given column and row. */
    int getTileIndexAt (int column, int row) const noexcept;

    /** Obtain the tile board's arrangement */
    const std::vector<Tile>& getTiles() const noexcept { return tiles; }

    //==============================================================================
    /** Get the number of columns this tile board contains */
    int getNumberOfColumns() const noexcept     { return numColumns; }

    /** Get the number of rows this tile board contains */
    int getNumberOfRows() const noexcept        { return numRows; }

    /** Get the */
    int getTotalNumTiles() const noexcept       { return numRows * numColumns; }

    /** Get the number of tiles that have the specified state. */
    int getNumTilesOfCertainState (int stateToCheckFor) const;

    //==============================================================================
    /** Serialise the state of your subclass' tile board into a std::string to
        help a front-end system save it.

        @warning The returned std::string must be valid (e.g.: non-empty),
                 and represent the full state of your subclass' tile board!

        @see loadFromState()
    */
    virtual std::string saveState() const = 0;

    /** Attempt restoring your subclass' state from the given std::string.

        @see loadFromState()
        @returns True if the operation was successful, false otherwise.
    */
    virtual bool loadFromState (const std::string& state) = 0;

protected:
    //==============================================================================
    std::vector<Tile> tiles;
    const int numRows;
    const int numColumns;
    const int uniqueId;

    //==============================================================================
    /** Call this in your subclass to setup an empty set of tiles.

        @see reset()
    */
    void initialise();

private:
    //==============================================================================
    TileBoard() BGE_DELETED_FUNCTION;
    BGE_DECLARE_NON_COPYABLE (TileBoard)
};

#endif //BGE_TILE_BOARD_H
