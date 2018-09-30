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
//==============================================================================
TileBoardComponent::TileComponent::TileComponent (BoardGame& bg,
                                                  const int i,
                                                  const bool ti) :
    Button (String (i)),
    boardGame (bg),
    index (i),
    showMoveHints (false),
    showTileIndices (ti)
{
}

void TileBoardComponent::TileComponent::setShowingMoveHint (const bool shouldShowMoveHint)
{
    if (showMoveHints != shouldShowMoveHint)
    {
        showMoveHints = shouldShowMoveHint;
        repaint();
    }
}

void TileBoardComponent::TileComponent::setShowingTileIndices (const bool shouldShowTileIndices)
{
    if (showTileIndices != shouldShowTileIndices)
    {
        showTileIndices = shouldShowTileIndices;
        repaint();
    }
}

//==============================================================================
TileBoardComponent::TileBoardComponent (BoardGame& bg,
                                        SoundManager& sm,
                                        const bool moveHints,
                                        const bool tileIndices) :
    boardGame (bg),
    tileBoard (bg.getTileBoard()),
    soundManager (sm),
    showingMoveHints (moveHints),
    showingTileIndices (tileIndices)
{
}

//==============================================================================
void TileBoardComponent::reset()
{
    if (isShowingMoveHints())
        showMoveHints();
    else
        hideMoveHints();
}

//==============================================================================
void TileBoardComponent::showMoveHints (const std::vector<int>& hintLocations)
{
    for (int i = 0; i < tiles.size(); ++i)
        tiles.getUnchecked (i)->setShowingMoveHint (std::binary_search (hintLocations.begin(), hintLocations.end(), i));
}

void TileBoardComponent::showMoveHints()
{
    std::vector<int> hintLocations;
    boardGame.getAllPossibleMoves (hintLocations);
    showMoveHints (hintLocations);
}

void TileBoardComponent::hideMoveHints()
{
    for (auto& tile : tiles)
        tile->setShowingMoveHint (false);
}

void TileBoardComponent::setShowingMoveHints (const bool shouldShowHints)
{
    if (showingMoveHints != shouldShowHints)
    {
        showingMoveHints = shouldShowHints;

        if (showingMoveHints)
            showMoveHints();
        else
            hideMoveHints();
    }
}

//==============================================================================
void TileBoardComponent::setShowingTileIndices (const bool shouldShowTileIndices)
{
    if (showingTileIndices != shouldShowTileIndices)
    {
        showingTileIndices = shouldShowTileIndices;

        for (auto& tile : tiles)
            tile->setShowingTileIndices (showingTileIndices);
    }
}

//==============================================================================
void TileBoardComponent::resized()
{
    const int width         = getWidth();
    const int height        = getHeight();
    const int numC          = tileBoard.getNumberOfColumns();
    const int numR          = tileBoard.getNumberOfRows();
    const int tileWidth     = width / numC;
    const int tileHeight    = height / numR;

    for (int j = 0; j < numR; ++j)
        for (int i = 0; i < numC; ++i)
            tiles.getUnchecked (i + (j * numC))->setBounds (i * tileWidth, j * tileHeight, tileWidth, tileHeight);
}

void TileBoardComponent::paint (Graphics& g)
{
    g.setColour (Colours::darkgrey.withAlpha (0.5f));
    g.fillRoundedRectangle (getLocalBounds().toFloat(), float_Pi);
}

void TileBoardComponent::buttonClicked (Button* const button)
{
    for (int i = tiles.size(); --i >= 0;)
    {
        if (tiles.getUnchecked (i) == button)
        {
            const bool result = boardGame.setTileStateToCurrentPlayer (i);

            if (result && showingMoveHints)
                showMoveHints(); //Force move hint update if they're showing

            repaint();
            break;
        }
    }
}

//==============================================================================
void TileBoardComponent::initialise()
{
    const int numTiles = tileBoard.getTotalNumTiles();
    for (int i = 0; i < numTiles; ++i)
    {
        auto* tileComponent = createTileComponent (i);
        jassert (tileComponent != nullptr);

        tiles.add (tileComponent)->addListener (this);
        addAndMakeVisible (tileComponent);
    }

    if (showingMoveHints)
        showMoveHints();
}
