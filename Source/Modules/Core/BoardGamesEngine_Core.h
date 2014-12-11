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
#ifndef BOARD_GAMES_ENGINE_CORE_H
#define BOARD_GAMES_ENGINE_CORE_H

//==============================================================================
#include <algorithm>
#include <cassert>
#include <cmath>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

//=============================================================================
/** Config: BGE_SET_USING_NAMESPACE
    If your code uses lots of BoardGamesEngine code, then this will obviously save you a lot of typing!
*/
#ifndef BGE_SET_USING_NAMESPACE
 #define BGE_SET_USING_NAMESPACE 1
#endif

//=============================================================================
namespace bge
{
   #ifndef BGE_BOARD_GAME_H
    #include "Game/BoardGame.h"
   #endif //BOARD_GAME_H

   #ifndef BGE_REVERSI_BOARD_GAME_H
    #include "Game/ReversiBoardGame.h"
   #endif //BGE_REVERSI_BOARD_GAME_H

   #ifndef BGE_REVERSI_TILE_BOARD_H
    #include "Game/ReversiTileBoard.h"
   #endif //BGE_REVERSI_TILE_BOARD_H

   #ifndef BGE_TILE_H
    #include "Game/Tile.h"
   #endif //BGE_TILE_H

   #ifndef BGE_TILE_BOARD_H
    #include "Game/TileBoard.h"
   #endif //BGE_TILE_BOARD_H

   #ifndef BGE_COMPILER_SUPPORT_H
    #include "Misc/CompilerSupport.h"
   #endif //BGE_COMPILER_SUPPORT_H

   #ifndef BGE_CORE_MACROS_H
    #include "Misc/Macros.h"
   #endif //BGE_CORE_MACROS_H

   #ifndef BGE_CORE_MATH_H
    #include "Misc/Math.h"
   #endif //BGE_CORE_MATH_H

   #ifndef BGE_CORE_MISC_H
    #include "Misc/Misc.h"
   #endif //BGE_CORE_MISC_H
}

#endif //BOARD_GAMES_ENGINE_CORE_H