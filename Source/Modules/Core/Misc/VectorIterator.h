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
#ifndef BGE_VECTOR_ITERATOR_H
#define BGE_VECTOR_ITERATOR_H

/** A truly user-friendly std::vector forward iterator

    @warning Altering the given std::vector while a VectorIterator
             is using it is not a safe operation.
*/
template<class Type>
class VectorIterator
{
public:
    /** Creates a forward iterator for the provided std::vector */
    VectorIterator (std::vector<Type>& vectorToIterate) :
        vecToIterate (vectorToIterate),
        currentIndex (0)
    {
    }

    //==============================================================================
    /** Retrieves a copy of the next result from the std::vector.

        @param[out] result
        @returns            True if an item was found, or false if the iterator has reached
                            the end of the std::vector
    */
    bool getNextItem (Type& result)
    {
        if (isInRange (currentIndex, vecToIterate))
        {
            result = vecToIterate[currentIndex];
            ++currentIndex;
            return true;
        }

        return false;
    }

    void setIndex (size_t newStartIndex)
    {
        rassert (isInRange (newStartIndex, vecToIterate));
        currentIndex = newStartIndex;
    }

    void reset()
    {
        currentIndex = 0;
    }

private:
    //==============================================================================
    std::vector<Type>& vecToIterate;
    size_t currentIndex;

    //==============================================================================
    static bool isInRange (size_t desiredIndex, std::vector<Type>& vecToIterate)
    {
        return isPositiveAndBelow (desiredIndex, vecToIterate.size());
    }

    //==============================================================================
    VectorIterator() BGE_DELETED_FUNCTION;
    BGE_DECLARE_NON_COPYABLE (VectorIterator)
    BGE_PREVENT_HEAP_ALLOCATION
};

#endif //BGE_VECTOR_ITERATOR_H