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
#ifndef BGE_SETTINGS_WINDOW_COMPONENT_H
#define BGE_SETTINGS_WINDOW_COMPONENT_H

#include "SoundManager.h"

class SettingsWindowComponent : public juce::Component,
                                private juce::ChangeListener,
                                private juce::Slider::Listener,
                                private juce::ComboBox::Listener
{
public:
    SettingsWindowComponent (LocalisationManager& lm,
                             SoundManager& sm);

    ~SettingsWindowComponent();

    //==============================================================================
    /** @internal */
    void resized() override;

private:
    //==============================================================================
    LocalisationManager& localisationManager;
    SoundManager& soundManager;

    //==============================================================================1
    enum
    {
        itemHeight = 32,
        itemMargin = 8,
        doubleItemMargin = itemMargin * 2
    };

    //==============================================================================
    class AlignedComponents : public juce::Component
    {
    public:
        AlignedComponents (const juce::String& text,
                           const juce::String& t,
                           juce::Component* r);

        //==============================================================================
        void update();

        //==============================================================================
        /** @internal */
        void resized() override;

        //==============================================================================
        juce::ScopedPointer<juce::Label> label;
        juce::ScopedPointer<juce::Component> comp;

    private:
        //==============================================================================
        const juce::String labelText; //< Untranslated
        const juce::String tooltip; //< Untranslated

        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlignedComponents)
    };

    juce::OwnedArray<AlignedComponents> alignedComponents;

    //==============================================================================
    enum ComponentIds
    {
        languageId      = 875612,
        volumeMasterId  = 574219,
        volumeMusicId   = 458144,
        volumeSFXId     = 355573
    };

    //==============================================================================
    juce::Slider* createSlider (int componentId, double initialValue) const;

    template<typename ComponentType>
    void addAlignedComps (const juce::String& labelText,
                          const juce::String& tooltip,
                          ComponentType* comp)
    {
        jassert (comp != nullptr);
        comp->addListener (this);
        addAndMakeVisible (alignedComponents.add (new AlignedComponents (labelText, tooltip, comp)));
    }

    //==============================================================================
    void updateToTranslation();

    //==============================================================================
    /** @internal */
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    /** @internal */
    void sliderValueChanged (juce::Slider* slider) override;
    /** @internal */
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;

    //==============================================================================
    SettingsWindowComponent() JUCE_DELETED_FUNCTION;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsWindowComponent)
};

//==============================================================================
class SettingsWindow : public juce::DocumentWindow,
                       public juce::ChangeBroadcaster,
                       private juce::ChangeListener
{
public:
    SettingsWindow (LocalisationManager& lm,
                    SoundManager& sm) :
        juce::DocumentWindow (TRANS ("Settings"),
                              juce::Colour (71, 115, 187),
                              juce::DocumentWindow::closeButton),
        localisationManager (lm)
    {
        localisationManager.addChangeListener (this);
        setUsingNativeTitleBar (true);
        setContentOwned (new SettingsWindowComponent (localisationManager, sm), true);
        setResizable (false, false);

        centreWithSize (getWidth(), getHeight());
        setVisible (true);
    }

    ~SettingsWindow()
    {
        localisationManager.removeChangeListener (this);
    }

    //==============================================================================
    /** @internal */
    void closeButtonPressed() override
    {
        sendChangeMessage();
    }

private:
    LocalisationManager& localisationManager;

    //==============================================================================
    /** @internal */
    void changeListenerCallback (juce::ChangeBroadcaster*)
    {
        setName (TRANS ("Settings"));
    }

    //==============================================================================
    SettingsWindow() JUCE_DELETED_FUNCTION;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsWindow)
};

#endif //BGE_SETTINGS_WINDOW_COMPONENT_H