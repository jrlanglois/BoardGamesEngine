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
#ifndef BOARD_GAMES_ENGINE_GRAPHICS_H
#define BOARD_GAMES_ENGINE_GRAPHICS_H

#ifndef BOARD_GAMES_ENGINE_GUI_BASICS_H
 #include "../GUIBasics/BoardGamesEngine_GUIBasics.h"
#endif //BOARD_GAMES_ENGINE_GUI_BASICS_H

//=============================================================================
/** Config: BGE_SILENT_AUDIO
    Disables all audio playback from all instances of a SoundManager (on all platforms).

    @note This is a JUCE/Introjucer module configuration flag, but may
          of course be externally and explicitly enabled or disabled.
*/
#ifndef BGE_SILENT_AUDIO
 #define BGE_SILENT_AUDIO 0
#endif

//=============================================================================
namespace bge
{
    //=============================================================================
    namespace PathHelpers
    {
        inline static String getReversiFolder()
        {
           #if JUCE_IOS
            return File::getSpecialLocation (File::currentExecutableFile)
                    .getFullPathName()
                    .upToFirstOccurrenceOf ("BoardGamesEngine.app/", true, true);
           #else
            return File::getSpecialLocation (File::currentExecutableFile)
                    .getFullPathName()
                    .replace ("\\", "/", true) //N.B.: Needed for Windows paths
                    .upToFirstOccurrenceOf ("BoardGamesEngine/", true, true);
           #endif
        }

        inline static String getMediaFolder()
        {
            return getReversiFolder() + "Media/";
        }
    }

    //=============================================================================
   #ifndef BGE_BOARD_GAME_COMPONENT_H
    #include "Game/BoardGameComponent.h"
   #endif //BGE_BOARD_GAME_COMPONENT_H

   #ifndef BGE_REVERSI_BOARD_GAME_COMPONENT_H
    #include "Game/ReversiBoardGameComponent.h"
   #endif //BGE_REVERSI_BOARD_GAME_COMPONENT_H

   #ifndef BGE_REVERSI_TILE_BOARD_COMPONENT_H
    #include "Game/ReversiTileBoardComponent.h"
   #endif //BGE_REVERSI_TILE_BOARD_COMPONENT_H

   #ifndef BGE_TILE_BOARD_COMPONENT_H
    #include "Game/TileBoardComponent.h"
   #endif //BGE_TILE_BOARD_COMPONENT_H

   #ifndef BGE_FONTS_H
    #include "Misc/Fonts.h"
   #endif //BGE_FONTS_H

   #ifndef BGE_LOCALISATION_MANAGER_H
    #include "Misc/LocalisationManager.h"
   #endif //LOCALISATION_MANAGER_H

   #ifndef BGE_SETTINGS_WINDOW_COMPONENT_H
    #include "Misc/SettingsWindowComponent.h"
   #endif //SETTINGS_WINDOW_COMPONENT_H

   #ifndef BGE_LOOK_AND_FEEL_H
    #include "Misc/BGELookAndFeel.h"
   #endif //BGE_LOOK_AND_FEEL_H

  #ifndef JUCE_IOS
   #ifndef BGE_SYSTEM_TRAY_ICON_COMPONENT_H
    #include "Misc/BGESystemTrayIconComponent.h"
   #endif //BGE_SYSTEM_TRAY_ICON_COMPONENT_H
  #endif //JUCE_IOS

   #ifndef BGE_SOUND_MANAGER_H
    #include "Misc/SoundManager.h"
   #endif //SOUND_MANAGER_H

   #ifndef BGE_TOOLBAR_ICONS_H
    #include "Misc/ToolbarIcons.h"
   #endif //TOOLBAR_ICONS_H
}

#endif //BOARD_GAMES_ENGINE_GRAPHICS_H
