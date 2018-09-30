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

//==============================================================================
class ReversiGameStateHandler
{
public:
    ReversiGameStateHandler() noexcept
    {
    }

    //==============================================================================
    BoardGame* restore (bool& userCancelledOperation) const
    {
        const File dataFolder (getDataFolder());

        FileChooser fc (TRANS ("Choose an existing game file"),
                              dataFolder,
                              "*.reversi");

        ScopedPointer<BoardGame> newBoardGame;

        userCancelledOperation = ! fc.browseForFileToOpen();
        if (! userCancelledOperation)
        {
            ScopedPointer<InputStream> fis (fc.getResult().createInputStream());
            String result;

            if (fis != nullptr)
                result = fis->readString();

            if (result.isNotEmpty())
            {
                const auto state = result.toStdString();

                newBoardGame = new StandardReversiBoardGame();
                if (newBoardGame->getTileBoard().loadFromState (state))
                    return newBoardGame.release();

                newBoardGame = new SmallReversiBoardGame();
                if (newBoardGame->getTileBoard().loadFromState (state))
                    return newBoardGame.release();

                newBoardGame = new WideReversiBoardGame();
                if (newBoardGame->getTileBoard().loadFromState (state))
                    return newBoardGame.release();
            }
        }

        return newBoardGame.release();
    }

    //==============================================================================
    bool save (TileBoard& tileBoard) const
    {
        const File dataFolder (getDataFolder());

        FileChooser fc (TRANS ("Choose a location to save the game file"),
                              dataFolder,
                              "*.reversi");

        if (fc.browseForFileToSave (true))
            if (ScopedPointer<OutputStream> fos = fc.getResult().createOutputStream())
                return fos->writeText (String (tileBoard.saveState()), false, false, "\r\n");

        return false;
    }

private:
    //==============================================================================
    String getDefaultDataFolderPath() const
    {
        return File::getSpecialLocation (File::userDocumentsDirectory).getFullPathName() + "/Reversi/";
    }

    File getDataFolder() const
    {
        const File dataFolder (getDefaultDataFolderPath());

        if (! dataFolder.exists())
        {
           #if JUCE_DEBUG
            jassert (dataFolder.createDirectory().wasOk());
           #else
            dataFolder.createDirectory();
           #endif
        }

        return dataFolder;
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReversiGameStateHandler)
};

//==============================================================================
class MainComponent::ToolbarItemFactory : public juce::ToolbarItemFactory
{
public:
    //==============================================================================
    enum ItemId
    {
        save = 1,
        open,
        showHints,
        showTileIndices,
        changeGame,
        restart,
        about,
        settings,

        numItems = settings
    };

    //==============================================================================
    ToolbarItemFactory (Button::Listener& bl,
                        LocalisationManager& lm) :
        buttonListener (bl),
        localisationManager (lm)
    {
        buttonInfos.add (new ButtonInfo (save, NEEDS_TRANS ("Save"), NEEDS_TRANS ("Save the current game"),
                                         ToolbarIcons::save_svg, ToolbarIcons::save_svgSize));

        buttonInfos.add (new ButtonInfo (open, NEEDS_TRANS ("Open"), NEEDS_TRANS ("Open an existing game"),
                                         ToolbarIcons::open_svg, ToolbarIcons::open_svgSize));

        {
            Path checkerBoard;
            checkerBoard.addRectangle (0.0f, 0.0f, 1.0f, 1.0f);
            checkerBoard.addRectangle (1.0f, 1.0f, 1.0f, 1.0f);
            checkerBoard.addLineSegment (Line<float> (2.0f, 0.0f, 2.0f, 1.0f), 0.1f); //Vertical, top-right
            checkerBoard.addLineSegment (Line<float> (1.0f, 0.0f, 2.0f, 0.0f), 0.1f); //Horizontal, top-right
            checkerBoard.addLineSegment (Line<float> (0.0f, 1.0f, 0.0f, 2.0f), 0.1f); //Vertical, bottom-left
            checkerBoard.addLineSegment (Line<float> (0.0f, 2.0f, 2.0f, 2.0f), 0.1f); //Horizontal, bottom-left

            DrawablePath* dp = new DrawablePath();
            dp->setPath (checkerBoard);

            buttonInfos.add (new ButtonInfo (changeGame,
                                             NEEDS_TRANS ("Change Game"),
                                             NEEDS_TRANS ("Change the current game type"),
                                             dp));
        }

        {
            auto* di = new DrawableImage();
            createHintButtonImage (*di, true);

            auto* toggledOn = new DrawableImage();
            createHintButtonImage (*toggledOn, false);

            buttonInfos.add (new ButtonInfo (showHints,
                                             NEEDS_TRANS ("Show or hide the hints"),
                                             NEEDS_TRANS ("Shows or hides the possible places the current player can place a token onto."),
                                             di, true, true))->toggleOnImage = toggledOn;
        }

        {
            auto* di = new DrawableImage();
            createTileIndexButtonImage (*di, true);

            auto* toggledOn = new DrawableImage();
            createTileIndexButtonImage (*toggledOn, false);

            buttonInfos.add (new ButtonInfo (showTileIndices,
                                             NEEDS_TRANS ("Shows or hides the tile indices"),
                                             NEEDS_TRANS ("Shows or hides the tile indices"),
                                             di, true, true))->toggleOnImage = toggledOn;
        }

        buttonInfos.add (new ButtonInfo (restart, NEEDS_TRANS ("Restart"), NEEDS_TRANS ("Restart the current game"),
                                         ToolbarIcons::restart_svg, ToolbarIcons::restart_svg_Size));

        buttonInfos.add (new ButtonInfo (about, NEEDS_TRANS ("About"), NEEDS_TRANS ("The man behind the madness"),
                                         ToolbarIcons::about_svg, ToolbarIcons::about_svg_Size));

        buttonInfos.add (new ButtonInfo (settings, NEEDS_TRANS ("Settings"), NEEDS_TRANS ("Basic settings you can change"),
                                         ToolbarIcons::gear_svg, ToolbarIcons::gear_svg_Size));
    }

    //==============================================================================
    void getAllToolbarItemIds (Array<int>& ids) override
    {
        for (int i = 0; i < numItems; ++i)
            ids.add (i + 1);

        ids.add (ToolbarItemFactory::separatorBarId);
    }

    void getDefaultItemSet (Array<int>& ids) override
    {
        ids.add (save);
        ids.add (open);
        ids.add (ToolbarItemFactory::separatorBarId);
        ids.add (showHints);
        ids.add (showTileIndices);
        ids.add (ToolbarItemFactory::separatorBarId);
        ids.add (changeGame);
        ids.add (restart);
        ids.add (ToolbarItemFactory::separatorBarId);
        ids.add (about);
        ids.add (settings);
    }

    //==============================================================================
    class CustomToolbarButton : public ToolbarButton,
                                private ChangeListener
    {
    public:
        CustomToolbarButton (int id,
                             const String& labelText,
                             const String& tooltipUntrans,
                             Drawable* normalImg,
                             Drawable* toggledOnImg,
                             LocalisationManager& lm) :
            ToolbarButton (id, labelText, normalImg, toggledOnImg),
            text (labelText),
            tooltip (tooltipUntrans),
            localisationManager (lm)
        {
            updateText();
            localisationManager.addChangeListener (this);
        }

        ~CustomToolbarButton()
        {
            localisationManager.removeChangeListener (this);
        }

    private:
        const String text, tooltip;
        LocalisationManager& localisationManager;

        void updateText()
        {
            setButtonText (TRANS (text));
            setTooltip (TRANS (tooltip));
        }

        void changeListenerCallback (ChangeBroadcaster*) override
        {
            updateText();
        }

        CustomToolbarButton() JUCE_DELETED_FUNCTION;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomToolbarButton)
    };

    //==============================================================================
    ToolbarItemComponent* createItem (int itemId) override
    {
        for (auto* bi : buttonInfos)
        {
            if (bi != nullptr)
            {
                if (bi->id == itemId)
                {
                    auto* button = new CustomToolbarButton (itemId,
                                                            bi->name,
                                                            bi->tooltip,
                                                            bi->drawable->createCopy(),
                                                            bi->toggleOnImage != nullptr ? bi->toggleOnImage->createCopy() : nullptr,
                                                            localisationManager);
                    button->addListener (&buttonListener);

                    if (bi->togglesState)
                    {
                        button->setClickingTogglesState (true);
                        button->setToggleState (bi->initialToggleState, dontSendNotification);
                    }

                    return button;
                }
            }
        }

        return nullptr;
    }

private:
    //==============================================================================
    class ButtonInfo
    {
    public:
        ButtonInfo (ItemId idTag,
                    const String& n,
                    const String& t,
                    const void* pathData, size_t pathSizeBytes) :
            id (idTag),
            name (n),
            tooltip (t),
            togglesState (false),
            initialToggleState (false)
        {
            drawable = Drawable::createFromImageData (pathData, pathSizeBytes);
        }

        ButtonInfo (ItemId idTag,
                    const String& n,
                    const String& t,
                    Drawable* d,
                    bool isToggle = false,
                    bool firstState = false) :
            id (idTag),
            name (n),
            tooltip (t),
            drawable (d),
            togglesState (isToggle),
            initialToggleState (firstState)
        {
        }

        //==============================================================================
        const ItemId id;
        const String name; //< Untranslated
        const String tooltip; //< Untranslated
        ScopedPointer<Drawable> drawable, toggleOnImage;
        const bool togglesState, initialToggleState;

    private:
        //==============================================================================
        ButtonInfo() JUCE_DELETED_FUNCTION;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ButtonInfo)
    };

    //==============================================================================
    Button::Listener& buttonListener;
    LocalisationManager& localisationManager;
    OwnedArray<ButtonInfo> buttonInfos;

    //==============================================================================
    static void createHintButtonImage (DrawableImage& destination, bool isCrossedOut)
    {
        const juce::Rectangle<float> b (64.0f, 64.0f);
        Image image (Image::ARGB, (int) b.getWidth(), (int) b.getHeight(), true);
        Graphics g (image);
        g.setColour (Colours::black);
        g.drawRoundedRectangle (b.reduced (1.0f), float_Pi, 1.0f);
        g.fillEllipse (b.reduced (6.0f));

        if (isCrossedOut)
        {
            g.setColour (Colours::red.withAlpha (0.8f));
            g.drawLine (0.0f, b.getHeight(),
                        b.getWidth(), 0.0f,
                        b.getWidth() * 0.1f);
        }

        destination.setImage (image);
    }

    static void createTileIndexButtonImage (DrawableImage& destination, bool isCrossedOut)
    {
        const juce::Rectangle<float> b (64.0f, 64.0f);
        Image image (Image::ARGB, (int) b.getWidth(), (int) b.getHeight(), true);
        Graphics g (image);
        g.setColour (Colours::black);
        g.drawRoundedRectangle (b.reduced (1.0f), float_Pi, 1.0f);
        g.setFont (Font (b.getHeight()));
        g.drawText ("#", b.reduced (3.0f), Justification::centred, false);

        if (isCrossedOut)
        {
            g.setColour (Colours::red.withAlpha (0.8f));
            g.drawLine (0.0f, b.getHeight(),
                        b.getWidth(), 0.0f,
                        b.getWidth() * 0.1f);
        }

        destination.setImage (image);
    }

    //==============================================================================
    ToolbarItemFactory() JUCE_DELETED_FUNCTION;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolbarItemFactory)
};

//==============================================================================
MainComponent::MainComponent() :
    toolbar (new Toolbar()),
    boardGame (new StandardReversiBoardGame())
{
    setLookAndFeel (&laf);

   #if ! JUCE_IOS
    {
        const int size = 128;
        const int halfSize = size / 2;

        Image image (Image::ARGB, 128, 128, true);
        Graphics g (image);

        const juce::Rectangle<int> b (halfSize, halfSize);
        BGELookAndFeel::drawReversiToken (g, true, false, false, false, false, b);
        BGELookAndFeel::drawReversiToken (g, true, false, false, false, false, b.withPosition (halfSize, halfSize));
        BGELookAndFeel::drawReversiToken (g, false, false, false, false, false, b.withPosition (halfSize, 0));
        BGELookAndFeel::drawReversiToken (g, false, false, false, false, false, b.withPosition (0, halfSize));

        systemTrayIconComponent.setIconImage (image);
    }
   #endif //JUCE_IOS

    toolbarItemFactory = new ToolbarItemFactory (*this, localisationManager);
    toolbar->addDefaultItems (*toolbarItemFactory);
    toolbar->setColour (Toolbar::backgroundColourId, Colours::white);
    addAndMakeVisible (toolbar);

    initialisBoardGameComponent();

    setSize (700, 700);
}

MainComponent::~MainComponent()
{
    boardGameComponent = nullptr;
    toolbar = nullptr;
    toolbarItemFactory = nullptr;

    setLookAndFeel (nullptr);
}

//==============================================================================
void MainComponent::initialisBoardGameComponent()
{
    if (boardGame != nullptr)
    {
        boardGame->addListener (this);

        bool showingMoveHints = true;
        bool showingTileIndices = false;

        for (int i = toolbar->getNumItems(); --i >= 0;)
        {
            auto* c = toolbar->getItemComponent (i);

            if (c->getItemId() == ToolbarItemFactory::showHints)
                showingMoveHints = c->getToggleState();
            if (c->getItemId() == ToolbarItemFactory::showTileIndices)
                showingTileIndices = c->getToggleState();
        }

        boardGameComponent = new ReversiBoardGameComponent (*boardGame,
                                                            soundManager,
                                                            localisationManager,
                                                            showingMoveHints,
                                                            showingTileIndices);
        addAndMakeVisible (boardGameComponent);

        updateTrayIconTooltip();
    }
    else
    {
        boardGameComponent = nullptr;
    }
}

void MainComponent::changeGame (BoardGame* const newBoardGame)
{
    jassert (newBoardGame != nullptr);

    if (boardGame != newBoardGame)
    {
        boardGameComponent = nullptr;
        boardGame = newBoardGame;
        initialisBoardGameComponent();
        resized();
    }
}

void MainComponent::updateTrayIconTooltip()
{
   #if ! JUCE_IOS
    if (boardGame != nullptr)
        systemTrayIconComponent.updateTooltipFromBoardGame (*boardGame);
   #endif
}

//==============================================================================
void MainComponent::resized()
{
    if (toolbar != nullptr)
        toolbar->setBounds (0, 0, getWidth(), buttonHeight);

    if (boardGameComponent != nullptr)
        boardGameComponent->setBounds (margin,
                                       margin + buttonHeight,
                                       getWidth() - (margin * 2),
                                       getHeight() - (margin * 2) - buttonHeight);
}

void MainComponent::changeListenerCallback (ChangeBroadcaster* const source)
{
    if (source == settingsWindow)
        settingsWindow = nullptr;
}

void MainComponent::buttonClicked (Button* const button)
{
    if (auto* tic = dynamic_cast<ToolbarItemComponent*> (button))
    {
        switch (tic->getItemId())
        {
            case ToolbarItemFactory::open:
            {
                bool userCancelledOperation = false;
                auto* newBoardGame = ReversiGameStateHandler().restore (userCancelledOperation);

                if (newBoardGame != nullptr)
                {
                    changeGame (newBoardGame);
                }
                else if (! userCancelledOperation)
                {
                    AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                                 TRANS ("Error"),
                                                 TRANS ("Failed loading the game file! The file may be corrupt, or may be for a different game."),
                                                 "OK");
                }
            }
            break;

            case ToolbarItemFactory::save:
            {
                if (! ReversiGameStateHandler().save (boardGameComponent->getTileBoard()))
                {
                    AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                                 TRANS ("Error"),
                                                 TRANS ("Failed saving the game file."),
                                                 "OK");
                }
            }
            break;

            case ToolbarItemFactory::about:
                Process::openDocument ("http://www.jrlanglois.com/CV/jrlanglois-cv.pdf", {});
            break;

            case ToolbarItemFactory::changeGame:
            {
                PopupMenu popup;
                popup.addItem (1, TRANS ("Reversi (Regular)"));
                popup.addItem (2, TRANS ("Reversi (Small)"));
                popup.addItem (3, TRANS ("Reversi (Wide)"));

                BoardGame* newBoardGame = nullptr;

                switch (popup.show())
                {
                    case 1: newBoardGame = new StandardReversiBoardGame(); break;
                    case 2: newBoardGame = new SmallReversiBoardGame(); break;
                    case 3: newBoardGame = new WideReversiBoardGame(); break;

                    default: break;
                };

                if (newBoardGame != nullptr)
                    changeGame (newBoardGame);
            }
            break;

            case ToolbarItemFactory::showHints:
                boardGameComponent->setShowingMoveHints (button->getToggleState());
            break;

            case ToolbarItemFactory::showTileIndices:
                boardGameComponent->setShowingTileIndices (button->getToggleState());
            break;

            case ToolbarItemFactory::restart:
            {
                enum { resultYes = 1, resultNo = 2 };

                AlertWindow aw (TRANS ("Restart"),
                                TRANS ("Are you sure you want to restart the game?"),
                                AlertWindow::InfoIcon);

                aw.addButton (TRANS ("Yes"), resultYes, KeyPress (KeyPress::returnKey));
                aw.addButton (TRANS ("No"), resultNo, KeyPress (KeyPress::escapeKey));

                if (aw.runModalLoop() == resultYes)
                    boardGameComponent->reset();
            }
            break;

            case ToolbarItemFactory::settings:
            {
                if (settingsWindow != nullptr)
                {
                    settingsWindow->toFront (true);
                }
                else
                {
                    settingsWindow = new SettingsWindow (localisationManager,
                                                         soundManager);

                    settingsWindow->addChangeListener (this);
                }
            }
            break;

            default:
                jassertfalse;
            break;
        };
    }
}

void MainComponent::attemptedTileStateChange (BoardGame* const, const int, const bool)
{
    updateTrayIconTooltip();
}

void MainComponent::gameEnded (BoardGame* const)
{
    updateTrayIconTooltip();
}

void MainComponent::playerChanged (BoardGame* const, const bool)
{
    updateTrayIconTooltip();
}
