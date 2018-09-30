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
    norwester = Typeface::createSystemTypefaceFor (Fonts::norwester_otf, Fonts::norwester_otf_Size);
    droidSans = Typeface::createSystemTypefaceFor (Fonts::droidsans_ttf, Fonts::droidsans_ttf_Size);

    setDefaultSansSerifTypefaceName ("Droid Sans");
}

//==============================================================================
void BGELookAndFeel::drawReversiToken (Graphics& g,
                                       const bool isFirstPlayer,
                                       const bool isTileBlank,
                                       const bool isMoveHint,
                                       const bool isMouseOver,
                                       const bool isButtonDown,
                                       const juce::Rectangle<int>& area)
{
    if (! isTileBlank || isMoveHint)
    {
        const auto a = area.reduced (2).toFloat();

        auto baseColour = isFirstPlayer
                            ? Colours::black
                            : Colours::white;

        if (isMoveHint) baseColour = baseColour.withAlpha (0.15f);
        if (isButtonDown) baseColour = baseColour.withAlpha (0.5f);

        const auto startColour = isFirstPlayer
                                    ? baseColour.brighter()
                                    : baseColour;

        const auto endColour = isFirstPlayer
                                ? baseColour
                                : baseColour.darker();

        { //Draw the base circle:
            const ColourGradient gradient (startColour, 0.0f, 0.0f,
                                           endColour, a.getWidth(), a.getHeight(),
                                           false);
            g.setFillType (gradient);
            g.fillEllipse (a);
        }

        { //Draw the inner circle:
            const auto ic = a.reduced (a.getWidth() * 0.1f);

            const ColourGradient gradient (endColour, ic.getX(), ic.getY(),
                                           startColour, ic.getWidth(), ic.getHeight(),
                                           false);
            g.setFillType (gradient);
            g.fillEllipse (ic);
        }
    }

    if (isMouseOver)
    {
        g.setColour (Colours::white.withAlpha (0.35f));
        g.fillRoundedRectangle (area.toFloat(), float_Pi);
    }
}

//==============================================================================
Typeface::Ptr BGELookAndFeel::getTypefaceForFont (const Font& f)
{
    if (f.getTypefaceName() == Font::getDefaultSansSerifFontName())
        return droidSans;

    if (f.getTypefaceName().containsIgnoreCase ("norwester"))
        return norwester;

    return LookAndFeel_V3::getTypefaceForFont (f);
}
