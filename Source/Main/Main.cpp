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

class ReversiApplication : public JUCEApplication
{
public:
    ReversiApplication() { }

    //==============================================================================
    /** @internal */
    const String getApplicationName() override              { return ProjectInfo::projectName; }
    /** @internal */
    const String getApplicationVersion() override           { return ProjectInfo::versionString; }
    /** @internal */
    bool moreThanOneInstanceAllowed() override              { return true; }
    /** @internal */
    void initialise (const String&) override                { mainWindow = new MainWindow (getApplicationName()); }
    /** @internal */
    void shutdown() override                                { mainWindow = nullptr; }
    /** @internal */
    void systemRequestedQuit() override                     { quit(); }
    /** @internal */
    void anotherInstanceStarted (const String&) override    {}

private:
    //==============================================================================
    class MainWindow : public DocumentWindow
    {
    public:
        MainWindow (const String& name) :
            DocumentWindow (name + " --- Jo\xc3\xabl R. Langlois",
                                  Colour (71, 115, 187),
                                  DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);
            setResizable (true, false);

            centreWithSize (getWidth(), getHeight());
            setVisible (true);

            if (auto* cbc = getConstrainer())
                cbc->setMinimumSize (600, 600);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        //==============================================================================
        MainWindow() JUCE_DELETED_FUNCTION;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

    //==============================================================================
    ScopedPointer<DocumentWindow> mainWindow;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReversiApplication)
};

//==============================================================================
START_JUCE_APPLICATION (ReversiApplication)
