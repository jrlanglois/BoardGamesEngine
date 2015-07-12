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
BoardGame::BoardGame (TileBoard* const tb) :
    tileBoard (tb),
    firstPlayerTurn (true),
    isProcessingTurn (true)
{
    rassert (tileBoard != nullptr);
    tileBoard->reset();
}

BoardGame::~BoardGame()
{
}

//==============================================================================
void BoardGame::addListener (Listener* const newListener)
{
    if (newListener != nullptr)
    {
        if (! std::binary_search (listeners.begin(), listeners.end(), newListener))
            listeners.push_back (newListener);
    }
    else
    {
        rassertfalse;
    }
}

void BoardGame::removeListener (Listener* const registeredListener)
{
    if (registeredListener != nullptr)
        listeners.erase (std::remove (listeners.begin(), listeners.end(), registeredListener), listeners.end());
    else
        rassertfalse;
}

//==============================================================================
bool BoardGame::beginTurn()
{
    rassert (! isProcessingTurn); //You're trying to start a new turn without ending the last one!
    isProcessingTurn = ! isGameOver();

    return isProcessingTurn;
}

void BoardGame::endTurn()
{
    if (isProcessingTurn)
    {
        firstPlayerTurn = ! firstPlayerTurn;

        //Check to see if there are possible moves that can be done this turn, for the next player:
        std::vector<int> playerMoves;
        getAllPossibleMoves (playerMoves, firstPlayerTurn);

        if (playerMoves.size() <= 0)
            firstPlayerTurn = ! firstPlayerTurn; //Since there aren't any moves, go back to the previous player

        for (int i = (int) listeners.size(); --i >= 0;)
            listeners[i]->playerChanged (this, firstPlayerTurn);
    }

    isProcessingTurn = false;

    if (isGameOver())
        for (int i = (int) listeners.size(); --i >= 0;)
            listeners[i]->gameEnded (this);
}

void BoardGame::reset()
{
    rassert (tileBoard != nullptr);

    tileBoard->reset();
    firstPlayerTurn = true;
}

bool BoardGame::isGameOver()
{
    std::vector<int> playerOneMoves, playerTwoMoves;
    getAllPossibleMoves (playerOneMoves, true);
    getAllPossibleMoves (playerTwoMoves, false);

    return playerOneMoves.size() <= 0
           && playerTwoMoves.size() <= 0;
}

//==============================================================================
bool BoardGame::setTileState (const int index, const int newState)
{
    bool wasSuccessful = false;

    if (isProcessingTurn
        && isPositiveAndBelow (index, tileBoard->getTotalNumTiles())
        && isPossibleMove (firstPlayerTurn, index))
    {
        tileChanged (index, newState); //Do this before in case the move checking algorithm relies on the previous board state
        tileBoard->setTileStateWithoutChecking (index, newState);
        wasSuccessful = true;
    }

    for (int i = (int) listeners.size(); --i >= 0;)
        listeners[i]->attemptedTileStateChange (this, index, wasSuccessful);

    return wasSuccessful;
}

bool BoardGame::setTileState (const int column, const int row, const int newState)
{
    return setTileState (tileBoard->getTileIndexAt (column, row), newState);
}

bool BoardGame::setTileStateToCurrentPlayer (const int column, const int row)
{
    return setTileState (column, row, getCurrentPlayerAsTileState());
}

bool BoardGame::setTileStateToCurrentPlayer (const int index)
{
    return setTileState (index, getCurrentPlayerAsTileState());
}

//==============================================================================
void BoardGame::getAllPossibleMoves (std::vector<int>& moves,
                                     const bool forFirstPlayer)
{
    const int totalNumTiles = tileBoard->getTotalNumTiles();

    for (int i = 0; i < totalNumTiles; ++i)
        if (isPossibleMove (forFirstPlayer, i))
            moves.push_back (i);
}

void BoardGame::getAllPossibleMoves (std::vector<int>& moves)
{
    getAllPossibleMoves (moves, firstPlayerTurn);
}