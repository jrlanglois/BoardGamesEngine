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
class ReversiTileBoardComponent::ReversiTileComponent : public TileBoardComponent::TileComponent
{
public:
    ReversiTileComponent (BoardGame& bg,
                          int tileIndex,
                          bool ti) :
        TileComponent (bg, tileIndex, ti)
    {
    }

    //==============================================================================
    void paintButton (juce::Graphics& g,
                      bool isMouseOverButton,
                      bool isButtonDown) override
    {
        enum { playerTokenReductionPixels = 6 };

        const Tile tile (boardGame.getTileBoard().getTile (getIndex()));

        if (! isEnabled())
        {
            isMouseOverButton = false;
            isButtonDown = false;
        }

        g.setColour (juce::Colours::black.withAlpha (0.5f));
        g.drawRoundedRectangle (getLocalBounds().reduced (2).toFloat(), juce::float_Pi, 1.0f);

        BGELookAndFeel::drawReversiToken (g,
                                          isShowingMoveHints() ? boardGame.isFirstPlayerTurn() : tile.hasPlayerOne(),
                                          tile.isBlank(), isShowingMoveHints(),
                                          isMouseOverButton, isButtonDown,
                                          getLocalBounds().reduced (playerTokenReductionPixels));

        if (isShowingTileIndices())
        {
            if (tile.hasPlayer() && tile.hasPlayerOne())
            {
                g.setColour (juce::Colours::white);
            }
            else
            {
                g.setColour (isShowingMoveHints() && boardGame.isFirstPlayerTurn()
                                ? juce::Colours::white
                                : juce::Colours::black);
            }

            g.drawText (juce::String (getIndex()), getLocalBounds().toFloat().reduced (6.0f),
                        juce::Justification::centred, false);
        }
    }

private:
    //==============================================================================
    BGE_DECLARE_NON_COPYABLE (ReversiTileComponent)
};

//==============================================================================
ReversiTileBoardComponent::ReversiTileBoardComponent (BoardGame& bg,
                                                      SoundManager& sm,
                                                      const bool mh,
                                                      const bool ti) :
    TileBoardComponent (bg, sm, mh, ti)
{
    initialise();
}

//==============================================================================
TileBoardComponent::TileComponent* ReversiTileBoardComponent::createTileComponent (const int index)
{
    return new ReversiTileComponent (boardGame,
                                     index,
                                     isShowingTileIndices());
}