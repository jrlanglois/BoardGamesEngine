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
#ifndef BOARD_GAMES_ENGINE_MASTER_H
#define BOARD_GAMES_ENGINE_MASTER_H

#ifndef BOARD_GAMES_ENGINE_CORE_H
 #include "Core/BoardGamesEngine_Core.h"
#endif //BOARD_GAMES_ENGINE_CORE_H

#ifndef BOARD_GAMES_ENGINE_GUI_BASICS_H
 #include "GUIBasics/BoardGamesEngine_GUIBasics.h"
#endif //BOARD_GAMES_ENGINE_GUI_BASICS_H

#ifndef BOARD_GAMES_ENGINE_GRAPHICS_H
 #include "Graphics/BoardGamesEngine_Graphics.h"
#endif //BOARD_GAMES_ENGINE_GRAPHICS_H

#if BGE_SET_USING_NAMESPACE
 using namespace bge;
#endif

#endif //BOARD_GAMES_ENGINE_MASTER_H