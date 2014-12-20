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
BoardGameView::BoardGameView (BoardGame& bg,
                              const bool showMoveHints,
                              const bool showTileIndices) noexcept :
    boardGame (bg),
    displayMoveHints (showMoveHints),
    displayTileIndices (showTileIndices)
{
}

BoardGameView::~BoardGameView()
{
}

//==============================================================================
PlayerView* BoardGameView::getPlayerView (const bool forFirstPlayer)
{
    return forFirstPlayer ? firstPlayerView.get()
                          : secondPlayerFirst.get();
}

//==============================================================================
void BoardGameView::initialise()
{
    firstPlayerView.reset (createPlayerView (true));
    secondPlayerFirst.reset (createPlayerView (false));
    rassert (firstPlayerView != secondPlayerFirst);
}

void BoardGameView::reset()
{
    boardGame.reset();

    if (PlayerView* const pv = getPlayerView (true))    pv->updateScore();
    if (PlayerView* const pv = getPlayerView (false))   pv->updateScore();
}

//==============================================================================
void BoardGameView::setShowingMoveHints (const bool shouldShowHints)
{
    if (displayMoveHints != shouldShowHints)
    {
        displayMoveHints = shouldShowHints;

        if (displayMoveHints)
            updateMoveHints();
        else
            hideMoveHints();
    }
}

void BoardGameView::setShowingTileIndices (const bool shouldShowTileIndices)
{
    if (displayTileIndices != shouldShowTileIndices)
    {
        displayTileIndices = shouldShowTileIndices;

        if (displayTileIndices)
            showTileIndices();
        else
            hideTileIndices();
    }
}