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
#ifndef BGE_SOUND_MANAGER_H
#define BGE_SOUND_MANAGER_H

/**

*/
class SoundManager
{
public:
    /** Constructor */
    SoundManager();

    /** Destructor */
    ~SoundManager();

    //==============================================================================
    void playSong();

    void stopSongPlayback();

    //==============================================================================
    enum SoundCategory
    {
        categoryMaster = 0,
        categoryMusic,
        categorySoundEffects
    };

    void setGain (SoundCategory category, float gain);
    float getGain (SoundCategory category) const;

    //==============================================================================
    /** The list of sound effects that can be played

        @note The values are to represent their index
    */
    enum SoundEffect
    {
        tileChanged = 0,
        tileFailedChange
    };

    void playSound (SoundEffect soundEffect);

private:
    //==============================================================================
    AudioFormatManager audioFormatManager;
    AudioDeviceManager deviceManager;
    AudioSourcePlayer audioSourcePlayer;
    MixerAudioSource mixerAudioSource;

    class Sound;
    ScopedPointer<Sound> music;
    OwnedArray<Sound> soundEffects;

    //==============================================================================
    void setupSoundLibrary();
    void addSound (const String& sourcePath,
                   const String& fileName);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundManager)
};

#endif //BGE_SOUND_MANAGER_H