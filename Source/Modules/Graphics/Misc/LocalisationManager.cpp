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
LocalisationManager::LocalisationManager() :
    currentFileIndex (-1)
{
    locateLanguagesFile();
    setLanguageFile (0);
}

//==============================================================================
void LocalisationManager::setLanguageFile (const int index)
{
    if (currentFileIndex != index)
    {
        if (TranslationFile* const tf = languageFiles[index])
        {
            currentFileIndex = index;
            juce::LocalisedStrings::setCurrentMappings (new juce::LocalisedStrings (tf->file, true));
            sendChangeMessage();
        }
    }
}

void LocalisationManager::locateLanguagesFile()
{
    juce::File languagesFolder (PathHelpers::getMediaFolder() + "Languages/");
    jassert (languagesFolder.exists() && languagesFolder.isDirectory()); //Something has gone horribly wrong with the path!

    juce::Array<juce::File> files;
    languagesFolder.findChildFiles (files, juce::File::findFiles, false, "*.txt");
    jassert (files.size() >= 3); //Missing default files!

    for (int i = 0; i < files.size(); ++i)
    {
        const juce::File& f = files.getUnchecked (i);
        const juce::LocalisedStrings ls (f, true);
        languageFiles.add (new TranslationFile (f, ls.getLanguageName().trim()));
    }
}

juce::StringArray LocalisationManager::getLanguageFileNames() const
{
    juce::StringArray names;

    for (int i = 0; i < languageFiles.size(); ++i)
        names.add (languageFiles.getUnchecked (i)->name);

    return names;
}