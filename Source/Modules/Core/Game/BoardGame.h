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
#ifndef BGE_BOARD_GAME_H
#define BGE_BOARD_GAME_H

#include "TileBoard.h"

/** A controller of a given tile board, logically letting your play a game.

    @see ReversiTileBoard, TileBoard
*/
class BoardGame
{
public:
    /** Constructor

        @param[in] tileBoard The TileBoard to control. Note that the given TileBoard will be owned by this BoardGame!
    */
    BoardGame (TileBoard* tileBoard);

    /** Destructor */
    virtual ~BoardGame();

    //==============================================================================
    /** Get the current score of your designed game. */
    virtual int getScore (bool forPlayerOne) const = 0;

    /** Find out what all possible moves are possible for the current player.

        @param[out] possibleTileIndices The resulting tile indexes that the player can potentially do a move on.
    */
    void getAllPossibleMoves (std::vector<int>& possibleTileIndicesToPlayOn);

    //==============================================================================
    /** Find out if it's the first player's turn */
    bool isFirstPlayerTurn() const { return firstPlayerTurn; }

    /** Obtain the player whose turn it is as a Tile::State */
    int getCurrentPlayerAsTileState() const { return firstPlayerTurn ? Tile::playerOne : Tile::playerTwo; }

    /** Returns the actively used tile board */
    TileBoard& getTileBoard() const { return *tileBoard; }

    //==============================================================================
    /** Attempt starting a turn for the next player.

        @returns True if the new turn has begun, false if it cannot begin.
                 If false, it's possible the game is over.

        @see isGameOver()
    */
    bool beginTurn();

    /** Attempt ending the current turn. If there are no turns left to be had,
        this will do nothing.
    */
    void endTurn();

    /** Puts the game back to its initial state.

        @warning Since this method also handles resetting the turn processing,
                 prefer calling this method over calling TileBoard::reset()!
    */
    void reset();

    /** Find out if the game is over.

        The default implementation of this method simply checks if all of the
        moves possible are exhausted for each player.

        Your subclass may override this method if it requires a better set of conditions.

        @returns True if the game is over, false otherwise.
    */
    virtual bool isGameOver();

    //==============================================================================
    /** Attempt changing the state of a tile situated at a given column and row.

        @note The first tile starts at the top left of the board, with index 0, or column 0 & row 0.

        @returns True if the operation was successful.
                 Otherwise, the game is over or the index to check is not a valid move.
    */
    bool setTileState (int column, int row, int newState);

    /** Attempt changing the state of a tile at a given index.

        @note The first tile starts at the top left of the board, with index 0, or column 0 & row 0.

        @returns True if the operation was successful.
                 Otherwise, the game is over or the index to check is not a valid move.
    */
    bool setTileState (int index, int newState);

    //==============================================================================
    /** Attempt changing the state of a tile situated at a given column and row.

        @note The first tile starts at the top left of the board, with index 0, or column 0 & row 0.

        @returns True if the operation was successful.
                 Otherwise, the game is over or the index to check is not a valid move.
    */
    bool setTileStateToCurrentPlayer (int column, int row);

    /** Attempt changing the state of a tile at a given index.

        @note The first tile starts at the top left of the board, with index 0, or column 0 & row 0.

        @returns True if the operation was successful.
                 Otherwise, the game is over or the index to check is not a valid move.
    */
    bool setTileStateToCurrentPlayer (int index);

    //==============================================================================
    /** A handy RAII approach to handling the taking of a turn for a player */
    class ScopedTurnTaker
    {
    public:
        /** Constructor */
        ScopedTurnTaker (BoardGame& bg) : boardGame (bg) { boardGame.beginTurn(); }

        /** Destructor */
        ~ScopedTurnTaker() { boardGame.endTurn(); }

    private:
        BoardGame& boardGame;

        //==============================================================================
        ScopedTurnTaker() BGE_DELETED_FUNCTION;
        BGE_DECLARE_NON_COPYABLE (ScopedTurnTaker)
    };

    //==============================================================================
    /** A handy RAII approach to handling the ending and starting of a turn for a player */
    class ScopedReverseTurnTaker
    {
    public:
        /** Constructor */
        ScopedReverseTurnTaker (BoardGame& bg) : boardGame (bg) { boardGame.endTurn(); }

        /** Destructor */
        ~ScopedReverseTurnTaker() { boardGame.beginTurn(); }

    private:
        BoardGame& boardGame;

        //==============================================================================
        ScopedReverseTurnTaker() BGE_DELETED_FUNCTION;
        BGE_DECLARE_NON_COPYABLE (ScopedReverseTurnTaker)
    };

    //==============================================================================
    class Listener
    {
    public:
        virtual ~Listener() { }

        /** A notification for letting subclasses know that an attempt
            has been made at changing a tile's state.

            @param[in] index                    The index of the tile
            @param[in] wasSuccessfullyChanged   Lets your subclass know if the attempt
                                                at changing the tile to a new state was successful
        */
        virtual void attemptedTileStateChange (int index, bool wasSuccessfullyChanged) = 0;

        /** A notification for letting subclasses know the player has changed.

            @param[in] toFirstPlayer Lets your subclass know if the
                                     next player's turn is the first player's.
        */
        virtual void playerChanged (bool /*toFirstPlayer*/) { }
    };

    /** Adds a listener that will receive various callbacks when something changes.

        @see addListener
    */
    void addListener (Listener* newListener);

    /** Removes a listener that was previously added with addListener().

        @see addListener
    */
    void removeListener (Listener* registeredListener);

protected:
    //==============================================================================
    std::unique_ptr<TileBoard> tileBoard;
    std::vector<Listener*> listeners;

    //==============================================================================
    /** You need to implement this in your subclass to find out what moves
        are possible for the given player and desired tile index.

        @param[in] forFirstPlayer   The player who's turn it is to make a move.
        @param[in] sourceTileIndex  The tile index as source. Not all board games will need this,
                                    so the parameter may be ignored in your subclass.

        @returns True if the given index can be a potential move for the current player
    */
    virtual bool isPossibleMove (bool forFirstPlayer,
                                 int sourceTileIndex) const = 0;

    /** Find out what all possible moves are possible for the current player.

        @param[out] possibleTileIndices The resulting tile indexes that the player can potentially do a move on.
        @param[in] forFirstPlayer       The player whose moves to look for.
    */
    void getAllPossibleMoves (std::vector<int>& possibleTileIndicesToPlayOn,
                              bool forFirstPlayer);

    /** A callback to notify your subclass when a tile has been changed successfully.

        @param[in] tileIndex    The index of the tile that was successfully changed.
        @param[in] newState     The new state of the tile at the specified index.

        @see setTileStateToCurrentPlayer
    */
    virtual void tileChanged (int tileIndex, int newState) = 0;

private:
    //==============================================================================
    bool firstPlayerTurn;
    bool isProcessingTurn;

    //==============================================================================
    BoardGame() BGE_DELETED_FUNCTION;
    BGE_DECLARE_NON_COPYABLE (BoardGame)
};

#endif //BGE_BOARD_GAME_H