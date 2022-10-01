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
* This file implements the \ref Util::BitSet class.
***********************************************************************************************************************/

#include <QString>
#include <QHash>
#include <QList>

#include <cstdint>
#include <algorithm>

#include "util_bit_functions.h"
#include "util_hash_functions.h"
#include "util_bit_set.h"

/***********************************************************************************************************************
 * Util::BitSet
 */

namespace Util {
    BitSet::BitSet(BitSet::BitNameHash* bitHash) {
        bitNames = bitHash;
        bitArray.clear();
    }


    BitSet::BitSet() {
        bitNames = Q_NULLPTR;
        bitArray.clear();
    }


    BitSet::BitSet(const BitSet& other) {
        bitNames = other.bitNames;
        bitArray = other.bitArray;
    }


    BitSet::~BitSet() {}


    void BitSet::clear() {
        bitArray.clear();
    }


    bool BitSet::bitDefined(const QString& bitName) {
        bool result;

        if (bitNames == Q_NULLPTR) {
            result = false;
        } else {
            result = bitNames->contains(bitName);
        }

        return result;
    }


    bool BitSet::setBit(const QString& bitName, bool isSet) {
        bool success = false;

        if (bitNames != Q_NULLPTR) {
            unsigned bitIndex = bitNames->value(bitName, static_cast<unsigned>(-1));

            if (bitIndex != static_cast<unsigned>(-1)) {
                unsigned wordIndex = bitIndex / bitsPerEntry;
                unsigned bitOffset = bitIndex % bitsPerEntry;

                while (wordIndex >= static_cast<unsigned>(bitArray.size())) {
                    bitArray.append(0);
                }

                ArrayType mask = static_cast<ArrayType>(1) << bitOffset;

                if (isSet) {
                    bitArray[wordIndex] |= mask;
                } else {
                    bitArray[wordIndex] &= ~mask;
                }

                success = true;
            }
        }

        return success;
    }


    bool BitSet::clearBit(const QString& bitName, bool isClear) {
        return setBit(bitName, !isClear);
    }


    unsigned BitSet::numberSetBits() const {
        unsigned countBits   = 0;
        unsigned numberWords = static_cast<unsigned>(bitArray.size());
        for (unsigned index=0 ; index<numberWords ; ++index) {
            ArrayType word = bitArray.at(index);

            if (bitsPerEntry == 32) {
                countBits += numberOnes32(word);
            } else if (bitsPerEntry == 64) {
                countBits += numberOnes64(word);
            } else {
                assert(false);
            }
        }

        return countBits;
    }


    unsigned BitSet::numberClearedBits() const {
        unsigned numberSet       = numberSetBits();
        unsigned totalNumberBits = static_cast<unsigned>(bitNames->size());

        assert(totalNumberBits >= numberSet);

        return totalNumberBits - numberSet;
    }


    bool BitSet::isSet(const QString& bitName) const {
        bool result = false;

        if (bitNames != Q_NULLPTR) {
            unsigned bitIndex = bitNames->value(bitName, static_cast<unsigned>(-1));

            if (bitIndex != static_cast<unsigned>(-1)) {
                unsigned wordIndex = bitIndex / bitsPerEntry;

                if (wordIndex < static_cast<unsigned>(bitArray.size())) {
                    unsigned  bitOffset = bitIndex % bitsPerEntry;
                    ArrayType mask      = static_cast<ArrayType>(1) << bitOffset;

                    result = (bitArray.at(wordIndex) & mask) != 0;
                }
            }
        }

        return result;
    }


    bool BitSet::isCleared(const QString& bitName) const {
        return !isSet(bitName);
    }


    bool BitSet::tracksSameBitsAs(const BitSet& other) const {
        return bitNames == other.bitNames;
    }


    BitSet BitSet::intersectionBits(const BitSet& other) const {
        Q_ASSERT(bitNames == other.bitNames);

        unsigned numberWords       = static_cast<unsigned>(bitArray.size());
        unsigned otherNumberWords  = static_cast<unsigned>(other.bitArray.size());
        unsigned numberCommonWords = std::min(numberWords, otherNumberWords);

        BitSet combined;
        combined.bitNames = bitNames;

        unsigned index;
        for (index=0 ; index<numberCommonWords ; ++index) {
            combined.bitArray.append(bitArray.at(index) & other.bitArray.at(index));
        }

        return combined;
    }


    BitSet BitSet::unionBits(const BitSet& other) const {
        Q_ASSERT(bitNames == other.bitNames);

        unsigned numberWords       = static_cast<unsigned>(bitArray.size());
        unsigned otherNumberWords  = static_cast<unsigned>(other.bitArray.size());
        unsigned numberCommonWords = std::min(numberWords, otherNumberWords);

        BitSet combined;
        combined.bitNames = bitNames;

        unsigned index;
        for (index=0 ; index<numberCommonWords ; ++index) {
            combined.bitArray.append(bitArray.at(index) | other.bitArray.at(index));
        }

        if (numberWords < otherNumberWords) {
            do {
                combined.bitArray.append(other.bitArray.at(index));
                ++index;
            }
            while (index < otherNumberWords);
        } else if (numberWords > otherNumberWords) {
            do {
                combined.bitArray.append(bitArray.at(index));
                ++index;
            }
            while (index < numberWords);
        }

        return combined;
    }


    bool BitSet::intersects(const BitSet& other) const {
        if (bitNames != Q_NULLPTR && other.bitNames != Q_NULLPTR) {
            Q_ASSERT(bitNames == other.bitNames);

            unsigned numberWords       = static_cast<unsigned>(bitArray.size());
            unsigned otherNumberWords  = static_cast<unsigned>(other.bitArray.size());
            unsigned numberCommonWords = std::min(numberWords, otherNumberWords);

            for (unsigned index=0 ; index<numberCommonWords ; ++index) {
                if (bitArray.at(index) & other.bitArray.at(index)) {
                    return true;
                }
            }
        }

        return false;
    }


    bool BitSet::sameAs(const BitSet& other) const {
        if (bitNames != other.bitNames) {
            return false;
        } else {
            unsigned numberWords       = static_cast<unsigned>(bitArray.size());
            unsigned otherNumberWords  = static_cast<unsigned>(other.bitArray.size());
            unsigned numberCommonWords = std::min(numberWords, otherNumberWords);

            unsigned index;
            for (index=0 ; index<numberCommonWords ; ++index) {
                if (bitArray.at(index) != other.bitArray.at(index)) {
                    return false;
                }
            }

            if (numberWords < otherNumberWords) {
                do {
                    if (other.bitArray.at(index) != 0) {
                        return false;
                    }

                    ++index;
                } while (index < otherNumberWords);
            } else if (numberWords > otherNumberWords) {
                do {
                    if (bitArray.at(index) != 0) {
                        return false;
                    }

                    ++index;
                } while (index < numberWords);
            }

            return true;
        }
    }


    bool BitSet::isEmpty() const {
        unsigned  numberWords = static_cast<unsigned>(bitArray.size());

        for (unsigned index=0 ; index<numberWords ; ++index) {
            if (bitArray.at(index) != 0) {
                return false;
            }
        }

        return true;
    }


    bool BitSet::isNotEmpty() const {
        unsigned  numberWords = static_cast<unsigned>(bitArray.size());

        for (unsigned index=0 ; index<numberWords ; ++index) {
            if (bitArray.at(index) != 0) {
                return true;
            }
        }

        return false;
    }


    QList<QString> BitSet::bits() const {
        QList<QString> names;

        if (bitNames != Q_NULLPTR) {
            names = bitNames->keys();
        }

        return names;
    }


    QList<QString> BitSet::setBits() const {
        QList<QString> setNames;

        if (bitNames != Q_NULLPTR) {
            QList<QString> allNames = bitNames->keys();
            for (QList<QString>::const_iterator it=allNames.constBegin(),end=allNames.constEnd() ; it!=end ; ++it) {
                if (isSet(*it)) {
                    setNames.append(*it);
                }
            }
        }

        return setNames;
    }


    BitSet BitSet::fullSet() const {
        BitSet result;

        if (bitNames != Q_NULLPTR) {
            result.bitNames = bitNames;

            unsigned numberBits  = static_cast<unsigned>(bitNames->size());
            unsigned numberWords = (numberBits + bitsPerEntry - 1) / bitsPerEntry;

            if (numberBits > 0) {
                for (unsigned index=1 ; index<numberWords ; ++index) {
                    result.bitArray.push_back(static_cast<ArrayType>(-1));
                }

                unsigned remainingBits = numberBits - bitsPerEntry * (numberWords - 1);
                if (remainingBits == bitsPerEntry) {
                    result.bitArray.push_back(static_cast<ArrayType>(-1));
                } else if (remainingBits > 0) {
                    ArrayType mask = static_cast<ArrayType>((static_cast<ArrayType>(1) << remainingBits) - 1);
                    result.bitArray.push_back(mask);
                }
            }
        }

        return result;
    }


    BitSet BitSet::complement() const {
        BitSet result = fullSet();

        unsigned numberWords = static_cast<unsigned>(bitArray.size());
        for (unsigned index=0 ; index<numberWords ; ++index) {
            ArrayType fullValue = result.bitArray.at(index);
            ArrayType thisValue = bitArray.at(index);
            ArrayType newValue  = fullValue & (~thisValue);

            result.bitArray[index] = newValue;
        }

        return result;
    }


    HashResult BitSet::hash(HashSeed seed) const {
        QList<ArrayType>::const_iterator it        = bitArray.constBegin();
        QList<ArrayType>::const_iterator end       = bitArray.constEnd();
        ArrayType                        hashInput = 0;

        while (it != end) {
            hashInput += *it;
            ++it;
        }

        return qHash(hashInput, seed);
    }


    BitSet& BitSet::operator=(const BitSet& other) {
        Q_ASSERT(bitNames == Q_NULLPTR || other.bitNames == Q_NULLPTR || bitNames == other.bitNames);

        bitNames = other.bitNames;
        bitArray = other.bitArray;

        return *this;
    }


    bool BitSet::operator<(const BitSet& other) const {
        bool result = false;

        unsigned thisNumberNonZeroWords  = bitArray.size();
        unsigned otherNumberNonZeroWords = other.bitArray.size();

        while (thisNumberNonZeroWords > 0 && bitArray.at(thisNumberNonZeroWords - 1) == 0) {
            --thisNumberNonZeroWords;
        }

        while (otherNumberNonZeroWords > 0 && other.bitArray.at(otherNumberNonZeroWords - 1) == 0) {
            --otherNumberNonZeroWords;
        }

        if (thisNumberNonZeroWords < otherNumberNonZeroWords) {
            result = true;
        } else if (thisNumberNonZeroWords == otherNumberNonZeroWords && thisNumberNonZeroWords > 0) {
            result = (bitArray.at(thisNumberNonZeroWords - 1) < other.bitArray.at(otherNumberNonZeroWords - 1));
        }

        return result;
    }


    bool BitSet::operator>(const BitSet& other) const {
        bool result = false;

        unsigned thisNumberNonZeroWords  = bitArray.size();
        unsigned otherNumberNonZeroWords = other.bitArray.size();

        while (thisNumberNonZeroWords > 0 && bitArray.at(thisNumberNonZeroWords - 1) == 0) {
            --thisNumberNonZeroWords;
        }

        while (otherNumberNonZeroWords > 0 && other.bitArray.at(otherNumberNonZeroWords - 1) == 0) {
            --otherNumberNonZeroWords;
        }

        if (thisNumberNonZeroWords > otherNumberNonZeroWords) {
            result = true;
        } else if (thisNumberNonZeroWords == otherNumberNonZeroWords && thisNumberNonZeroWords > 0) {
            result = (bitArray.at(thisNumberNonZeroWords - 1) > other.bitArray.at(otherNumberNonZeroWords - 1));
        }

        return result;
    }


    bool BitSet::operator<=(const BitSet& other) const {
        bool result = false;

        unsigned thisNumberNonZeroWords  = bitArray.size();
        unsigned otherNumberNonZeroWords = other.bitArray.size();

        while (thisNumberNonZeroWords > 0 && bitArray.at(thisNumberNonZeroWords - 1) == 0) {
            --thisNumberNonZeroWords;
        }

        while (otherNumberNonZeroWords > 0 && other.bitArray.at(otherNumberNonZeroWords - 1) == 0) {
            --otherNumberNonZeroWords;
        }

        if (thisNumberNonZeroWords < otherNumberNonZeroWords) {
            result = true;
        } else if (thisNumberNonZeroWords == otherNumberNonZeroWords && thisNumberNonZeroWords > 0) {
            result = (bitArray.at(thisNumberNonZeroWords - 1) <= other.bitArray.at(otherNumberNonZeroWords - 1));
        }else if (thisNumberNonZeroWords == 0 && otherNumberNonZeroWords == 0) {
            result = true;
        }

        return result;
    }


    bool BitSet::operator>=(const BitSet& other) const {
        bool result = false;

        unsigned thisNumberNonZeroWords  = bitArray.size();
        unsigned otherNumberNonZeroWords = other.bitArray.size();

        while (thisNumberNonZeroWords > 0 && bitArray.at(thisNumberNonZeroWords - 1) == 0) {
            --thisNumberNonZeroWords;
        }

        while (otherNumberNonZeroWords > 0 && other.bitArray.at(otherNumberNonZeroWords - 1) == 0) {
            --otherNumberNonZeroWords;
        }

        if (thisNumberNonZeroWords > otherNumberNonZeroWords) {
            result = true;
        } else if (thisNumberNonZeroWords == otherNumberNonZeroWords && thisNumberNonZeroWords > 0) {
            result = (bitArray.at(thisNumberNonZeroWords - 1) >= other.bitArray.at(otherNumberNonZeroWords - 1));
        } else if (thisNumberNonZeroWords == 0 && otherNumberNonZeroWords == 0) {
            result = true;
        }

        return result;
    }


    BitSet& BitSet::operator&=(const BitSet& other) {
        Q_ASSERT(bitNames == other.bitNames);

        unsigned numberWords       = static_cast<unsigned>(bitArray.size());
        unsigned otherNumberWords  = static_cast<unsigned>(other.bitArray.size());
        unsigned numberCommonWords = std::min(numberWords, otherNumberWords);

        if (numberWords > numberCommonWords) {
            bitArray.erase(bitArray.begin() + numberCommonWords, bitArray.end());
        }

        for (unsigned index=0 ; index<numberCommonWords ; ++index) {
            bitArray[index] &= other.bitArray[index];
        }

        return *this;
    }


    BitSet& BitSet::operator|=(const BitSet& other) {
        Q_ASSERT(bitNames == other.bitNames);

        unsigned numberWords       = static_cast<unsigned>(bitArray.size());
        unsigned otherNumberWords  = static_cast<unsigned>(other.bitArray.size());
        unsigned numberCommonWords = std::min(numberWords, otherNumberWords);

        unsigned index;
        for (index=0 ; index<numberCommonWords ; ++index) {
            bitArray[index] |= other.bitArray[index];
        }

        while (index < otherNumberWords) {
            bitArray.push_back(other.bitArray[index]);
            ++index;
        }

        return *this;
    }
}

/***********************************************************************************************************************
 * Util::BitSetForwardIterator
 */

namespace Util {
    BitSetForwardIterator::BitSetForwardIterator() {
        workingBitSet = Q_NULLPTR;
        workingValue  = 0;
        currentWord   = 0;
    }


    BitSetForwardIterator::BitSetForwardIterator(const BitSet& bitSet) {
        workingBitSet = &bitSet;
        workingValue  = 0;
        currentWord   = 0;

        const QList<BitSet::ArrayType>& workingArray     = workingBitSet->bitArray;
        unsigned                        workingArraySize = static_cast<unsigned>(workingArray.size());

        if (workingArraySize > 0) {
            unsigned index = 0;
            while (index < workingArraySize && workingArray.at(index) == 0) {
                reportedValue.bitArray.push_back(0);
                ++index;
            }

            if (index < workingArraySize) {
                workingValue           = workingArray.at(index);
                currentWord            = index;
                BitSet::ArrayType mask = maskLsbOne(workingValue);
                reportedValue.bitArray.push_back(mask);

                ++index;
            }

            while (index < workingArraySize) {
                reportedValue.bitArray.push_back(0);
                ++index;
            }
        }

        reportedValue.bitNames = bitSet.bitNames;
    }


    BitSetForwardIterator::BitSetForwardIterator(const BitSetForwardIterator& other) {
        workingBitSet = other.workingBitSet;
        workingValue  = other.workingValue;
        currentWord   = other.currentWord;
        reportedValue = other.reportedValue;
    }


    BitSetForwardIterator::~BitSetForwardIterator() {}


    bool BitSetForwardIterator::isEnd() const {
        return workingValue == 0;
    }


    bool BitSetForwardIterator::isNotEnd() const {
        return workingValue != 0;
    }


    BitSetForwardIterator& BitSetForwardIterator::operator++() {
        const QList<BitSet::ArrayType>& workingArray     = workingBitSet->bitArray;
        unsigned                        workingArraySize = static_cast<unsigned>(workingArray.size());

        if (currentWord < workingArraySize) {
            BitSet::ArrayType mask = reportedValue.bitArray.at(currentWord);
            workingValue &= ~mask;

            if (workingValue == 0) {
                reportedValue.bitArray[currentWord] = 0;
                ++currentWord;

                while (currentWord < workingArraySize && workingArray.at(currentWord) == 0) {
                    ++currentWord;
                }

                if (currentWord < workingArraySize) {
                    workingValue = workingArray.at(currentWord);
                    mask         = maskLsbOne(workingValue);
                    reportedValue.bitArray[currentWord] = mask;
                }
            } else {
                mask = maskLsbOne(workingValue);
                reportedValue.bitArray[currentWord] = mask;
            }
        }

        return *this;
    }


    BitSetForwardIterator BitSetForwardIterator::operator++(int) {
        BitSetForwardIterator oldValue(*this);
        operator++();

        return oldValue;
    }


    const BitSet& BitSetForwardIterator::operator*() const {
        return reportedValue;
    }


    const BitSet* BitSetForwardIterator::operator->() const {
        return &reportedValue;
    }


    BitSetForwardIterator& BitSetForwardIterator::operator=(const BitSetForwardIterator& other) {
        workingBitSet = other.workingBitSet;
        workingValue  = other.workingValue;
        currentWord   = other.currentWord;
        reportedValue = other.reportedValue;

        return *this;
    }


    bool BitSetForwardIterator::operator==(const BitSetForwardIterator& other) {
        return (
               workingBitSet == other.workingBitSet
            && workingValue  == other.workingValue
            && currentWord   == other.currentWord
        );
    }

    bool BitSetForwardIterator::operator!=(const BitSetForwardIterator& other) {
        return (
               workingBitSet != other.workingBitSet
            || workingValue  != other.workingValue
            || currentWord   != other.currentWord
        );
    }
}

/***********************************************************************************************************************
 * Util::BitSetReverseIterator
 */

namespace Util {
    BitSetReverseIterator::BitSetReverseIterator() {
        workingBitSet = Q_NULLPTR;
        workingValue  = 0;
        currentWord   = 0;
    }


    BitSetReverseIterator::BitSetReverseIterator(const BitSet& bitSet) {
        workingBitSet = &bitSet;
        workingValue  = 0;
        currentWord   = 0;

        const QList<BitSet::ArrayType>& workingArray     = workingBitSet->bitArray;
        unsigned                        workingArraySize = static_cast<unsigned>(workingArray.size());

        if (workingArraySize > 0) {
            unsigned index = workingArraySize;
            while (index > 0 && workingArray.at(index - 1) == 0) {
                reportedValue.bitArray.push_front(0);
                --index;
            }

            if (index > 0) {
                workingValue           = workingArray.at(index - 1);
                currentWord            = index;
                BitSet::ArrayType mask = maskMsbOne(workingValue);
                reportedValue.bitArray.push_front(mask);

                --index;
            }

            while (index > 0) {
                reportedValue.bitArray.push_front(0);
                --index;
            }
        }

        reportedValue.bitNames = bitSet.bitNames;
    }


    BitSetReverseIterator::BitSetReverseIterator(const BitSetReverseIterator& other) {
        workingBitSet = other.workingBitSet;
        workingValue  = other.workingValue;
        currentWord   = other.currentWord;
        reportedValue = other.reportedValue;
    }


    BitSetReverseIterator::~BitSetReverseIterator() {}


    bool BitSetReverseIterator::isEnd() const {
        return workingValue == 0;
    }


    bool BitSetReverseIterator::isNotEnd() const {
        return workingValue != 0;
    }


    BitSetReverseIterator& BitSetReverseIterator::operator++() {
        const QList<BitSet::ArrayType>& workingArray = workingBitSet->bitArray;

        if (currentWord > 0) {
            BitSet::ArrayType mask = reportedValue.bitArray.at(currentWord - 1);
            workingValue &= ~mask;

            if (workingValue == 0) {
                reportedValue.bitArray[currentWord - 1] = 0;
                --currentWord;

                while (currentWord > 0 && workingArray.at(currentWord - 1) == 0) {
                    --currentWord;
                }

                if (currentWord > 0) {
                    workingValue = workingArray.at(currentWord - 1);
                    mask         = maskMsbOne(workingValue);
                    reportedValue.bitArray[currentWord - 1] = mask;
                }
            } else {
                mask = maskMsbOne(workingValue);
                reportedValue.bitArray[currentWord - 1] = mask;
            }
        }

        return *this;
    }


    BitSetReverseIterator BitSetReverseIterator::operator++(int) {
        BitSetReverseIterator oldValue(*this);
        operator++();

        return oldValue;
    }


    const BitSet& BitSetReverseIterator::operator*() const {
        return reportedValue;
    }


    const BitSet* BitSetReverseIterator::operator->() const {
        return &reportedValue;
    }


    BitSetReverseIterator& BitSetReverseIterator::operator=(const BitSetReverseIterator& other) {
        workingBitSet = other.workingBitSet;
        workingValue  = other.workingValue;
        currentWord   = other.currentWord;
        reportedValue = other.reportedValue;

        return *this;
    }


    bool BitSetReverseIterator::operator==(const BitSetReverseIterator& other) {
        return (
               workingBitSet == other.workingBitSet
            && workingValue  == other.workingValue
            && currentWord   == other.currentWord
        );
    }

    bool BitSetReverseIterator::operator!=(const BitSetReverseIterator& other) {
        return (
               workingBitSet != other.workingBitSet
            || workingValue  != other.workingValue
            || currentWord   != other.currentWord
        );
    }
}
