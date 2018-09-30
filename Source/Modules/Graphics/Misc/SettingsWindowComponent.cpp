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
SettingsWindowComponent::AlignedComponents::AlignedComponents (const String& text,
                                                               const String& t,
                                                               Component* r) :
    label (new Label()),
    comp (r),
    labelText (text),
    tooltip (t)
{
    jassert (label != nullptr && comp != nullptr);

    label->setJustificationType (Justification::centredRight);
    update();

    addAndMakeVisible (label);
    addAndMakeVisible (comp);
}

void SettingsWindowComponent::AlignedComponents::update()
{
    label->setText (TRANS (labelText) + ":", dontSendNotification);

    if (SettableTooltipClient* const stc = dynamic_cast<SettableTooltipClient*> (comp.get()))
        stc->setTooltip (TRANS (labelText));
}

void SettingsWindowComponent::AlignedComponents::resized()
{
    const int leftWidth = roundToInt (getWidth() * 0.33);

    label->setBounds (0, 0, leftWidth, getHeight());
    comp->setBounds (label->getRight(), 0, getWidth() - leftWidth, getHeight());
}

//==============================================================================
SettingsWindowComponent::SettingsWindowComponent (LocalisationManager& lm,
                                                  SoundManager& sm) :
    localisationManager (lm),
    soundManager (sm)
{
    localisationManager.addChangeListener (this);

    //Language settings:
    {
        ComboBox* cb = new ComboBox();
        cb->setComponentID (String (languageId));

        const StringArray names (localisationManager.getLanguageFileNames());

        for (int i = 0; i < names.size(); ++i)
            cb->addItem (names.strings.getUnchecked (i), i + 1);

        cb->setSelectedId (localisationManager.getCurrentFileIndex() + 1);

        addAlignedComps (NEEDS_TRANS ("Language"),
                         NEEDS_TRANS ("Lets you change the current language"),
                         cb);
    }

#if ! BGE_SILENT_AUDIO
    //Master volume:
    {
        addAlignedComps (NEEDS_TRANS ("Master Volume"),
                         NEEDS_TRANS ("Lets you change the current volume of all the sound, music and effects."),
                         createSlider (volumeMasterId, soundManager.getGain (SoundManager::categoryMaster)));
    }

    //Music volume:
    {
        addAlignedComps (NEEDS_TRANS ("Music Volume"),
                         NEEDS_TRANS ("Lets you change the current volume of the music."),
                         createSlider (volumeMusicId, soundManager.getGain (SoundManager::categoryMusic)));
    }

    //SFX volume:
    {
        addAlignedComps (NEEDS_TRANS ("Sound Effects Volume"),
                         NEEDS_TRANS ("Lets you change the current volume of the sound effects."),
                         createSlider (volumeSFXId, soundManager.getGain (SoundManager::categorySoundEffects)));
    }
#endif

    setSize (500,
             alignedComponents.size() * itemHeight
             + (alignedComponents.size() + 1) * itemMargin);
}

SettingsWindowComponent::~SettingsWindowComponent()
{
    localisationManager.removeChangeListener (this);
}

//==============================================================================
class SettingsSlider : public Slider
{
public:
    SettingsSlider (double initialValue) :
        Slider (Slider::LinearHorizontal, Slider::NoTextBox)
    {
        setPopupDisplayEnabled (true, true, nullptr);
        setRange (0.0, 1.0, 0.01);
        setValue (initialValue, dontSendNotification);
        setTextValueSuffix ("%");
    }

    String getTextFromValue (double value) override
    {
        return String (value * 100.0) + getTextValueSuffix();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsSlider)
};

Slider* SettingsWindowComponent::createSlider (const int componentId,
                                                     const double initialValue) const
{
    auto* slider = new SettingsSlider (initialValue);
    slider->setComponentID (String (componentId));
    return slider;
}

//==============================================================================
void SettingsWindowComponent::resized()
{
    for (int i = 0; i < alignedComponents.size(); ++i)
        alignedComponents.getUnchecked (i)->setBounds (itemMargin,
                                                       (i * itemHeight) + itemMargin,
                                                       getWidth() - doubleItemMargin,
                                                       itemHeight);
}

//==============================================================================
void SettingsWindowComponent::updateToTranslation()
{
    for (auto& comp : alignedComponents)
        comp->update();
}

void SettingsWindowComponent::changeListenerCallback (ChangeBroadcaster*)
{
    updateToTranslation();
}

void SettingsWindowComponent::sliderValueChanged (Slider* const slider)
{
    const auto value = (float) slider->getValue();

    switch (slider->getComponentID().getIntValue())
    {
        case volumeMasterId: soundManager.setGain (SoundManager::categoryMaster, value); break;
        case volumeMusicId: soundManager.setGain (SoundManager::categoryMusic, value); break;
        case volumeSFXId: soundManager.setGain (SoundManager::categorySoundEffects, value); break;

        default: jassertfalse; break; //Unknown id!
    };
}

void SettingsWindowComponent::comboBoxChanged (ComboBox* const comboBox)
{
    switch (comboBox->getComponentID().getIntValue())
    {
        case languageId:
            localisationManager.setLanguageFile (comboBox->getSelectedId() - 1);
        break;

        default:
            jassertfalse;
        break; //Unknown id!
    }
}
