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
BGESystemTrayIconComponent::BGESystemTrayIconComponent()
{
    initialiseImage();
}

//==============================================================================
void BGESystemTrayIconComponent::initialiseImage()
{
    const int size = 128;
    const int halfSize = size / 2;

    juce::Image image (juce::Image::ARGB, 128, 128, true);
    juce::Graphics g (image);

    const juce::Rectangle<int> b (halfSize, halfSize);
    BGELookAndFeel::drawReversiToken (g, true, false, false, false, false, b);
    BGELookAndFeel::drawReversiToken (g, true, false, false, false, false, b.withPosition (halfSize, halfSize));
    BGELookAndFeel::drawReversiToken (g, false, false, false, false, false, b.withPosition (halfSize, 0));
    BGELookAndFeel::drawReversiToken (g, false, false, false, false, false, b.withPosition (0, halfSize));

    setIconImage (image);
}

//==============================================================================
void BGESystemTrayIconComponent::updateTooltipFromBoardGame (BoardGame& boardGame)
{
    juce::String tooltip = TRANS ("Board Games Engine");

    tooltip += juce::newLine;

    tooltip += TRANS ("Score") + ": ";
    tooltip += juce::String (boardGame.getScore (true)) + " - " + juce::String (boardGame.getScore (false));

    setIconTooltip (tooltip);
}

void BGESystemTrayIconComponent::bringWindowToFocus()
{
    juce::Desktop& desktop = juce::Desktop::getInstance();

    for (int i = desktop.getNumComponents(); --i >= 1;)
        if (juce::Component* topLevelComp = desktop.getComponent (i))
            topLevelComp->toFront (false);

    if (juce::Component* topLevelComp = desktop.getComponent (0))
        topLevelComp->toFront (true);
}

//==============================================================================
void BGESystemTrayIconComponent::mouseDown (const juce::MouseEvent& e)
{
    bringWindowToFocus();

    if (e.mods.isRightButtonDown())
    {
        enum Options
        {
            about = 1,
            exit
        };

        juce::PopupMenu menu;

        menu.addSectionHeader (juce::JUCEApplication::getInstance()->getApplicationName());
        menu.addSeparator();

        menu.addItem (about, TRANS ("About"));
        menu.addItem (exit, TRANS ("Exit"));

        switch (menu.show())
        {
            case about: juce::Process::openDocument ("http://www.jrlanglois.com/CV/jrlanglois-cv-2014.pdf", juce::String::empty); break;
            case exit:  juce::JUCEApplication::getInstance()->quit(); break;

            default: break;
        };
    }
}