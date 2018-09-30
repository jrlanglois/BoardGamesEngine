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
class ReversiBoardGameComponent::ReversiPlayerComponent : public BoardGameComponent::PlayerComponent
{
public:
    ReversiPlayerComponent (BoardGame& bg, bool fp) :
        PlayerComponent (bg, fp),
        playerName (String::empty, TRANS ("PLAYER #").replace ("#", String (fp ? 1 : 2)))
    {
        boardGame.addListener (this);

        playerName.setFont (Font ("Norwester", 36.0f, Font::plain));
        playerName.setColour (Label::textColourId, Colours::white);
        playerName.setJustificationType (Justification::centred);

        score.setFont (Font (28.0f));
        score.setColour (Label::textColourId, Colours::white);
        score.setJustificationType (Justification::centredLeft);

        updateText();

        addAndMakeVisible (&playerName);
        addAndMakeVisible (&score);
    }

    ~ReversiPlayerComponent()
    {
        boardGame.removeListener (this);
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        g.fillAll (Colour (0xff7A7878));

        enum { playerTokenReductionPixels = 8 };

        const int tokenSize = jmin (getWidth(), getHeight());

        BGELookAndFeel::drawReversiToken (g,
                                          firstPlayer, false, false,
                                          false, false,
                                          juce::Rectangle<int> (tokenSize, tokenSize).reduced (playerTokenReductionPixels));
    }

    void paintOverChildren (Graphics& g) override
    {
        if (boardGame.isFirstPlayerTurn() == firstPlayer)
        {
            g.setColour (Colours::red);
            g.drawRect (getLocalBounds(), 2);
        }
    }

    void resized() override
    {
        const int halfHeight = getHeight() / 2;
        const int tokenSize = jmin (getWidth(), getHeight());

        playerName.setBounds (0,
                              0,
                              getWidth(),
                              halfHeight);

        score.setBounds (tokenSize,
                         halfHeight,
                         getWidth() - tokenSize,
                         halfHeight);
    }

    //==============================================================================
    void updateText() override
    {
        playerName.setText (TRANS ("PLAYER #").replace ("#", String (firstPlayer ? 1 : 2)),
                            dontSendNotification);

        updateScore();
    }

    void updateScore() override
    {
        score.setText (TRANS ("SCORE") + ": " + String (boardGame.getScore (firstPlayer)),
                       dontSendNotification);
    }

    void playerChanged (BoardGame*, bool) override
    {
        updateScore();
        resized();
        repaint();
    }

    /** (Unused) */
    void attemptedTileStateChange (BoardGame*, int, bool) override { }
    /** (Unused) */
    void gameEnded (BoardGame*) override { }

private:
    //==============================================================================
    Label playerName, score;

    //==============================================================================
    BGE_DECLARE_NON_COPYABLE (ReversiPlayerComponent)
};

//==============================================================================
ReversiBoardGameComponent::ReversiBoardGameComponent (BoardGame& bg,
                                                      SoundManager& sm,
                                                      LocalisationManager& lm,
                                                      const bool mh,
                                                      const bool ti) :
    BoardGameComponent (bg, sm, lm, mh, ti)
{
    initialise();
}

//==============================================================================
PlayerView* ReversiBoardGameComponent::createPlayerView (const bool forFirstPlayer)
{
    return new ReversiPlayerComponent (boardGame, forFirstPlayer);
}

TileBoardComponent* ReversiBoardGameComponent::createTileBoardComponent()
{
    return new ReversiTileBoardComponent (boardGame,
                                          soundManager,
                                          isShowingMoveHints(),
                                          isShowingTileIndices());
}
