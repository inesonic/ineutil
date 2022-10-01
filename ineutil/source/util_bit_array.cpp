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
* This file implements the \ref Util::BitArray class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedDataPointer>
#include <QSharedData>

#include <cstdint>
#include <cassert>

#include "util_bit_functions.h"
#include "util_bit_array_private.h"
#include "util_bit_array.h"

namespace Util {
    BitArray::BitArray() : impl(new BitArray::Private) {}


    BitArray::BitArray(BitArray::Index numberBits, bool value) : impl(new BitArray::Private(numberBits, value)) {}


    BitArray::BitArray(
            const bool*     rawData,
            BitArray::Index numberBits
        ) : impl(
             new BitArray::Private(
                 rawData,
                 numberBits
             )
        ) {}


    BitArray::BitArray(
            const std::uint8_t* rawData,
            BitArray::Index     numberBits
        ) : impl(
            new BitArray::Private(
                rawData,
                numberBits
            )
        ) {}


    BitArray::BitArray(
            const std::uint16_t* rawData,
            BitArray::Index      numberBits
        ) : impl(
            new BitArray::Private(
                rawData,
                numberBits
            )
        ) {}


    BitArray::BitArray(
            const std::uint32_t* rawData,
            BitArray::Index      numberBits
        ) : impl(
            new BitArray::Private(
                rawData,
                numberBits
            )
        ) {}


    BitArray::BitArray(
            const std::uint64_t* rawData,
            BitArray::Index      numberBits
        ) : impl(
            new BitArray::Private(
                rawData,
                numberBits
            )
        ) {}


    BitArray::BitArray(const BitArray& other) {
        impl = other.impl;
    }


    BitArray::~BitArray() {}


    BitArray::Index BitArray::size() const {
        return impl->size();
    }


    BitArray::Index BitArray::count() const {
        return impl->size();
    }


    BitArray::Index BitArray::length() const {
        return impl->size();
    }


    void BitArray::clear() {
        impl->clear();
    }


    void BitArray::resize(Index newLength) {
        impl->resize(newLength);
    }


    void BitArray::setBit(BitArray::Index bitIndex, bool nowSet) {
        impl->setBit(bitIndex, nowSet);
    }


    void BitArray::clearBit(BitArray::Index bitIndex, bool nowCleared) {
        impl->clearBit(bitIndex, nowCleared);
    }


    void BitArray::setBits(BitArray::Index startingIndex, BitArray::Index endingIndex, bool nowSet) {
        impl->setBits(startingIndex, endingIndex, nowSet);
    }


    void BitArray::clearBits(BitArray::Index startingIndex, BitArray::Index endingIndex, bool nowCleared) {
        impl->clearBits(startingIndex, endingIndex, nowCleared);
    }


    bool BitArray::isSet(BitArray::Index index) const {
        return impl->isSet(index);
    }


    bool BitArray::isClear(BitArray::Index index) const {
        return impl->isClear(index);
    }


    BitArray::Index BitArray::firstSetBit() const {
        return impl->firstSetBit();
    }


    BitArray::Index BitArray::firstClearedBit() const {
        return impl->firstClearedBit();
    }


    BitArray::Index BitArray::firstSetBit(BitArray::Index startingIndex) const {
        return impl->firstSetBit(startingIndex);
    }


    BitArray::Index BitArray::firstClearedBit(BitArray::Index startingIndex) const {
        return impl->firstClearedBit(startingIndex);
    }


    BitArray& BitArray::operator=(const BitArray& other) {
        impl = other.impl;
        return *this;
    }


    bool BitArray::operator==(const BitArray& other) const {
        return impl == other.impl || *impl == *other.impl;
    }


    bool BitArray::operator!=(const BitArray& other) const {
        return impl != other.impl && *impl != *other.impl;
    }
}
