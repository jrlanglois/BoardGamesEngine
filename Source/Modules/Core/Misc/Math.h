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
#ifndef BGE_CORE_MATH_H
#define BGE_CORE_MATH_H

#include "Macros.h"

/** Find out if the given integer value is odd.

    @returns True if the value is odd, false if it is even.
*/
template<typename IntegerType>
inline bool isValueOdd (IntegerType value) noexcept
{
    return (value % 2) == 0;
}

/** Returns true if a value is at least zero, and also below a specified upper limit. */
template<typename Type>
inline bool isPositiveAndBelow (Type valueToTest, Type upperLimit) noexcept
{
    rassert (Type() <= upperLimit); //Doesn't make sense to call this if the upper limit is itself below zero
    return Type() <= valueToTest && valueToTest < upperLimit;
}

/** Returns true if a value is at least zero, and also below a specified upper limit. */
template<>
inline bool isPositiveAndBelow (int valueToTest, int upperLimit) noexcept
{
    rassert (upperLimit >= 0); //Doesn't make sense to call this if the upper limit is itself below zero
    return static_cast<unsigned int> (valueToTest) < static_cast<unsigned int> (upperLimit);
}

//==============================================================================
/** Calculate the index of the tile at the given column and row. */
inline int getIndexOfTile (int column, int row, int numColumns) noexcept
{
    return row + (column * numColumns);
}

/** Calculate the row of the tile at the given index. */
template<typename Type>
inline Type findRowOfTile (Type index, Type numRows) noexcept
{
    return static_cast<Type> (std::floor (static_cast<double> (index) / static_cast<double> (numRows)));
}

/** Calculate the column of the tile at the given index. */
template<typename Type>
inline Type findColumnOfTile (Type index, Type numColumns) noexcept
{
    return index % numColumns;
}

#endif //BGE_CORE_MATH_H