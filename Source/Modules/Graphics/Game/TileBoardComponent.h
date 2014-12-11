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
#ifndef BGE_TILE_BOARD_COMPONENT_H
#define BGE_TILE_BOARD_COMPONENT_H

#include "../Misc/LocalisationManager.h"
#include "../Misc/SoundManager.h"

/** Base-class for designing a component that represents a board of tiles

*/
class TileBoardComponent : public juce::Component,
                           public juce::Button::Listener
{
public:
    /** Constructor

        @param[in] boardGame        A board game whose tile board will be displayed.
        @param[in] soundManager     A sound manager to play sound effects when needed.
        @param[in] showMoveHints    Show the possible places the current player can do a move onto.
        @param[in] showTileIndices  Show the indices of every tile.
    */
    TileBoardComponent (BoardGame& boardGame,
                        SoundManager& soundManager,
                        bool showMoveHints,
                        bool showTileIndices);

    //==============================================================================
    void setShowingMoveHints (bool shouldShowHints);
    void setShowingTileIndices (bool shouldShowTileIndices);

    //==============================================================================
    bool isShowingMoveHints() const noexcept { return showingMoveHints; }
    bool isShowingTileIndices() const noexcept { return showingTileIndices; }

    //==============================================================================
    /** @internal */
    void resized() override;
    /** @internal */
    void paint (juce::Graphics& g) override;
    /** @internal */
    void buttonClicked (juce::Button* button) override;

    //==============================================================================
    class TileComponent : public juce::Button
    {
    public:
        TileComponent (BoardGame& boardGame,
                       int index,
                       bool showTileIndices);

        //==============================================================================
        int getIndex() const noexcept { return index; }

        //==============================================================================
        void setShowingMoveHint (bool shouldShowMoveHint);
        void setShowingTileIndices (bool shouldShowTileIndices);

    protected:
        //==============================================================================
        BoardGame& boardGame;

        //==============================================================================
        bool isShowingMoveHints() const noexcept { return showMoveHints; }
        bool isShowingTileIndices() const noexcept { return showTileIndices; }

    private:
        //==============================================================================
        const int index;
        bool showMoveHints;
        bool showTileIndices;

        //==============================================================================
        TileComponent() BGE_DELETED_FUNCTION;
        BGE_DECLARE_NON_COPYABLE (TileComponent)
    };

protected:
    //==============================================================================
    BoardGame& boardGame;

    //==============================================================================
    void initialise();

    virtual TileComponent* createTileComponent (int index) = 0;

private:
    //==============================================================================
    TileBoard& tileBoard;
    SoundManager& soundManager;
    bool showingMoveHints, showingTileIndices;
    juce::OwnedArray<TileComponent> tiles;

    //==============================================================================
    void showMoveHints();
    void showMoveHints (const std::vector<int>& hintLocations);
    void hideMoveHints();
    void showTileIndices();
    void hideTileIndices();

    //==============================================================================
    TileBoardComponent() BGE_DELETED_FUNCTION;
    BGE_DECLARE_NON_COPYABLE (TileBoardComponent)
};

#endif //BGE_TILE_BOARD_COMPONENT_H