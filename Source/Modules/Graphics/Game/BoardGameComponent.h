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
#ifndef BGE_BOARD_GAME_COMPONENT_H
#define BGE_BOARD_GAME_COMPONENT_H

#include "TileBoardComponent.h"

/** */
class BoardGameComponent : public juce::Component,
                           public BoardGameView,
                           private juce::ChangeListener
{
public:
    /** Constructor */
    BoardGameComponent (BoardGame& boardGame,
                        SoundManager& soundManager,
                        LocalisationManager& localisationManager,
                        bool showMoveHints,
                        bool showTileIndices);

    /** Destructor */
    ~BoardGameComponent();

    //==============================================================================
    /** @internal */
    void paint (juce::Graphics& g) override;
    /** @internal */
    void resized() override;
    /** @internal */
    void attemptedTileStateChange (BoardGame* boardGame, int index, bool wasSuccessfullyChanged) override;
    /** @internal */
    void gameEnded (BoardGame* boardGame) override;
    /** @internal */
    void initialise() override;
    /** @internal */
    void reset() override;
    /** @internal */
    void updateMoveHints() override;
    /** @internal */
    void hideMoveHints() override;
    /** @internal */
    void showTileIndices() override;
    /** @internal */
    void hideTileIndices() override;

    //==============================================================================
    /** */
    class PlayerComponent : public juce::Component,
                            public PlayerView
    {
    public:
        /** Constructor

            @param[in] boardGame
            @param[in] firstPlayer
        */
        PlayerComponent (BoardGame& boardGame, bool firstPlayer);

        //==============================================================================
        /** Callback to notify that the text needs to be updated
            because the language has changed.
        */
        virtual void updateText() = 0;

    private:
        //==============================================================================
        PlayerComponent() BGE_DELETED_FUNCTION;
        BGE_DECLARE_NON_COPYABLE (PlayerComponent)
    };

protected:
    //==============================================================================
    SoundManager& soundManager;
    LocalisationManager& localisationManager;

    //==============================================================================
    /** Create your custom component that will represent the tile board */
    virtual TileBoardComponent* createTileBoardComponent() = 0;

private:
    //==============================================================================
    enum
    {
        tileBoardBorderThickness = 3,
        tileBoardBorderMargin = tileBoardBorderThickness * 2
    };

    //==============================================================================
    juce::ScopedPointer<TileBoardComponent> tileBoardComponent;
    juce::Array<PlayerComponent*> playerComponents;

    //==============================================================================
    void setupComponents();

    //==============================================================================
    /** @internal */
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;

    //==============================================================================
    BoardGameComponent() BGE_DELETED_FUNCTION;
    BGE_DECLARE_NON_COPYABLE (BoardGameComponent)
};

#endif //BGE_BOARD_GAME_COMPONENT_H