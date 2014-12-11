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
class SoundManager::Sound : private juce::ChangeListener
{
public:
    Sound (juce::AudioFormatManager& audioFormatManager,
           const juce::File& path,
           bool shouldLoop)
    {
        reader = audioFormatManager.createReaderFor (path.createInputStream());

        jassert (reader != nullptr); //Is there something wrong with the file path?
        if (reader != nullptr)
        {
            readerSource = new juce::AudioFormatReaderSource (reader, true);
            readerSource->setLooping (shouldLoop);

            transport = new juce::AudioTransportSource();
            transport->setSource (readerSource);
            transport->setLooping (shouldLoop);

            if (! shouldLoop)
                transport->addChangeListener (this);
        }
    }

    ~Sound()
    {
        transport = nullptr;
        readerSource = nullptr;
        reader = nullptr;
    }

    //==============================================================================
    void play()
    {
        if (transport != nullptr)
        {
            if (! transport->isLooping())
                reset();

            transport->start();
        }
    }

    void stop()
    {
        if (transport != nullptr)
            transport->stop();
    }

    void setGain (float gain)
    {
        if (transport != nullptr)
            transport->setGain (gain);
    }

    //==============================================================================
    juce::AudioTransportSource* getTransport() const noexcept { return transport; }

private:
    //==============================================================================
    juce::AudioFormatReader* reader;
    juce::ScopedPointer<juce::AudioFormatReaderSource> readerSource;
    juce::ScopedPointer<juce::AudioTransportSource> transport;

    //==============================================================================
    void reset()
    {
        if (transport != nullptr)
            transport->setPosition (0.0);
    }

    void changeListenerCallback (juce::ChangeBroadcaster*) override
    {
        reset();
    }

    //==============================================================================
    Sound() JUCE_DELETED_FUNCTION;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sound)
};

//==============================================================================
SoundManager::SoundManager()
{
   #if ! BGE_SILENT_AUDIO
    audioFormatManager.registerBasicFormats();

    setupSoundLibrary();

    audioSourcePlayer.setSource (&mixerAudioSource);
    deviceManager.addAudioCallback (&audioSourcePlayer);
    deviceManager.initialiseWithDefaultDevices (0, 2);

    playSong();
   #endif
}

SoundManager::~SoundManager()
{
    deviceManager.removeAudioCallback (&audioSourcePlayer);
    audioSourcePlayer.setSource (nullptr);
    mixerAudioSource.removeAllInputs();
}

//==============================================================================
void SoundManager::setupSoundLibrary()
{
    const juce::String appLoc (PathHelpers::getMediaFolder());
    music = new Sound (audioFormatManager, juce::File (appLoc + "Music/bensound-theelevatorbossanova.mp3"), true);
    music->getTransport()->setGain (0.75f);
    mixerAudioSource.addInputSource (music->getTransport(), false);

    addSound (appLoc, "SFX/Tile_Change.wav");
    addSound (appLoc, "SFX/Tile_FailedChange.wav");
}

void SoundManager::addSound (const juce::String& sourcePath,
                             const juce::String& fileName)
{
    Sound* const s = soundEffects.add (new Sound (audioFormatManager, juce::File (sourcePath + fileName), false));
    mixerAudioSource.addInputSource (s->getTransport(), false);
}

void SoundManager::playSound (const SoundEffect soundEffect)
{
    if (Sound* const sound = soundEffects[(int) soundEffect])
        sound->play();
}

//==============================================================================
void SoundManager::playSong()
{
    if (music != nullptr)
        music->play();
}

void SoundManager::stopSongPlayback()
{
    if (music != nullptr)
        music->stop();
}

//==============================================================================
void SoundManager::setGain (const SoundCategory category, const float gain)
{
   #if ! BGE_SILENT_AUDIO
    switch (category)
    {
        case categoryMaster: audioSourcePlayer.setGain (gain); break;
        case categoryMusic: music->setGain (gain); break;

        case categorySoundEffects:
            for (int i = soundEffects.size(); --i >= 0;)
                soundEffects.getUnchecked (i)->setGain (gain);
        break;

        default: jassertfalse; break;
    };
   #else
    (void) category;
    (void) gain;
   #endif
}

float SoundManager::getGain (const SoundCategory category) const
{
   #if ! BGE_SILENT_AUDIO
    switch (category)
    {
        case categoryMaster:
            return audioSourcePlayer.getGain();
        break;

        case categoryMusic:
            if (music != nullptr)
                if (juce::AudioTransportSource* const ats = music->getTransport())
                    return ats->getGain();
        break;

        case categorySoundEffects:
            if (Sound* const sound = soundEffects.getFirst())
                if (juce::AudioTransportSource* const ats = sound->getTransport())
                    return ats->getGain();
        break;

        default:
            jassertfalse;
        break;
    };
   #else
    (void) category;
   #endif

    return 0.0f;
}