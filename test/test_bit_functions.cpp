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
* This file implements tests of the Cryptographic helper functions.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QtTest/QtTest>
#include <QDateTime>
#include <QTime>
#include <QByteArray>

#include <cstdint>
#include <random>

#include <util_bit_functions.h>

#include "test_bit_functions.h"

TestBitFunctions::TestBitFunctions() {}


TestBitFunctions::~TestBitFunctions() {}


void TestBitFunctions::initTestCase() {}


void TestBitFunctions::testNumberOnes32() {
    std::mt19937 rng;
    std::uniform_int_distribution<std::uint32_t> randomValue(0UL, static_cast<std::uint32_t>(-1));

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        std::uint32_t value = randomValue(rng);

        std::uint32_t v = value;
        unsigned      numberOnes = 0;
        while (v) {
            numberOnes += (v & 1);
            v >>= 1;
        }

        QCOMPARE(Util::numberOnes32(value), numberOnes);
    }
}


void TestBitFunctions::testNumberOnes64() {
    std::mt19937 rng;
    std::uniform_int_distribution<std::uint64_t> randomValue(0UL, static_cast<std::uint64_t>(-1));

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        std::uint64_t value = randomValue(rng);

        std::uint64_t v = value;
        unsigned      numberOnes = 0;
        while (v) {
            numberOnes += (v & 1);
            v >>= 1;
        }

        QCOMPARE(Util::numberOnes64(value), numberOnes);
    }
}


void TestBitFunctions::testMsbLocation32() {
    QCOMPARE(Util::msbLocation32(0), -1);

    std::mt19937 rng;
    std::uniform_int_distribution<unsigned>      bitLocation(0, 31);
    std::uniform_int_distribution<std::uint32_t> randomBits(0UL, static_cast<std::uint32_t>(-1));

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned      msbLocation = bitLocation(rng);
        std::uint32_t msbBit      = 1UL << msbLocation;
        std::uint32_t value       = msbBit | ((msbBit-1) & randomBits(rng));
        int           location    = Util::msbLocation32(value);

        QCOMPARE(location, static_cast<int>(msbLocation));
    }
}


void TestBitFunctions::testMsbLocation64() {
    QCOMPARE(Util::msbLocation64(0), -1);

    std::mt19937 rng;
    std::uniform_int_distribution<unsigned>      bitLocation(0, 63);
    std::uniform_int_distribution<std::uint64_t> randomBits(0UL, static_cast<std::uint64_t>(-1));

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned      msbLocation = bitLocation(rng);
        std::uint64_t msbBit      = 1ULL << msbLocation;
        std::uint64_t value       = msbBit | ((msbBit-1) & randomBits(rng));
        int           location    = Util::msbLocation64(value);

        QCOMPARE(location, static_cast<int>(msbLocation));
    }
}


void TestBitFunctions::testMaskLsbZero() {
    std::mt19937 rng;
    std::uniform_int_distribution<unsigned>      bitLocation(0, 63);
    std::uniform_int_distribution<std::uint64_t> randomBits(0UL, static_cast<std::uint64_t>(-1));

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned      lsbLocation = bitLocation(rng);
        std::uint64_t lsbBit      = 1ULL << lsbLocation;
        std::uint64_t lsbMask     = ~((lsbBit << 1) - 1);
        std::uint64_t value       = ~(lsbBit | (lsbMask & randomBits(rng)));
        std::uint64_t mask        = Util::maskLsbZero(value);

        QCOMPARE(mask, lsbBit);
    }
}


void TestBitFunctions::testMaskLsbOne() {
    std::mt19937 rng;
    std::uniform_int_distribution<unsigned>      bitLocation(0, 63);
    std::uniform_int_distribution<std::uint64_t> randomBits(0UL, static_cast<std::uint64_t>(-1));

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned      lsbLocation = bitLocation(rng);
        std::uint64_t lsbBit      = 1ULL << lsbLocation;
        std::uint64_t lsbMask     = ~((lsbBit << 1) - 1);
        std::uint64_t value       = lsbBit | (lsbMask & randomBits(rng));
        std::uint64_t mask        = Util::maskLsbOne(value);

        QCOMPARE(mask, lsbBit);
    }
}


void TestBitFunctions::testMaskMsbZero() {
    std::mt19937 rng;
    std::uniform_int_distribution<unsigned>      bitLocation(0, 63);
    std::uniform_int_distribution<std::uint64_t> randomBits(0UL, static_cast<std::uint64_t>(-1));

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned      msbLocation = bitLocation(rng);
        std::uint64_t msbBit      = 1ULL << msbLocation;
        std::uint64_t msbMask     = msbBit - 1;
        std::uint64_t value       = ~(msbBit | (msbMask & randomBits(rng)));
        std::uint64_t mask        = Util::maskMsbZero(value);

        QCOMPARE(mask, msbBit);
    }
}


void TestBitFunctions::testMaskMsbOne() {
    {
        std::mt19937 rng;
        std::uniform_int_distribution<unsigned>      bitLocation(0, 63);
        std::uniform_int_distribution<std::uint64_t> randomBits(0UL, static_cast<std::uint64_t>(-1));

        for (unsigned i=0 ; i<numberIterations ; ++i) {
            unsigned      msbLocation = bitLocation(rng);
            std::uint64_t msbBit      = 1ULL << msbLocation;
            std::uint64_t msbMask     = msbBit - 1;
            std::uint64_t value       = msbBit | (msbMask & randomBits(rng));
            std::uint64_t mask        = Util::maskMsbOne(value);

            QCOMPARE(mask, msbBit);
        }
    }

    {
        std::mt19937 rng;
        std::uniform_int_distribution<unsigned>     bitLocation(0, 62);
        std::uniform_int_distribution<std::int64_t> randomBits(0UL, INT64_MAX);

        for (unsigned i=0 ; i<numberIterations ; ++i) {
            unsigned      msbLocation = bitLocation(rng);
            std::int64_t msbBit      = 1ULL << msbLocation;
            std::int64_t msbMask     = msbBit - 1;
            std::int64_t value       = msbBit | (msbMask & randomBits(rng));
            std::int64_t mask        = Util::maskMsbOne(value);

            QCOMPARE(mask, msbBit);
        }
    }
}


void TestBitFunctions::testIsPowerOf2() {
    QCOMPARE(Util::isPowerOf2(0), true);

    std::mt19937 rng;
    std::uniform_int_distribution<unsigned> bitLocation(0, 63);
    std::uniform_int_distribution<unsigned> numberBits(1, 16);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned      bitCount = numberBits(rng);
        std::uint64_t value = 0;
        for (unsigned bitNumber=0 ; bitNumber<bitCount ; ++bitNumber) {
            std::uint64_t mask;
            do {
                mask = 1ULL << bitLocation(rng);
            } while (mask & value);

            value |= mask;
        }

        bool expectedResult = (bitCount == 1);
        bool result         = Util::isPowerOf2(value);

        QCOMPARE(result, expectedResult);
    }
}


