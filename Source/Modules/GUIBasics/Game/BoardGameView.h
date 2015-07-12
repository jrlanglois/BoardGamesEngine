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
#ifndef BGE_BOARD_GAME_VIEW_H
#define BGE_BOARD_GAME_VIEW_H

#include "PlayerView.h"

/** A base-class for designing UI objects representing a BoardGame.

    @see PlayerView
*/
class BoardGameView : public BoardGame::Listener
{
public:
    /** Constructor

        @param[in] boardGame        The board game to represent
        @param[in] showMoveHints    Initially show move hints or not
        @param[in] showTileIndices  Initially show tile indices or not
    */
    BoardGameView (BoardGame& boardGame,
                   bool showMoveHints,
                   bool showTileIndices) noexcept;

    /** Destructor */
    ~BoardGameView();

    //==============================================================================
    /** Get the tile board */
    TileBoard& getTileBoard() const noexcept { return boardGame.getTileBoard(); }

    //==============================================================================
    /** You can optionally implement this method to perform additional steps
        required to setup your subclass' BoardGameView.

        @warning If you do override this method, you must call this version of initialise()!
    */
    virtual void initialise();

    /** You can optionally implement this method to perform additional steps
        required to reset your BoardGameView.

        @warning If you do override this method, you must call this version of reset()!
    */
    virtual void reset();

    //==============================================================================
    /** Enables or disables showing move hints.

        Move hints are potential places the player can do a move onto.
    */
    void setShowingMoveHints (bool shouldShowHints);

    /** Enables or disables showing tile indices.

        Tile indices are the positions at which each tile is located, in index form.
    */
    void setShowingTileIndices (bool shouldShowTileIndices);

    /** Find out if move hints are showing or not. */
    bool isShowingMoveHints() const noexcept    { return displayMoveHints; }

    /** Find out if tile indices are showing or not. */
    bool isShowingTileIndices() const noexcept  { return displayTileIndices; }

protected:
    //==============================================================================
    BoardGame& boardGame;

    //==============================================================================
    /** Creates a view to represent the specified player.

        @param[in] forFirstPlayer Lets your implementation know which player
                                  the PlayerView will represent.

        @result Must not be null!
    */
    virtual PlayerView* createPlayerView (bool forFirstPlayer) = 0;

    /** Get the subclass' customised player view for the specified player. */
    PlayerView* getPlayerView (bool forFirstPlayer) const noexcept;

    //==============================================================================
    /** This is internally called to update the hints, if they are even showing. */
    virtual void updateMoveHints() = 0;

    /** This is internally called to stop showing hints, if they are even showing. */
    virtual void hideMoveHints() = 0;

    //==============================================================================
    /** This is internally called to display tile indices if they weren't previously showing. */
    virtual void showTileIndices() = 0;

    /** This is internally called to stop showing tile indices, if they are even showing. */
    virtual void hideTileIndices() = 0;

private:
    //==============================================================================
    bool displayMoveHints, displayTileIndices;
    std::unique_ptr<PlayerView> firstPlayerView, secondPlayerFirst;

    //==============================================================================
    BoardGameView() BGE_DELETED_FUNCTION;
    BGE_DECLARE_NON_COPYABLE (BoardGameView)
};

#endif //BGE_BOARD_GAME_VIEW_H