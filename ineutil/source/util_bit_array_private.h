/*-*-c++-*-*************************************************************************************************************
* Copyright 2016 - 2022 Inesonic, LLC.
* 
* This file is licensed under two licenses.
*
* Inesonic Commercial License, Version 1:
*   All rights reserved.  Inesonic, LLC retains all rights to this software, including the right to relicense the
*   software in source or binary formats under different terms.  Unauthorized use under the terms of this license is
*   strictly prohibited.
*
* GNU Public License, Version 2:
*   This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
*   License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
*   version.
*   
*   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
*   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
*   details.
*   
*   You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
*   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************************************************************//**
* \file
*
* This header defines the Util::BitArray::Private class.
***********************************************************************************************************************/

/* .. sphinx-project ineutil */

#ifndef UTIL_BIT_ARRAY_PRIVATE_H
#define UTIL_BIT_ARRAY_PRIVATE_H

#include <QSharedDataPointer>
#include <QSharedData>

#include <cstdint>

#include "util_common.h"
#include "util_bit_array.h"

namespace Util {
    /**
     * Private implementation class that can be used to maintain a searchable array of bits.
     */
    class UTIL_PUBLIC_API BitArray::Private:public QSharedData {
        public:
            /**
             * Default constructor.
             */
            Private();

            /**
             * Constructor, constructs an array of bits of a fixed sized.
             *
             * \param[in] numberBits The desired initial length of the array, in bits.
             *
             * \param[in] value      The value to assign to all the bits in the array.
             */
            Private(BitArray::Index numberBits, bool value = false);

            /**
             * Constructor, constructs an array of bits from an array of bytes.
             *
             * \param[in] rawData    Raw data to use to create an array of bits.
             *
             * \param[in] numberBits The raw data array length, in bits.
             */
            Private(const bool* rawData, Index numberBits);

            /**
             * Constructor, constructs an array of bits from an array of bytes.
             *
             * \param[in] rawData    Raw data to use to create an array of bits.  Bits will be ordered LSB first.
             *
             * \param[in] numberBits The raw data array length, in bits.
             */
            Private(const void* rawData, Index numberBits);

            /**
             * Copy constructor.
             *
             * \param[in] other The instance to be deep copied.
             */
            Private(const BitArray::Private& other);

            ~Private();

            /**
             * Method you can use to determine the size of the array, in bits.
             *
             * \return Returns the array size, in bits.
             */
            Index size() const;

            /**
             * Method you can use to clear the array contents.
             */
            void clear();

            /**
             * Method you can use to resize the bit array.
             *
             * \param[in] newLength The new bit array length, in bits.
             */
            void resize(Index newLength);

            /**
             * Method you can use to set a single bit.
             *
             * \param[in] bitIndex The zero based bit index of the bit to be set.  The bit array will be extended, if
             *                     needed.
             *
             * \param[in] nowSet   If true, the bit will be set.  if false, the bit will be cleared.
             */
            void setBit(Index bitIndex, bool nowSet = true);

            /**
             * Method you can use to clear a single bit.
             *
             * \param[in] bitIndex   The zero based bit index of the bit to be set.  The bit array will be extended,
             *                       if needed.
             *
             * \param[in] nowCleared If true, the bit will be cleared.  if false, the bit will be set.
             */
            void clearBit(Index bitIndex, bool nowCleared = true);

            /**
             * Method you can use to set a range of bits.  The array will be extended, if needed.
             *
             * \param[in] startingIndex The starting index of the range of bits to set.  Value is inclusive.
             *
             * \param[in] endingIndex   The ending index of the range of bits to set.  Value is inclusive.
             *
             * \param[in] nowSet        If true, bits will be set.  if false, bits will be cleared.
             */
            void setBits(Index startingIndex, Index endingIndex, bool nowSet = true);

            /**
             * Method you can use to clear a range of bits.  The array will be extended, if needed.
             *
             * \param[in] startingIndex The starting index of the range of bits to clear.  Value is inclusive.
             *
             * \param[in] endingIndex   The ending index of the range of bits to clear.  Value is inclusive.
             *
             * \param[in] nowCleared    If true, bits will be cleared.  if false, bits will be set.
             */
            void clearBits(Index startingIndex, Index endingIndex, bool nowCleared = true);

            /**
             * Method you can use to determine if a bit is set.
             *
             * \param[in] index The index of the bit to obtain.
             *
             * \return Returns true if the bit is set, returns false if the bit is cleared.
             */
            bool isSet(Index index) const;

            /**
             * Method you can use to determine if a bit is cleared.
             *
             * \param[in] index The index of the bit to obtain.
             *
             * \return Returns true if the bit is cleared, returns false if the bit is set.
             */
            bool isClear(Index index) const;

            /**
             * Method you can use to locate the first set bit in the array.
             *
             * \return Returns the index of the first set bit in the array.  A value of
             *         \ref Util::BitArray::invalidIndex is returned if no set bits exist in the array.
             */
            Index firstSetBit() const;

            /**
             * Method you can use to locate the first cleared bit in the array.
             *
             * \return Returns the index of the first cleared bit in the array.  A value of
             *         \ref Util::BitArray::invalidIndex is returned if no cleared bits exist in the array.
             */
            Index firstClearedBit() const;

            /**
             * Method you can use to locate the first set bit in the array at or after a specific index.
             *
             * \param[in] startingIndex The starting index to perform the search at.
             *
             * \return Returns the index of the first set bit in the array.  A value of
             *         \ref Util::BitArray::invalidIndex is returned if no set bits exist in the array.
             */
            Index firstSetBit(Index startingIndex) const;

            /**
             * Method you can use to locate the first cleared bit in the array at or after a specific index.
             *
             * \param[in] startingIndex The starting index to perform the search at.
             *
             * \return Returns the index of the first cleared bit in the array.  A value of
             *         \ref Util::BitArray::invalidIndex is returned if no cleared bits exist in the array.
             */
            Index firstClearedBit(Index startingIndex) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the instances are equal.  Returns false if the instances are different.
             */
            bool operator==(const BitArray::Private& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the instances are different.  Returns false if the instances are equal.
             */
            bool operator!=(const BitArray::Private& other) const;

        private:
            /**
             * The allocation unit used by the bit array.
             */
            typedef std::uint64_t AllocationUnit;

            /**
             * The allocation unit size, in bits.
             */
            static constexpr unsigned allocationUnitSize = 64;

            /**
             * A value representing an all 1's allocation unit.
             */
            static constexpr AllocationUnit allOnes = static_cast<AllocationUnit>(-1);

            /**
             * Method that calculates the data length required to store a specified number of bits.
             *
             * \param[in] bitLength the required data store length, in bits.
             *
             * \return Returns the data store length in allocation units.
             */
            static unsigned long allocationDataSize(BitArray::Index bitLength);

            /**
             * Method that checks a bit index and resizes the array, if needed.
             *
             * \param[in] index The index to be checked.
             */
            void resizeToFit(BitArray::Index index);

            /**
             * Data contained in the class.
             */
            AllocationUnit* data;

            /**
             * The size of the data array, in allocation units.
             */
            unsigned long dataLength;

            /**
             * The size of the bit array, in bits.
             */
            unsigned long bitLength;
    };
}

#endif
