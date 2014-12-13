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
#ifndef MAIN_COMPONENT_H
#define MAIN_COMPONENT_H

#include "../Modules/BoardGamesEngine_Master.h"

class MainComponent : public juce::Component,
                      private juce::Button::Listener,
                      private juce::ChangeListener,
                      private BoardGame::Listener
{
public:
    MainComponent();
    ~MainComponent();

    //==============================================================================
    /** @internal */
    void resized() override;

private:
    //==============================================================================
    LocalisationManager localisationManager;
    SoundManager soundManager;
    juce::SystemTrayIconComponent systemTrayIconComponent;

    BGELookAndFeel laf;
    juce::TooltipWindow tooltipWindow;

    juce::ScopedPointer<juce::Toolbar> toolbar;

    class ToolbarItemFactory;
    juce::ScopedPointer<ToolbarItemFactory> toolbarItemFactory;

    juce::ScopedPointer<SettingsWindow> settingsWindow;

    //==============================================================================
    juce::ScopedPointer<BoardGame> boardGame;
    juce::ScopedPointer<BoardGameComponent> boardGameComponent;

    //==============================================================================
    enum
    {
        margin          = 8,
        buttonHeight    = 36
    };

    //==============================================================================
    void initialisBoardGameComponent();

    /** Change the current game being played

        @param[in] newBoardGame The new board game to start playing
    */
    void changeGame (BoardGame* newBoardGame);

    void updateTrayIconTooltip();

    //==============================================================================
    /** @internal */
    void buttonClicked (juce::Button* button) override;
    /** @internal */
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    /** @internal */
    void attemptedTileStateChange (int index, bool wasSuccessfullyChanged) override;
    /** @internal */
    void playerChanged (bool toFirstPlayer) override;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

#endif //MAIN_COMPONENT_H