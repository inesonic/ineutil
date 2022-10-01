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
* This file implements tests of the BitSet class
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QtTest/QtTest>
#include <QString>
#include <QList>
#include <QSet>

#include <cstdint>
#include <random>

#include <util_bit_set.h>

#include "test_bit_set.h"

/***********************************************************************************************************************
 * BitSet1
 */

BitSet1::BitNameHash BitSet1::bitNameHash;

BitSet1::BitSet1():Util::BitSet(&bitNameHash) {}

BitSet1::~BitSet1() {}

bool BitSet1::assignBit(const QString& bitName) {
    bool success;

    if (!bitNameHash.contains(bitName)) {
        unsigned bitIndex = bitNameHash.size();
        bitNameHash.insert(bitName, bitIndex);

        success = true;
    } else {
        success = false;
    }

    return success;
}

/***********************************************************************************************************************
 * BitSet2
 */

BitSet2::BitNameHash BitSet2::bitNameHash;

BitSet2::BitSet2():Util::BitSet(&bitNameHash) {}

BitSet2::~BitSet2() {}

bool BitSet2::assignBit(const QString& bitName) {
    bool success;

    if (!bitNameHash.contains(bitName)) {
        unsigned bitIndex = bitNameHash.size();
        bitNameHash.insert(bitName, bitIndex);

        success = true;
    } else {
        success = false;
    }

    return success;
}

/***********************************************************************************************************************
 * TestBitSet
 */

TestBitSet::TestBitSet() {}


TestBitSet::~TestBitSet() {}


void TestBitSet::initTestCase() {
    for (unsigned i=0 ; i<256 ; ++i) {
        BitSet1::assignBit(tr("BIT%1").arg(i + 1));
        BitSet2::assignBit(tr("BIT%1").arg(i + 1));
    }
}


void TestBitSet::testConstructors() {
    BitSet1      bitSet1;
    BitSet2      bitSet2;
    Util::BitSet bitSet3 = bitSet1;

    QCOMPARE(bitSet1.tracksSameBitsAs(bitSet2), false);
    QCOMPARE(bitSet1.tracksSameBitsAs(bitSet3), true);
}


void TestBitSet::testSingleBitSetClearMethods() {
    BitSet1 bitSet;

    std::mt19937 rng;
    std::uniform_int_distribution<unsigned> randomBitIndex(0U, 255U);
    std::uniform_int_distribution<unsigned> randomState(0U, 1U);

    QList<bool> setBits;
    unsigned bitIndex;

    for (bitIndex=0 ; bitIndex<256 ; ++bitIndex) {
        setBits.push_back(false);
    }

    for (unsigned iterationNumber=0 ; iterationNumber<numberIterations ; ++iterationNumber) {
        unsigned bitIndex = randomBitIndex(rng);
        QCOMPARE(setBits.at(bitIndex), bitSet.isSet(QString("BIT%1").arg(bitIndex + 1)));

        bitIndex = randomBitIndex(rng);
        QCOMPARE(!setBits.at(bitIndex), bitSet.isCleared(QString("BIT%1").arg(bitIndex + 1)));

        bitIndex = randomBitIndex(rng);
        bool nowSet = (randomState(rng) != 0);

        setBits[bitIndex] = nowSet;
        bitSet.setBit(QString("BIT%1").arg(bitIndex + 1), nowSet);
    }
}


void TestBitSet::testTemplateMethods() {
    BitSet1       bitSet;
    QSet<QString> setBits;

    bitSet.setBits("BIT5", "BIT3", "BIT254", "BIT35", "BIT77", "BIT45");
    setBits << "BIT5" << "BIT3" << "BIT254" << "BIT35" << "BIT77" << "BIT45";

    for (unsigned index=0; index<256 ; ++index) {
        QString bitName = QString("BIT%1").arg(index + 1);
        QCOMPARE(bitSet.isSet(bitName), setBits.contains(bitName));
    }

    bitSet.clearBits("BIT3", "BIT254", "BIT77");
    setBits.remove("BIT3");
    setBits.remove("BIT254");
    setBits.remove("BIT77");

    for (unsigned index=0; index<256 ; ++index) {
        QString bitName = QString("BIT%1").arg(index + 1);
        QCOMPARE(bitSet.isSet(bitName), setBits.contains(bitName));
    }
}


void TestBitSet::testIntersectionMethod() {
    BitSet1 bitSet1("BIT1", "BIT2", "BIT3", "BIT4");
    BitSet1 bitSet2("BIT3", "BIT4", "BIT5", "BIT6");
    BitSet1 bitSet3("BIT7", "BIT8");

    BitSet1 bitSet4 = bitSet1.intersectionBits(bitSet2);
    QCOMPARE(bitSet4.isEmpty(), false);
    QCOMPARE(bitSet4.isNotEmpty(), true);

    QCOMPARE(bitSet4.isSet("BIT1"), false);
    QCOMPARE(bitSet4.isSet("BIT2"), false);
    QCOMPARE(bitSet4.isSet("BIT3"), true);
    QCOMPARE(bitSet4.isSet("BIT4"), true);

    unsigned index;
    for (index=5 ; index<=256 ; ++index) {
        QCOMPARE(bitSet4.isSet(QString("BIT%1").arg(index)), false);
    }

    BitSet1 bitSet5 = bitSet1.intersectionBits(bitSet3);
    QCOMPARE(bitSet5.isEmpty(), true);
    QCOMPARE(bitSet5.isNotEmpty(), false);

    QCOMPARE(bitSet1.intersects(bitSet2), true);
    QCOMPARE(bitSet1.intersects(bitSet3), false);
}


void TestBitSet::testUnionMethod() {
    BitSet1 bitSet1("BIT1", "BIT2", "BIT3", "BIT4");
    BitSet1 bitSet2("BIT3", "BIT4", "BIT5", "BIT6");
    BitSet1 bitSet3("BIT7", "BIT8");

    BitSet1 bitSet4 = bitSet1.unionBits(bitSet2);

    unsigned index;
    for (index=1 ; index<=256 ; ++index) {
        QCOMPARE(bitSet4.isSet(QString("BIT%1").arg(index)), index <= 6);
    }
}


void TestBitSet::testMethodOperators() {
    BitSet1      bitSet1("BIT1",         "BIT3", "BIT64", "BIT65", "BIT201" );
    BitSet1      bitSet2("BIT1", "BIT2", "BIT3",          "BIT65"           );
    Util::BitSet bitSet3 = bitSet1;

    QCOMPARE(bitSet1.sameAs(bitSet1), true);
    QCOMPARE(bitSet1.sameAs(bitSet2), false);
    QCOMPARE(bitSet1.sameAs(bitSet3), true);

    QCOMPARE(bitSet1[QString::fromLocal8Bit("BIT3")], true);
    QCOMPARE(bitSet1[QString::fromLocal8Bit("BIT2")], false);
    QCOMPARE(bitSet1[QString::fromLocal8Bit("foo")], false);

    QCOMPARE(bitSet1["BIT3"], true);
    QCOMPARE(bitSet1["BIT2"], false);
    QCOMPARE(bitSet1["foo"], false);

    bitSet3 &= bitSet2;
    QSet<QString> compareSet;
    compareSet << "BIT1" << "BIT3" << "BIT65";

    unsigned index;
    for (index=1 ; index<=256 ; ++index) {
        QString bitName = QString("BIT%1").arg(index);
        QCOMPARE(bitSet3[bitName], compareSet.contains(bitName));
    }

    bitSet3 |= BitSet1("BIT4");
    compareSet << "BIT4";

    for (index=1 ; index<=256 ; ++index) {
        QString bitName = QString("BIT%1").arg(index);
        QCOMPARE(bitSet3[bitName], compareSet.contains(bitName));
    }

    bitSet3 << "BIT5";
    compareSet << "BIT5";

    QCOMPARE(bitSet3["BIT5"], true);

    BitSet1 bitSet4 = ~bitSet3;
    for (index=1 ; index<=256 ; ++index) {
        QString bitName = QString("BIT%1").arg(index);
        QCOMPARE(bitSet4[bitName], !compareSet.contains(bitName));
    }
}


void TestBitSet::testComparisonOperators() {
    // Note: This method relies on an understanding of the internal operation of the Util::BitSet class.

    BitSet1 bitSet1;
    BitSet1 bitSet2("BIT1"                  );
    BitSet1 bitSet3("BIT1", "BIT2"          );
    BitSet1 bitSet4("BIT1",         "BIT129");

    QCOMPARE(bitSet1 == bitSet1, true);
    QCOMPARE(bitSet1 == bitSet2, false);
    QCOMPARE(bitSet1 == bitSet3, false);
    QCOMPARE(bitSet1 == bitSet4, false);

    QCOMPARE(bitSet2 == bitSet1, false);
    QCOMPARE(bitSet2 == bitSet2, true);
    QCOMPARE(bitSet2 == bitSet3, false);
    QCOMPARE(bitSet2 == bitSet4, false);

    QCOMPARE(bitSet3 == bitSet1, false);
    QCOMPARE(bitSet3 == bitSet2, false);
    QCOMPARE(bitSet3 == bitSet3, true);
    QCOMPARE(bitSet3 == bitSet4, false);

    QCOMPARE(bitSet4 == bitSet1, false);
    QCOMPARE(bitSet4 == bitSet2, false);
    QCOMPARE(bitSet4 == bitSet3, false);
    QCOMPARE(bitSet4 == bitSet4, true);


    QCOMPARE(bitSet1 != bitSet1, false);
    QCOMPARE(bitSet1 != bitSet2, true);
    QCOMPARE(bitSet1 != bitSet3, true);
    QCOMPARE(bitSet1 != bitSet4, true);

    QCOMPARE(bitSet2 != bitSet1, true);
    QCOMPARE(bitSet2 != bitSet2, false);
    QCOMPARE(bitSet2 != bitSet3, true);
    QCOMPARE(bitSet2 != bitSet4, true);

    QCOMPARE(bitSet3 != bitSet1, true);
    QCOMPARE(bitSet3 != bitSet2, true);
    QCOMPARE(bitSet3 != bitSet3, false);
    QCOMPARE(bitSet3 != bitSet4, true);

    QCOMPARE(bitSet4 != bitSet1, true);
    QCOMPARE(bitSet4 != bitSet2, true);
    QCOMPARE(bitSet4 != bitSet3, true);
    QCOMPARE(bitSet4 != bitSet4, false);


    QCOMPARE(bitSet1 < bitSet1, false);
    QCOMPARE(bitSet1 < bitSet2, true);
    QCOMPARE(bitSet1 < bitSet3, true);
    QCOMPARE(bitSet1 < bitSet4, true);

    QCOMPARE(bitSet2 < bitSet1, false);
    QCOMPARE(bitSet2 < bitSet2, false);
    QCOMPARE(bitSet2 < bitSet3, true);
    QCOMPARE(bitSet2 < bitSet4, true);

    QCOMPARE(bitSet3 < bitSet1, false);
    QCOMPARE(bitSet3 < bitSet2, false);
    QCOMPARE(bitSet3 < bitSet3, false);
    QCOMPARE(bitSet3 < bitSet4, true);

    QCOMPARE(bitSet4 < bitSet1, false);
    QCOMPARE(bitSet4 < bitSet2, false);
    QCOMPARE(bitSet4 < bitSet3, false);
    QCOMPARE(bitSet4 < bitSet4, false);


    QCOMPARE(bitSet1 > bitSet1, false);
    QCOMPARE(bitSet1 > bitSet2, false);
    QCOMPARE(bitSet1 > bitSet3, false);
    QCOMPARE(bitSet1 > bitSet4, false);

    QCOMPARE(bitSet2 > bitSet1, true);
    QCOMPARE(bitSet2 > bitSet2, false);
    QCOMPARE(bitSet2 > bitSet3, false);
    QCOMPARE(bitSet2 > bitSet4, false);

    QCOMPARE(bitSet3 > bitSet1, true);
    QCOMPARE(bitSet3 > bitSet2, true);
    QCOMPARE(bitSet3 > bitSet3, false);
    QCOMPARE(bitSet3 > bitSet4, false);

    QCOMPARE(bitSet4 > bitSet1, true);
    QCOMPARE(bitSet4 > bitSet2, true);
    QCOMPARE(bitSet4 > bitSet3, true);
    QCOMPARE(bitSet4 > bitSet4, false);


    QCOMPARE(bitSet1 <= bitSet1, true);
    QCOMPARE(bitSet1 <= bitSet2, true);
    QCOMPARE(bitSet1 <= bitSet3, true);
    QCOMPARE(bitSet1 <= bitSet4, true);

    QCOMPARE(bitSet2 <= bitSet1, false);
    QCOMPARE(bitSet2 <= bitSet2, true);
    QCOMPARE(bitSet2 <= bitSet3, true);
    QCOMPARE(bitSet2 <= bitSet4, true);

    QCOMPARE(bitSet3 <= bitSet1, false);
    QCOMPARE(bitSet3 <= bitSet2, false);
    QCOMPARE(bitSet3 <= bitSet3, true);
    QCOMPARE(bitSet3 <= bitSet4, true);

    QCOMPARE(bitSet4 <= bitSet1, false);
    QCOMPARE(bitSet4 <= bitSet2, false);
    QCOMPARE(bitSet4 <= bitSet3, false);
    QCOMPARE(bitSet4 <= bitSet4, true);


    QCOMPARE(bitSet1 >= bitSet1, true);
    QCOMPARE(bitSet1 >= bitSet2, false);
    QCOMPARE(bitSet1 >= bitSet3, false);
    QCOMPARE(bitSet1 >= bitSet4, false);

    QCOMPARE(bitSet2 >= bitSet1, true);
    QCOMPARE(bitSet2 >= bitSet2, true);
    QCOMPARE(bitSet2 >= bitSet3, false);
    QCOMPARE(bitSet2 >= bitSet4, false);

    QCOMPARE(bitSet3 >= bitSet1, true);
    QCOMPARE(bitSet3 >= bitSet2, true);
    QCOMPARE(bitSet3 >= bitSet3, true);
    QCOMPARE(bitSet3 >= bitSet4, false);

    QCOMPARE(bitSet4 >= bitSet1, true);
    QCOMPARE(bitSet4 >= bitSet2, true);
    QCOMPARE(bitSet4 >= bitSet3, true);
    QCOMPARE(bitSet4 >= bitSet4, true);
}


void TestBitSet::testOtherOperators() {
    BitSet1      bitSet1("BIT1",         "BIT3", "BIT64", "BIT65", "BIT201" );
    BitSet1      bitSet2("BIT1", "BIT2", "BIT3",          "BIT65"           );

    Util::BitSet bitSet3 = bitSet1 & bitSet2;

    QSet<QString> compareSet;
    compareSet << "BIT1" << "BIT3" << "BIT65";

    QList<QString> bitNames = bitSet1.bits();
    for (QList<QString>::const_iterator it=bitNames.constBegin(),end=bitNames.constEnd() ; it!=end ; ++it) {
        QCOMPARE(bitSet3[*it], compareSet.contains(*it));
    }

    Util::BitSet bitSet4 = bitSet3 | BitSet1("BIT72");
    compareSet << "BIT72";

    for (QList<QString>::const_iterator it=bitNames.constBegin(),end=bitNames.constEnd() ; it!=end ; ++it) {
        QCOMPARE(bitSet4[*it], compareSet.contains(*it));
    }
}


void TestBitSet::testForwardIterator() {
    BitSet1 bitSet1;
    BitSet1 bitSet2("BIT1", "BIT3", "BIT64", "BIT65", "BIT201");

    Util::BitSetForwardIterator iterator1;
    QCOMPARE(iterator1.isEnd(), true);
    QCOMPARE(iterator1->isEmpty(), true);

    Util::BitSetForwardIterator iterator2(bitSet1);
    QCOMPARE(iterator2.isEnd(), true);
    QCOMPARE(iterator2->isEmpty(), true);

    Util::BitSetForwardIterator iterator3(bitSet2);
    QCOMPARE(iterator3.isEnd(), false);

    QCOMPARE(iterator3->isSet("BIT1"), true);
    unsigned index;
    for (index=2 ; index<256 ; ++index) {
        QString bitName = QString("BIT%1").arg(index);
        QCOMPARE(iterator3->isSet(bitName), false);
    }

    Util::BitSetForwardIterator iterator4 = iterator3;
    QCOMPARE(iterator4.isEnd(), false);

    QCOMPARE(iterator4->isSet("BIT1"), true);
    for (index=2 ; index<256 ; ++index) {
        QString bitName = QString("BIT%1").arg(index);
        QCOMPARE(iterator4->isSet(bitName), false);
    }

    // This relies on an understanding of the Util::BitSet and Util::
    QList<QString> bitOrdering;
    bitOrdering << "BIT1" << "BIT3" << "BIT64" << "BIT65" << "BIT201";

    QList<QString>::const_iterator bitOrderingIterator    = bitOrdering.constBegin();
    QList<QString>::const_iterator bitOrderingEndIterator = bitOrdering.constEnd();
    Util::BitSetForwardIterator    iterator(bitSet2);

    bool usePreIncrement = true;
    while (iterator.isNotEnd() && bitOrderingIterator != bitOrderingEndIterator) {
        Util::BitSet reportedBit = *iterator;
        for (index=1 ; index<=256 ; ++index) {
            QString bitName     = QString("BIT%1").arg(index);
            bool    shouldBeSet = (bitName == *bitOrderingIterator);
            QCOMPARE(reportedBit.isSet(bitName), shouldBeSet);
        }

        ++bitOrderingIterator;

        if (usePreIncrement) {
            ++iterator;
        } else {
            iterator++;
        }

        usePreIncrement = !usePreIncrement;
    }

    QCOMPARE(iterator.isEnd(), true);
    QCOMPARE(bitOrderingIterator, bitOrderingEndIterator);
}


void TestBitSet::testReverseIterator() {
    BitSet1 bitSet1;
    BitSet1 bitSet2("BIT1", "BIT3", "BIT64", "BIT65", "BIT201");

    Util::BitSetReverseIterator iterator1;
    QCOMPARE(iterator1.isEnd(), true);
    QCOMPARE(iterator1->isEmpty(), true);

    Util::BitSetReverseIterator iterator2(bitSet1);
    QCOMPARE(iterator2.isEnd(), true);
    QCOMPARE(iterator2->isEmpty(), true);

    Util::BitSetReverseIterator iterator3(bitSet2);
    QCOMPARE(iterator3.isEnd(), false);

    unsigned index;
    for (index=1 ; index<=256 ; ++index) {
        QString bitName = QString("BIT%1").arg(index);
        QCOMPARE(iterator3->isSet(bitName), index == 201);
    }

    Util::BitSetReverseIterator iterator4 = iterator3;
    QCOMPARE(iterator4.isEnd(), false);

    for (index=1 ; index<=256 ; ++index) {
        QString bitName = QString("BIT%1").arg(index);
        QCOMPARE(iterator4->isSet(bitName), index == 201);
    }

    // This relies on an understanding of the Util::BitSet and Util::
    QList<QString> bitOrdering;
    bitOrdering << "BIT201" << "BIT65" << "BIT64" << "BIT3" << "BIT1";

    QList<QString>::const_iterator bitOrderingIterator    = bitOrdering.constBegin();
    QList<QString>::const_iterator bitOrderingEndIterator = bitOrdering.constEnd();
    Util::BitSetReverseIterator    iterator(bitSet2);

    bool usePreIncrement = true;
    while (iterator.isNotEnd() && bitOrderingIterator != bitOrderingEndIterator) {
        Util::BitSet reportedBit = *iterator;
        for (index=1 ; index<=256 ; ++index) {
            QString bitName     = QString("BIT%1").arg(index);
            bool    shouldBeSet = (bitName == *bitOrderingIterator);
            QCOMPARE(reportedBit.isSet(bitName), shouldBeSet);
        }

        ++bitOrderingIterator;

        if (usePreIncrement) {
            ++iterator;
        } else {
            iterator++;
        }

        usePreIncrement = !usePreIncrement;
    }

    QCOMPARE(iterator.isEnd(), true);
    QCOMPARE(bitOrderingIterator, bitOrderingEndIterator);
}
