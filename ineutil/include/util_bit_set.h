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
* This header defines the \ref Util::BitSet class.
***********************************************************************************************************************/

/* .. sphinx-project ineutil */

#ifndef UTIL_BIT_SET_H
#define UTIL_BIT_SET_H

#include <QString>
#include <QHash>
#include <QList>

#include <cstdint>

#include "util_common.h"
#include "util_hash_functions.h"

namespace Util {
    class BitSetForwardIterator;
    class BitSetReverseIterator;

    /**
     * Class that can be used to maintain an extensible set of flags.  You can use this class in much the same way as
     * an integer and set of defines or enumerated values to create masks that can be tested.  Flags are defined using
     * a map of string values.
     */
    class UTIL_PUBLIC_API BitSet {
        friend class BitSetForwardIterator;
        friend class BitSetReverseIterator;

        public:
            /**
             * Type used for bit name hashes.
             */
            typedef QHash<QString, unsigned> BitNameHash;

        protected:
            /**
             * Constructor, you should use this constructor in derived classes to create specific types of bit sets.
             *
             * \param[in] bitHash Pointer to a static instance of BitNameHash used to map names to specific bits.  Note
             *                    that bit numbers in the hash are expected to always be assigned consecutively,
             *                    starting from 0.
             */
            BitSet(BitNameHash* bitHash);

        public:
            BitSet();

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            BitSet(const BitSet& other);

            ~BitSet();

            /**
             * Method that clears the bit set.
             */
            void clear();

            /**
             * Method you can use to determine if a specified bit exists, by name.
             *
             * \param[in] bitName The name of the bit to check.
             *
             * \return Returns true if the bit is defined.  Returns false if the bit is not defined.
             */
            bool bitDefined(const QString& bitName);

            /**
             * Method you can use to selectively set or clear an individual bit.
             *
             * \param[in] bitName The name of the bit.
             *
             * \param[in] isSet   If true, the bit will be set.  If false, the bit will be cleared.
             *
             * \return Returns true if the bit is defined.  Returns false if the bit is not defined.
             */
            bool setBit(const QString& bitName, bool isSet = true);

            /**
             * Method you can use to selectively clear or set an individual bit.
             *
             * \param[in] bitName The name of the bit.
             *
             * \param[in] isClear If true, the bit will be cleared.  If false, the bit will be set.
             *
             * \return Returns true if the bit is defined.  Returns false if the bit is not defined.
             */
            bool clearBit(const QString& bitName, bool isClear = true);

            /**
             * Template method that sets one or more bits, by name.
             *
             * \param[in] firstBit  The name of the first bit to be set.
             *
             * \return Returns true if the bit is defined.  Returns false if the bit is not defined.
             */
            template <typename T> UTIL_PUBLIC_TEMPLATE_METHOD bool setBits(const T& firstBit) {
                return setBit(firstBit, true);
            }

            /**
             * Template method that sets one or more bits, by name.
             *
             * \param[in] firstBit  The name of the first bit to be set.
             *
             * \param[in] otherBits The name of other, additional bits to be set.
             *
             * \return Returns true if all the bits are defined.  Returns false if any bit is undefined.
             */
            template <typename T1, typename... T2> UTIL_PUBLIC_TEMPLATE_METHOD bool setBits(
                    const T1&    firstBit,
                    const T2&... otherBits
                ) {
                return setBit(firstBit, true) && setBits(otherBits...);
            }

            /**
             * Template method that clears one or more bits, by name.
             *
             * \param[in] firstBit  The name of the first bit to be cleared.
             *
             * \return Returns true if the bit is defined.  Returns false if the bit is not defined.
             */
            template <typename T> UTIL_PUBLIC_TEMPLATE_METHOD bool clearBits(const T& firstBit) {
                return setBit(firstBit, false);
            }

            /**
             * Template method that clears one or more bits, by name.
             *
             * \param[in] firstBit  The name of the first bit to be cleared.
             *
             * \param[in] otherBits The name of other, additional bits to be cleared.
             *
             * \return Returns true if all the bits are defined.  Returns false if any bit is undefined.
             */
            template <typename T1, typename... T2> UTIL_PUBLIC_TEMPLATE_METHOD bool clearBits(
                    const T1&    firstBit,
                    const T2&... otherBits
                ) {
                return setBit(firstBit, false) && clearBits(otherBits...);
            }

            /**
             * Method you can use to determine the current number of set bits in the set.
             *
             * \return Returns the current number of set bits in the set.
             */
            unsigned numberSetBits() const;

            /**
             * Method you can use to determine the current number of cleared bits in the set.
             *
             * \return Returns the current number of cleared bits in the set.
             */
            unsigned numberClearedBits() const;

            /**
             * Method you can use to determine if a specific bit is set.
             *
             * \param[in] bitName The name of the bit.
             *
             * \return Returns true if the bit is set.  Returns false if the bit is cleared.  The method will return
             *         false if the bit is not defined.
             */
            bool isSet(const QString& bitName) const;

            /**
             * Method you can use to determine if a specific bit is cleared.
             *
             * \param[in] bitName The name of the bit.
             *
             * \return Returns true if the bit is cleared.  Returns false if the bit is set.  The method will return
             *         true if the bit is not defined.
             */
            bool isCleared(const QString& bitName) const;

            /**
             * Method that determines if this bit set instance tracks the same bits as another instance.
             *
             * \param[in] other The instance to check against this instance.
             *
             * \return Returns true if the instances track the same bits.  Returns false if the instances track
             *         different bits.
             */
            bool tracksSameBitsAs(const BitSet& other) const;

            /**
             * Method that returns the intersection of this bit set with another bit set.  This method will assert if
             * the two bit set instances do not use the same name hash.
             *
             * \param[in] other The instance to calculate the intersection with.
             *
             * \return Returns a bit set that represents the intersection of the two bit sets.
             */
            BitSet intersectionBits(const BitSet& other) const;

            /**
             * Method that returns the union of this bit set with another bit set.  This method will assert if the two
             * bit set instances do not use the same name hash.
             *
             * \param[in] other The instance to calculate the union with.
             *
             * \return Returns a bit set that represents the union of the two bit sets.
             */
            BitSet unionBits(const BitSet& other) const;

            /**
             * Method that determines if this instance intersects another instance.  This method will assert if the two
             * bit set instances do not use the same name hash.
             *
             * \param[in] other The instance to check against.
             *
             * \return Returns true if this instance intersects the other instance.  Returns false if there is not
             *         intersection.
             */
            bool intersects(const BitSet& other) const;

            /**
             * Method that determines if this instance is the same as another instance.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the instances are equal.  Returns false if the instances are not equal.
             */
            bool sameAs(const BitSet& other) const;

            /**
             * Method that determines if this set is empty.
             *
             * \return Returns true if this is an empty set.  Returns false if this is not an empty set.
             */
            bool isEmpty() const;

            /**
             * Method that determines if this set is not empty.
             *
             * \return Returns true if this is not an empty set.  Returns false if this is an empty set.
             */
            bool isNotEmpty() const;

            /**
             * Method that returns a list of all the bits that are tracked by this bit set.
             *
             * \return Returns a list of every bit tracked by the bit set.
             */
            QList<QString> bits() const;

            /**
             * Method that returns a list of the names of all the set bits.  Note that this method is slow.
             *
             * \return Returns a list of the name of every set bit.
             */
            QList<QString> setBits() const;

            /**
             * Method that returns a universal set for the given type.
             *
             * \return Returns a universal set for this type, that is, a set with every bit set.
             */
            BitSet fullSet() const;

            /**
             * Method that returns the complement of this set.
             *
             * \return Returns the complmenent of this set.
             */
            BitSet complement() const;

            /**
             * Method that calculates a hash for this \ref Util::BitSet suitable for use in hash tables and other
             * similar structures.
             *
             * \param[in] seed An optional seed to apply to the hash.
             *
             * \return Returns a hash for the \ref Util::BitSet
             */
            HashResult hash(HashSeed seed = 0) const;

            /**
             * Assignment operator.  Note that this operator will assert if the instances do not reference the same
             * bit name hash.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this object.
             */
            BitSet& operator=(const BitSet& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns true if the instances are equal.  Returns false if the instances are not equal.
             */
            inline bool operator==(const BitSet& other) const {
                return sameAs(other);
            }

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns true if the instances are different.  Returns false if the instances are equal.
             */
            inline bool operator!=(const BitSet& other) const {
                return !sameAs(other);
            }

            /**
             * Comparison operator.  Provided to allow bit set instances to be used in maps.
             *
             * \param[in] other The instance to compare against.
             *
             * \return Returns true if this instance should precede the other instance in the map.  Returns false if
             *         the two instances are the same or this instance should follow the other instance in the map.
             */
            bool operator<(const Util::BitSet& other) const;

            /**
             * Comparison operator.  Provided to allow bit set instances to be used in maps.
             *
             * \param[in] other The instance to compare against.
             *
             * \return Returns true if this instance should precede the other instance in the map.  Returns false if
             *         the two instances are the same or this instance should follow the other instance in the map.
             */
            bool operator>(const Util::BitSet& other) const;

            /**
             * Comparison operator.  Provided to allow bit set instances to be used in maps.
             *
             * \param[in] other The instance to compare against.
             *
             * \return Returns true if this instance should precede the other instance in the map.  Returns false if
             *         the two instances are the same or this instance should follow the other instance in the map.
             */
            bool operator<=(const Util::BitSet& other) const;

            /**
             * Comparison operator.  Provided to allow bit set instances to be used in maps.
             *
             * \param[in] other The instance to compare against.
             *
             * \return Returns true if this instance should precede the other instance in the map.  Returns false if
             *         the two instances are the same or this instance should follow the other instance in the map.
             */
            bool operator>=(const Util::BitSet& other) const;

            /**
             * Index operator.
             *
             * \param[in] bitName The name of the bit to check.
             *
             * \return Returns true if the bit is set.  Returns false if the bit is cleared.
             */
            inline bool operator[](const QString& bitName) const {
                return isSet(bitName);
            }

            /**
             * Index operator.
             *
             * \param[in] bitName The name of the bit to check.
             *
             * \return Returns true if the bit is set.  Returns false if the bit is cleared.
             */
            inline bool operator[](const char* bitName) const {
                return isSet(QString::fromLocal8Bit(bitName));
            }

            /**
             * Modifying intersection operator.  This operator will assert if the two instances do not use the same bit
             * name hash.
             *
             * \param[in] other The instance to intersect with this instance.
             *
             * \return Returns a reference to this object.
             */
            BitSet& operator&=(const BitSet& other);

            /**
             * Modifying union operator.  This operator will assert if the two instances do not use the same bit name
             * hash.
             *
             * \param[in] other The instance to join with this instance.
             *
             * \return Returns a reference to this object.
             */
            BitSet& operator|=(const BitSet& other);

            /**
             * Modifying bit set operator.
             *
             * \param[in] bitName The name of the bit to be set.
             *
             * \return Returns a reference to this object.
             */
            inline BitSet& operator<<(const QString& bitName) {
                setBit(bitName, true);
                return *this;
            }

            /**
             * Complement operator.
             *
             * \return Returns a complement of this bit set.
             */
            inline BitSet operator~() const {
                return complement();
            }

        private:
            /**
             * Type used for the internal array storage.
             */
            typedef std::uint32_t ArrayType;

            /**
             * Value indicating the number of bits stored per array entry type.
             */
            static constexpr unsigned bitsPerEntry = 32;

            /**
             * Pointer to the underlying hash.
             */
            BitNameHash* bitNames;

            /**
             * The underlying bit array.
             */
            QList<ArrayType> bitArray;
    };

    /**
     * Class that can be used to interate through the bit elements in a \ref Util::BitSet.  You can use this class much
     * like a traditional forward iterator.
     */
    class UTIL_PUBLIC_API BitSetForwardIterator {
        public:
            BitSetForwardIterator();

            /**
             * Constructor.
             *
             * \param[in] bitSet The \ref Util::BitSet to use to initialize this iterator.
             */
            BitSetForwardIterator(const BitSet& bitSet);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            BitSetForwardIterator(const BitSetForwardIterator& other);

            ~BitSetForwardIterator();

            /**
             * Method you can use to determine if the iterator has passed the last set value.
             *
             * \return Returns true if the iterator has reached the end entry.  Returns false if the iterator still
             *         points to a valid entry.
             */
            bool isEnd() const;

            /**
             * Method you can use to determine if the iterator has not passed the last set value.
             *
             * \return Returns true if the iterator points to a valid entry.  Returns false if the iterator has reached
             *         the end.
             */
            bool isNotEnd() const;

            /**
             * Prefix increment operator.
             *
             * \return Returns a reference to this instance.
             */
            BitSetForwardIterator& operator++();

            /**
             * Postfix increment operator.
             *
             * \param[in] dummy Dummy parameter, unused.
             *
             * \return Returns a copy of this instance prior to the increment operation.
             */
            BitSetForwardIterator operator++(int dummy);

            /**
             * Reference operator.
             *
             * \return Returns a reference to a local \ref Util::BitSet instance containing a single set entry.
             */
            const BitSet& operator*() const;

            /**
             * Pointer operator.
             *
             * \return Returns a pointer to a local \ref Util::BitSet instance containing a single set entry.
             */
            const BitSet* operator->() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            BitSetForwardIterator& operator=(const BitSetForwardIterator& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the iterators are identical.  Returns false if the iterators are different.
             */
            bool operator==(const BitSetForwardIterator& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the iterators are different.  Returns false if the iterators are identical.
             */
            bool operator!=(const BitSetForwardIterator& other);

        private:
            /**
             * Pointer to the bit set we are operating on.
             */
            const BitSet* workingBitSet;

            /**
             * The current working value in the bit set.
             */
            BitSet::ArrayType workingValue;

            /**
             * The current word in the bit set.
             */
            unsigned currentWord;

            /**
             * A bit set used to report the current value.
             */
            BitSet reportedValue;
    };

    /**
     * Class that can be used to interate backwards through the bit elements in a \ref Util::BitSet.  You can use this
     * class much like a traditional reverse iterator.
     */
    class UTIL_PUBLIC_API BitSetReverseIterator {
        public:
            BitSetReverseIterator();

            /**
             * Constructor.
             *
             * \param[in] bitSet The \ref Util::BitSet to use to initialize this iterator.
             */
            BitSetReverseIterator(const BitSet& bitSet);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            BitSetReverseIterator(const BitSetReverseIterator& other);

            ~BitSetReverseIterator();

            /**
             * Method you can use to determine if the iterator has passed the last set value.
             *
             * \return Returns true if the iterator has reached the end entry.  Returns false if the iterator still
             *         points to a valid entry.
             */
            bool isEnd() const;

            /**
             * Method you can use to determine if the iterator has not passed the last set value.
             *
             * \return Returns true if the iterator points to a valid entry.  Returns false if the iterator has reached
             *         the end.
             */
            bool isNotEnd() const;

            /**
             * Prefix increment operator.
             *
             * \return Returns a reference to this instance.
             */
            BitSetReverseIterator& operator++();

            /**
             * Postfix increment operator.
             *
             * \param[in] dummy Dummy parameter, unused.
             *
             * \return Returns a copy of this instance prior to the increment operation.
             */
            BitSetReverseIterator operator++(int dummy);

            /**
             * Reference operator.
             *
             * \return Returns a reference to a local \ref Util::BitSet instance containing a single set entry.
             */
            const BitSet& operator*() const;

            /**
             * Pointer operator.
             *
             * \return Returns a pointer to a local \ref Util::BitSet instance containing a single set entry.
             */
            const BitSet* operator->() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            BitSetReverseIterator& operator=(const BitSetReverseIterator& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the iterators are identical.  Returns false if the iterators are different.
             */
            bool operator==(const BitSetReverseIterator& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to be compared against.
             *
             * \return Returns true if the iterators are different.  Returns false if the iterators are identical.
             */
            bool operator!=(const BitSetReverseIterator& other);

        private:
            /**
             * Pointer to the bit set we are operating on.
             */
            const BitSet* workingBitSet;

            /**
             * The current working value in the bit set.
             */
            BitSet::ArrayType workingValue;

            /**
             * The current word in the bit set.
             */
            unsigned currentWord;

            /**
             * A bit set used to report the current value.
             */
            BitSet reportedValue;
    };
}

/**
 * Hash function for the \ref Util::BitSet class.
 *
 * \param[in] value The \ref Util::BitSet to be hashed.
 *
 * \param[in] seed  A seed to apply when calculating the hash.
 *
 * \return Returns a hash for this value.
 */
inline UTIL_PUBLIC_API Util::HashResult qHash(const Util::BitSet& value, Util::HashSeed seed = 0) {
    return value.hash(seed);
}

/**
 * Intersection operator.
 *
 * \param[in] a The first bit set to calculate the intersection from.
 *
 * \param[in] b The second bit set to calculate the intersection from.
 *
 * \return Returns the intersection of the two bit sets.
 */
inline UTIL_PUBLIC_API Util::BitSet operator&(const Util::BitSet& a, const Util::BitSet& b) {
    return a.intersectionBits(b);
}

/**
 * Union operator.
 *
 * \param[in] a The first bit set to calculate the union from.
 *
 * \param[in] b The second bit set to calculate the union from.
 *
 * \return Returns the intersection of the two bit sets.
 */
inline UTIL_PUBLIC_API Util::BitSet operator|(const Util::BitSet& a, const Util::BitSet& b) {
    return a.unionBits(b);
}

#endif
