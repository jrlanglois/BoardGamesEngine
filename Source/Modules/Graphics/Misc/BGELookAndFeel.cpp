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
BGELookAndFeel::BGELookAndFeel()
{
    loadAndSetTypefaces();
    setDefaultLookAndFeel (this);
}

BGELookAndFeel::~BGELookAndFeel()
{
    setDefaultLookAndFeel (nullptr);
    getDefaultLookAndFeel(); //N.B.: Fixes up the desktop default L&F
}

//==============================================================================
void BGELookAndFeel::loadAndSetTypefaces()
{
    norwester = juce::Typeface::createSystemTypefaceFor (Fonts::norwester_otforwester_otf,
                                                         Fonts::norwester_otforwester_otf_Size);

    droidSans = juce::Typeface::createSystemTypefaceFor (Fonts::droidsans_ttfroidSans_ttf,
                                                         Fonts::droidsans_ttfroidSans_ttf_Size);

    setDefaultSansSerifTypefaceName ("Droid Sans");
}

//==============================================================================
void BGELookAndFeel::drawReversiToken (juce::Graphics& g,
                                       const bool isFirstPlayer,
                                       const bool isTileBlank,
                                       const bool isMoveHint,
                                       const bool isMouseOver,
                                       const bool isButtonDown,
                                       const juce::Rectangle<int>& area)
{
    if (! isTileBlank || isMoveHint)
    {
        const juce::Rectangle<float> a (area.reduced (2).toFloat());

        juce::Colour baseColour = isFirstPlayer
                                    ? juce::Colours::black
                                    : juce::Colours::white;

        if (isMoveHint) baseColour = baseColour.withAlpha (0.15f);
        if (isButtonDown) baseColour = baseColour.withAlpha (0.5f);

        const juce::Colour startColour = isFirstPlayer
                                            ? baseColour.brighter()
                                            : baseColour;

        const juce::Colour endColour = isFirstPlayer
                                            ? baseColour
                                            : baseColour.darker();

        { //Draw the base circle:
            const juce::ColourGradient gradient (startColour, 0.0f, 0.0f,
                                                 endColour, a.getWidth(), a.getHeight(),
                                                 false);
            g.setFillType (gradient);
            g.fillEllipse (a);
        }

        { //Draw the inner circle:
            const juce::Rectangle<float> ic (a.reduced (a.getWidth() * 0.1f));

            const juce::ColourGradient gradient (endColour, ic.getX(), ic.getY(),
                                                 startColour, ic.getWidth(), ic.getHeight(),
                                                 false);
            g.setFillType (gradient);
            g.fillEllipse (ic);
        }
    }

    if (isMouseOver)
    {
        g.setColour (juce::Colours::white.withAlpha (0.35f));
        g.fillRoundedRectangle (area.toFloat(), juce::float_Pi);
    }
}