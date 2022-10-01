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
* This file implements tests of the BitArray class
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QtTest/QtTest>
#include <QString>
#include <QList>
#include <QSet>

#include <QDebug> // Debug

#include <cstdint>
#include <algorithm>
#include <random>

#include <util_bit_array.h>

#include "test_bit_array.h"

TestBitArray::TestBitArray() {}


TestBitArray::~TestBitArray() {}


void TestBitArray::initTestCase() {}


void TestBitArray::testConstructors() {
    std::mt19937 rng;
    std::uniform_int_distribution<unsigned>       randomBool(0U, 1U);
    std::uniform_int_distribution<unsigned short> random8(0, static_cast<std::uint8_t>(-1));
    std::uniform_int_distribution<std::uint16_t>  random16(0, static_cast<std::uint16_t>(-1));
    std::uniform_int_distribution<std::uint32_t>  random32(0, static_cast<std::uint32_t>(-1));
    std::uniform_int_distribution<std::uint64_t>  random64(0, static_cast<std::uint64_t>(-1));
    std::uniform_int_distribution<unsigned>       randomLength(0U, 65536U);

    Util::BitArray bitArray1;
    QCOMPARE(bitArray1.size(), 0U);

    for (unsigned iteration=1 ; iteration<numberIterations ; ++iteration) {
        unsigned index;

        unsigned bitLength = randomLength(rng);

        bool globalValue = (iteration % 2 == 1);
        Util::BitArray bitArray2(bitLength, globalValue);
        for (index=0 ; index<bitLength ; ++index) {
            QCOMPARE(bitArray2.isSet(index), globalValue);
        }

        bool*    boolArray  = new bool[bitLength];
        for (index=0 ; index<bitLength ; ++index) {
            boolArray[index] = (randomBool(rng) == 1U);
        }

        Util::BitArray bitArray3(boolArray, bitLength);
        QCOMPARE(bitArray3.length(), bitLength);

        for (index=0 ; index<bitLength ; ++index) {
            QCOMPARE(bitArray3.isSet(index), boolArray[index]);
        }

        delete[] boolArray;

        unsigned byteLength = (bitLength + 7) / 8;
        std::uint8_t* byteArray = new std::uint8_t[byteLength];
        for (index=0 ; index<byteLength ; ++index) {
            byteArray[index] = random8(rng);
        }

        Util::BitArray bitArray4(byteArray, bitLength);
        QCOMPARE(bitArray4.length(), bitLength);

        for (index=0 ; index<bitLength ; ++index) {
            bool value = byteArray[index / 8] & (1 << (index % 8)) ? true : false;
            QCOMPARE(bitArray4.isSet(index), value);
        }

        delete[] byteArray;

        unsigned shortLength = (bitLength + 15) / 16;
        std::uint16_t* shortArray = new std::uint16_t[shortLength];
        for (index=0 ; index<shortLength ; ++index) {
            shortArray[index] = random16(rng);
        }

        Util::BitArray bitArray5(shortArray, bitLength);
        QCOMPARE(bitArray5.length(), bitLength);

        for (index=0 ; index<bitLength ; ++index) {
            bool value = shortArray[index / 16] & (1 << (index % 16)) ? true : false;
            QCOMPARE(bitArray5.isSet(index), value);
        }

        delete[] shortArray;

        unsigned longLength = (bitLength + 31) / 32;
        std::uint32_t* longArray = new std::uint32_t[longLength];
        for (index=0 ; index<longLength ; ++index) {
            longArray[index] = random32(rng);
        }

        Util::BitArray bitArray6(longArray, bitLength);
        QCOMPARE(bitArray6.length(), bitLength);

        for (index=0 ; index<bitLength ; ++index) {
            bool value = longArray[index / 32] & (static_cast<std::uint32_t>(1) << (index % 32)) ? true : false;
            QCOMPARE(bitArray6.isSet(index), value);
        }

        delete[] longArray;

        unsigned longLongLength = (bitLength + 63) / 64;
        std::uint64_t* longLongArray = new std::uint64_t[longLongLength];
        for (index=0 ; index<longLongLength ; ++index) {
            longLongArray[index] = random64(rng);
        }

        Util::BitArray bitArray7(longLongArray, bitLength);
        QCOMPARE(bitArray7.length(), bitLength);

        for (index=0 ; index<bitLength ; ++index) {
            bool value = longLongArray[index / 64] & (static_cast<std::uint64_t>(1) << (index % 64)) ? true : false;
            QCOMPARE(bitArray7.isSet(index), value);
        }

        delete[] longLongArray;
    }

    bitArray1.setBit(0);
    bitArray1.clearBit(1);
    bitArray1.setBit(2);
    bitArray1.clearBit(3);
    bitArray1.setBit(4);

    Util::BitArray bitArray2 = bitArray1;

    QVERIFY(bitArray2.isSet(0));
    QVERIFY(bitArray2.isClear(1));
    QVERIFY(bitArray2.isSet(2));
    QVERIFY(bitArray2.isClear(3));
    QVERIFY(bitArray2.isSet(4));

    bitArray1.clearBit(0);
    bitArray1.setBit(1);

    QVERIFY(bitArray1.isClear(0));
    QVERIFY(bitArray1.isSet(1));

    QVERIFY(bitArray2.isSet(0));
    QVERIFY(bitArray2.isClear(1));
}


void TestBitArray::testAssignmentOperator() {
    Util::BitArray bitArray1;

    bitArray1.setBit(0);
    bitArray1.clearBit(1);
    bitArray1.setBit(2);
    bitArray1.clearBit(3);
    bitArray1.setBit(4);

    Util::BitArray bitArray2;
    bitArray2.clearBit(0);
    bitArray2.setBit(1);
    bitArray2.clearBit(2);
    bitArray2.setBit(3);
    bitArray2.clearBit(4);

    QVERIFY(bitArray1.isSet(0));
    QVERIFY(bitArray1.isClear(1));
    QVERIFY(bitArray1.isSet(2));
    QVERIFY(bitArray1.isClear(3));
    QVERIFY(bitArray1.isSet(4));

    QVERIFY(bitArray2.isClear(0));
    QVERIFY(bitArray2.isSet(1));
    QVERIFY(bitArray2.isClear(2));
    QVERIFY(bitArray2.isSet(3));
    QVERIFY(bitArray2.isClear(4));

    bitArray2 = bitArray1;

    QVERIFY(bitArray2.isSet(0));
    QVERIFY(bitArray2.isClear(1));
    QVERIFY(bitArray2.isSet(2));
    QVERIFY(bitArray2.isClear(3));
    QVERIFY(bitArray2.isSet(4));

    bitArray1.setBit(0);
    bitArray1.clearBit(1);
    bitArray1.clearBit(2);
    bitArray1.setBit(3);
    bitArray1.clearBit(4);

    QVERIFY(bitArray2.isSet(0));
    QVERIFY(bitArray2.isClear(1));
    QVERIFY(bitArray2.isSet(2));
    QVERIFY(bitArray2.isClear(3));
    QVERIFY(bitArray2.isSet(4));

    QVERIFY(bitArray1.isSet(0));
    QVERIFY(bitArray1.isClear(1));
    QVERIFY(bitArray1.isClear(2));
    QVERIFY(bitArray1.isSet(3));
    QVERIFY(bitArray1.isClear(4));

    bitArray2.clearBit(0);
    bitArray2.setBit(1);
    bitArray2.clearBit(2);
    bitArray2.setBit(3);
    bitArray2.clearBit(4);

    QVERIFY(bitArray1.isSet(0));
    QVERIFY(bitArray1.isClear(1));
    QVERIFY(bitArray1.isClear(2));
    QVERIFY(bitArray1.isSet(3));
    QVERIFY(bitArray1.isClear(4));
}


void TestBitArray::testBasicAccessors() {
    std::mt19937 rng;
    std::uniform_int_distribution<unsigned> randomBool(0U, 1U);
    std::uniform_int_distribution<unsigned> randomLength(0U, 65536U);

    for (unsigned iteration=1 ; iteration<numberIterations ; ++iteration) {
        Util::BitArray bitArray;

        unsigned bitLength      = randomLength(rng);
        bool*    expectedValues = new bool[bitLength];

        for (unsigned index=0 ; index<bitLength ; ++index) {
            bool value = randomBool(rng) ? true : false;

            expectedValues[index] = value;
            if (value) {
                bitArray.setBit(index);
            } else {
                bitArray.clearBit(index);
            }
        }

        QCOMPARE(bitArray.length(), bitLength);
        QCOMPARE(bitArray.count(),  bitLength);
        QCOMPARE(bitArray.size(),   bitLength);

        for (unsigned index=0 ; index<bitLength ; ++index) {
            QCOMPARE(bitArray.isSet(index),    expectedValues[index]);
            QCOMPARE(bitArray.isClear(index), !expectedValues[index]);
        }

        delete[] expectedValues;
    }
}


void TestBitArray::testResizeMethod() {
    std::mt19937 rng;
    std::uniform_int_distribution<unsigned> randomBool(0U, 1U);
    std::uniform_int_distribution<unsigned> randomLength(0U, 65536U);
    std::exponential_distribution<>         randomLengthAdjustment(1.0/32.0);

    for (unsigned iteration=1 ; iteration<numberIterations ; ++iteration) {
        Util::BitArray bitArray;

        unsigned bitLength      = randomLength(rng);
        bool*    expectedValues = new bool[4 * bitLength];

        for (unsigned index=0 ; index<bitLength ; ++index) {
            bool value = randomBool(rng) ? true : false;

            expectedValues[index] = value;
            bitArray.setBit(index, value);
        }

        QCOMPARE(bitArray.length(), bitLength);
        QCOMPARE(bitArray.count(),  bitLength);
        QCOMPARE(bitArray.size(),   bitLength);

        unsigned newLength;
        bool     shrinkArray = (randomBool(rng) == 1);

        if (shrinkArray) {
            unsigned adjustment;
            do {
                adjustment = randomLengthAdjustment(rng);
            } while (adjustment > bitLength);

            newLength = bitLength - adjustment;
        } else {
            unsigned adjustment;
            do {
                adjustment = randomLengthAdjustment(rng);
            } while (adjustment >= 3 * bitLength);

            newLength = bitLength + adjustment;

            for (unsigned index=bitLength ; index<newLength ; ++index) {
                expectedValues[index] = false;
            }
        }

        bitArray.resize(newLength);

        QCOMPARE(bitArray.length(), newLength);
        QCOMPARE(bitArray.count(),  newLength);
        QCOMPARE(bitArray.size(),   newLength);

        for (unsigned index=0 ; index<newLength ; ++index) {
            QCOMPARE(bitArray.isSet(index), expectedValues[index]);
        }

        delete[] expectedValues;
    }
}


void TestBitArray::testRangeSetClearMethods() {
    std::mt19937 rng;
    std::uniform_int_distribution<unsigned> randomBool(0U, 1U);
    std::uniform_int_distribution<unsigned> randomLength(0U, 65536U);
    std::exponential_distribution<>         randomFillLength(1.0/128.0);

    bool* expectedValues = new bool[2 * 65536U];

    for (unsigned iteration=1 ; iteration<numberIterations ; ++iteration) {
        Util::BitArray bitArray;

        unsigned bitLength      = randomLength(rng);

        for (unsigned index=0 ; index<bitLength ; ++index) {
            bool value = randomBool(rng) ? true : false;

            expectedValues[index] = value;
            bitArray.setBit(index, value);
        }

        QCOMPARE(bitArray.length(), bitLength);
        QCOMPARE(bitArray.count(),  bitLength);
        QCOMPARE(bitArray.size(),   bitLength);

        unsigned startingIndex = randomLength(rng);
        unsigned endingIndex;

        do {
            unsigned fillLength = randomFillLength(rng);
            endingIndex = startingIndex + fillLength;
        } while (endingIndex >= (2U * 65536U));

        bool setBits = randomBool(rng) ? true : false;

        if (setBits) {
            bitArray.setBits(startingIndex, endingIndex);
        } else {
            bitArray.clearBits(startingIndex, endingIndex);
        }

        for (unsigned index=bitLength ; index<startingIndex ; ++index) {
            expectedValues[index] = false;
        }

        for (unsigned index=startingIndex ; index<=endingIndex ; ++index) {
            expectedValues[index] = setBits;
        }

        unsigned newLength = std::max(endingIndex + 1, bitLength);

        QCOMPARE(bitArray.length(), newLength);
        QCOMPARE(bitArray.count(),  newLength);
        QCOMPARE(bitArray.size(),   newLength);

        for (unsigned index=0 ; index<newLength ; ++index) {
            bool reportedValue = bitArray.isSet(index);
            bool expectedValue = expectedValues[index];

            QCOMPARE(reportedValue, expectedValue);
        }
    }

    delete[] expectedValues;
}


void TestBitArray::testSearchMethods() {
    std::mt19937 rng;
    std::uniform_int_distribution<unsigned> randomBool(0U, 1U);
    std::exponential_distribution<>         randomLength(1.0/1024.0);
    std::exponential_distribution<>         randomLeaderLength(1.0/512.0);

    for (unsigned iteration=1 ; iteration<numberIterations ; ++iteration) {
        {
            Util::BitArray bitArray;

            unsigned bitLength = randomLength(rng);

            unsigned leaderLength;
            do {
                leaderLength = randomLeaderLength(rng);
            } while (leaderLength > bitLength);

            Util::BitArray::Index expectedValue = Util::BitArray::invalidIndex;
            for (unsigned index=0 ; index<bitLength ; ++index) {
                bool value = index < leaderLength ? false : (randomBool(rng) ? true : false);

                if (value == true && expectedValue == Util::BitArray::invalidIndex) {
                    expectedValue = index;
                }

                bitArray.setBit(index, value);
            }

            Util::BitArray::Index reportedValue = bitArray.firstSetBit();
            QCOMPARE(reportedValue, expectedValue);

            if (bitLength > 0) {
                std::uniform_int_distribution<unsigned> randomStartingIndex(0U, bitLength - 1);
                Util::BitArray::Index startingIndex = randomStartingIndex(rng);
                Util::BitArray::Index expectedValue = startingIndex;
                while (expectedValue < bitLength && bitArray.isClear(expectedValue)) {
                    ++expectedValue;
                }

                if (expectedValue >= bitLength) {
                    expectedValue = Util::BitArray::invalidIndex;
                }

                Util::BitArray::Index result = bitArray.firstSetBit(startingIndex);
                QCOMPARE(result, expectedValue);
            }
        }

        {
            Util::BitArray bitArray;

            unsigned bitLength = randomLength(rng);

            unsigned leaderLength;
            do {
                leaderLength = randomLeaderLength(rng);
            } while (leaderLength > bitLength);

            Util::BitArray::Index expectedValue = Util::BitArray::invalidIndex;
            for (unsigned index=0 ; index<bitLength ; ++index) {
                bool value = index < leaderLength ? true : (randomBool(rng) ? true : false);

                if (value == false && expectedValue == Util::BitArray::invalidIndex) {
                    expectedValue = index;
                }

                bitArray.setBit(index, value);
            }

            Util::BitArray::Index reportedValue = bitArray.firstClearedBit();
            QCOMPARE(reportedValue, expectedValue);

            if (bitLength > 0) {
                std::uniform_int_distribution<unsigned> randomStartingIndex(0U, bitLength - 1);
                Util::BitArray::Index startingIndex = randomStartingIndex(rng);
                Util::BitArray::Index expectedValue = startingIndex;
                while (expectedValue < bitLength && bitArray.isSet(expectedValue)) {
                    ++expectedValue;
                }

                if (expectedValue >= bitLength) {
                    expectedValue = Util::BitArray::invalidIndex;
                }

                Util::BitArray::Index result = bitArray.firstClearedBit(startingIndex);
                QCOMPARE(result, expectedValue);
            }
        }
    }
}


void TestBitArray::testComparisonOperators() {
    std::mt19937 rng;
    std::uniform_int_distribution<unsigned> randomBool(0U, 1U);
    std::exponential_distribution<>         randomLength(1.0/1024.0);

    for (unsigned iteration=1 ; iteration<numberIterations ; ++iteration) {
        unsigned bitLength = randomLength(rng);

        Util::BitArray bitArray1;
        Util::BitArray bitArray2;

        unsigned bitFlipLocation;;
        do {
            bitFlipLocation = randomLength(rng);
        } while (bitFlipLocation >= bitLength);

        for (unsigned index=0 ; index<bitLength ; ++index) {
            bool value = randomBool(rng) ? true : false;
            bitArray1.setBit(index, value);
            bitArray2.setBit(index, value);
        }

        QCOMPARE((bitArray1 == bitArray2), true);
        QCOMPARE((bitArray1 != bitArray2), false);

        Util::BitArray bitArray3 = bitArray1;

        bool value = bitArray2.isSet(bitFlipLocation);
        bitArray2.setBit(bitFlipLocation, !value);

        QCOMPARE((bitArray1 == bitArray2), false);
        QCOMPARE((bitArray1 != bitArray2), true);

        bitArray3.resize(bitArray3.size() + 1);

        QCOMPARE((bitArray1 == bitArray3), false);
        QCOMPARE((bitArray1 != bitArray3), true);

        Util::BitArray bitArray4;
        Util::BitArray bitArray5;

        QCOMPARE((bitArray1 == bitArray4), false);
        QCOMPARE((bitArray1 != bitArray4), true);

        QCOMPARE((bitArray4 == bitArray5), true);
        QCOMPARE((bitArray4 != bitArray5), false);
    }
}

