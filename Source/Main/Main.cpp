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
#include "MainComponent.h"

class ReversiApplication : public juce::JUCEApplication
{
public:
    ReversiApplication() { }

    //==============================================================================
    /** @internal */
    const juce::String getApplicationName() override            { return ProjectInfo::projectName; }
    /** @internal */
    const juce::String getApplicationVersion() override         { return ProjectInfo::versionString; }
    /** @internal */
    bool moreThanOneInstanceAllowed() override                  { return true; }
    /** @internal */
    void initialise (const juce::String&) override              { mainWindow = new MainWindow (getApplicationName()); }
    /** @internal */
    void shutdown() override                                    { mainWindow = nullptr; }
    /** @internal */
    void systemRequestedQuit() override                         { quit(); }
    /** @internal */
    void anotherInstanceStarted (const juce::String&) override  {}

private:
    //==============================================================================
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow (const juce::String& name) :
            juce::DocumentWindow (name + " --- Jo\xc3\xabl R. Langlois",
                                  juce::Colour (71, 115, 187),
                                  juce::DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);
            setResizable (true, false);

            centreWithSize (getWidth(), getHeight());
            setVisible (true);

            if (juce::ComponentBoundsConstrainer* const cbc = getConstrainer())
                cbc->setMinimumSize (600, 600);
        }

        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        //==============================================================================
        MainWindow() JUCE_DELETED_FUNCTION;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

    //==============================================================================
    juce::ScopedPointer<juce::DocumentWindow> mainWindow;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReversiApplication)
};

//==============================================================================
START_JUCE_APPLICATION (ReversiApplication)