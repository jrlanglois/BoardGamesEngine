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
#ifndef BGE_LOCALISATION_MANAGER_H
#define BGE_LOCALISATION_MANAGER_H

class LocalisationManager : public juce::ChangeBroadcaster
{
public:
    LocalisationManager();

    //==============================================================================
    int getCurrentFileIndex() const noexcept { return currentFileIndex; }

    //==============================================================================
    class TranslationFile
    {
    public:
        TranslationFile (const juce::File& f,
                         const juce::String& n) :
            file (f),
            name (n)
        {
        }

        const juce::File file;
        const juce::String name;

    private:
        TranslationFile() JUCE_DELETED_FUNCTION;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TranslationFile)
    };

    const juce::OwnedArray<TranslationFile>& getLanguageFiles() const noexcept { return languageFiles; }

    juce::StringArray getLanguageFileNames() const;

    //==============================================================================
    void setLanguageFile (int index);

private:
    //==============================================================================
    int currentFileIndex;

    juce::OwnedArray<TranslationFile> languageFiles;

    //==============================================================================
    void locateLanguagesFile();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LocalisationManager)
};

#endif //BGE_LOCALISATION_MANAGER_H