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
#ifndef BGE_CORE_MISC_H
#define BGE_CORE_MISC_H

static const std::string emptyString;

/** A simple function that allows appending data types to a std::string. */
template<typename Type>
static void append (std::string& destination, Type source)
{
    std::ostringstream s;
    s << source;
    destination.append (s.str());
}

/** A simple function that allows appending a container of a particular data type to a std::string.

    This is only meant to be used with containers that support operator[],
    and provide the number of elements it contains with the method size().
*/
template<template<class> class ContainerType, typename Type>
static void append (std::string& destination, const ContainerType<Type>& source)
{
    std::ostringstream s;

    for (int i = 0; i < source.size(); ++i) 
        s << source[i];

    destination.append (s.str());
}

/** Attempts converting a std::string to a desired value type. */
template<typename Type>
static Type toValue (const std::string& source)
{
    std::istringstream iss (source);
    Type result = 0;
    iss >> result;

    return result;
}

template<typename Type>
static Type toValue (char source)
{
    std::stringstream ss;
    ss << source;

    std::string result;
    ss >> result;

    return toValue<Type> (result);
}

#endif //BGE_CORE_MISC_H
