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
#ifndef BGE_PLAYER_VIEW_H
#define BGE_PLAYER_VIEW_H

/** A base-class for designing UI objects representing a BoardGame's player.

    @see BoardGame
*/
class PlayerView : public BoardGame::Listener
{
public:
    /** Constructor

        @param[in] boardGame        The board game to represent
        @param[in] showMoveHints    Initially show move hints or not
    */
    PlayerView (BoardGame& boardGame, bool forFirstPlayer);

    /** Destructor */
    ~PlayerView();

    //==============================================================================
    /** Find out if this player view represents the first or second player */
    bool representsFirstPlayer() const noexcept { return firstPlayer; }

    //==============================================================================
    /** Callback to notify that the score display needs to be updated */
    virtual void updateScore() = 0;

protected:
    //==============================================================================
    BoardGame& boardGame;
    const bool firstPlayer;

private:
    //==============================================================================
    PlayerView() BGE_DELETED_FUNCTION;
    BGE_DECLARE_NON_COPYABLE (PlayerView)
};

#endif //BGE_PLAYER_VIEW_H