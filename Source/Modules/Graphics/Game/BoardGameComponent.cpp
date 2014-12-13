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
//==============================================================================
BoardGameComponent::PlayerComponent::PlayerComponent (BoardGame& bg, const bool firstPlayer) :
    PlayerView (bg, firstPlayer)
{
    setOpaque (true);
}

//==============================================================================
BoardGameComponent::BoardGameComponent (BoardGame& bg,
                                        SoundManager& sm,
                                        LocalisationManager& lm,
                                        const bool mh,
                                        const bool ti) :
    BoardGameView (bg, mh, ti),
    soundManager (sm),
    localisationManager (lm)
{
    boardGame.addListener (this);
    localisationManager.addChangeListener (this);
}

BoardGameComponent::~BoardGameComponent()
{
    boardGame.removeListener (this);
    localisationManager.removeChangeListener (this);
    playerComponents.clearQuick();
    tileBoardComponent = nullptr;
}

//==============================================================================
void BoardGameComponent::reset()
{
    BoardGameView::reset();
    tileBoardComponent->reset();
    resized();
    repaint();
}

void BoardGameComponent::initialise()
{
    BoardGameView::initialise();

    tileBoardComponent = createTileBoardComponent();
    jassert (tileBoardComponent != nullptr);
    addAndMakeVisible (tileBoardComponent);

    if (PlayerComponent* const pc = dynamic_cast<PlayerComponent*> (getPlayerView (true)))
    {
        playerComponents.addIfNotAlreadyThere (pc);
        addAndMakeVisible (pc);
    }

    if (PlayerComponent* const pc = dynamic_cast<PlayerComponent*> (getPlayerView (false)))
    {
        playerComponents.addIfNotAlreadyThere (pc);
        addAndMakeVisible (pc);
    }
}

//==============================================================================
void BoardGameComponent::updateMoveHints()  { tileBoardComponent->setShowingMoveHints (true); }
void BoardGameComponent::hideMoveHints()    { tileBoardComponent->setShowingMoveHints (false); }
void BoardGameComponent::showTileIndices()  { tileBoardComponent->setShowingTileIndices (true); }
void BoardGameComponent::hideTileIndices()  { tileBoardComponent->setShowingTileIndices (false); }

void BoardGameComponent::attemptedTileStateChange (const int, const bool wasSuccessfullyChanged)
{
    if (wasSuccessfullyChanged)
    {
        const BoardGame::ScopedReverseTurnTaker srtt (boardGame);
        soundManager.playSound (SoundManager::tileChanged);
    }
    else
    {
        soundManager.playSound (SoundManager::tileFailedChange);
    }

    if (boardGame.isGameOver())
    {
        const int player1Score = boardGame.getScore (true);
        const int player2Score = boardGame.getScore (false);
        const bool didPlayerOneWin = player1Score > player2Score;

        juce::String message = player1Score == player2Score
                                    ? TRANS ("Tie game with a score of SSSS!")
                                    : TRANS ("Player 1234 has won the game with a score of SSSS!")
                                        .replace ("1234", juce::String (didPlayerOneWin ? 1 : 2)) ;

        message = message.replace ("SSSS", juce::String (didPlayerOneWin ? player1Score : player2Score));

        juce::AlertWindow aw (TRANS ("Game has ended!"), message, juce::AlertWindow::InfoIcon);

        aw.addButton ("OK", 1, juce::KeyPress (juce::KeyPress::returnKey), juce::KeyPress (juce::KeyPress::escapeKey));

        aw.runModalLoop(); //Just run the modal loop and no matter the result, reset the game.
        reset();
    }
}

//==============================================================================
void BoardGameComponent::paint (juce::Graphics& g)
{
    g.setColour (juce::Colours::black);
    g.drawRoundedRectangle (tileBoardComponent->getBounds().toFloat(), juce::float_Pi, (float) tileBoardBorderThickness);
}

void BoardGameComponent::resized()
{
    enum
    {
        margin              = 8,
        doubleMargin        = margin * 2,
        tooltipHeight       = 128,
        playerCompHeight    = 64
    };

    const int numColumns = getTileBoard().getNumberOfColumns();
    const int numRows = getTileBoard().getNumberOfRows();

    const int boardSpaceHeight = getHeight() - playerCompHeight;
    const int tileBoardSize = juce::jmin (getWidth() - tileBoardBorderMargin,
                                          boardSpaceHeight - margin - tileBoardBorderMargin);

    int tileBoardWidth = numColumns > numRows
                                ? tileBoardSize
                                : juce::roundToInt ((double) tileBoardSize * ((double) numColumns / (double) numRows));

    const int tileBoardHeight = numColumns > numRows
                                    ? juce::roundToInt ((double) tileBoardSize * ((double) numRows / (double) numColumns))
                                    : tileBoardSize;

    tileBoardComponent->setBounds ((getWidth() / 2) - (tileBoardWidth / 2),
                                   (boardSpaceHeight / 2) - (tileBoardHeight / 2),
                                   tileBoardWidth,
                                   tileBoardHeight);

    const int numPlayerComps    = playerComponents.size();
    const int playerCompsY      = getHeight() - playerCompHeight;
    const int playerCompsWidth  = juce::jmin (250, (getWidth() / numPlayerComps) - ((numPlayerComps - 2) * margin));

    int playerCompsX = (getWidth() / 2) - ((playerCompsWidth * numPlayerComps) + ((numPlayerComps - 2) * margin)) / 2;

    for (int i = 0; i < numPlayerComps; ++i)
    {
        PlayerComponent* const pc = playerComponents.getUnchecked (i);
        pc->setBounds (playerCompsX,
                       playerCompsY,
                       playerCompsWidth,
                       playerCompHeight);

        playerCompsX = pc->getRight() + margin;
    }
}

void BoardGameComponent::changeListenerCallback (juce::ChangeBroadcaster*)
{
    for (int i = playerComponents.size(); --i >= 0;)
        playerComponents.getUnchecked (i)->updateText();
}