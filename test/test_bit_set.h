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
* This header provides tests for the BitSet class.
***********************************************************************************************************************/

#ifndef TEST_BIT_SET_H
#define TEST_BIT_SET_H

#include <QtGlobal>
#include <QObject>
#include <QtTest/QtTest>

#include <util_bit_set.h>

class BitSet1:public Util::BitSet {
    private:
        static BitNameHash bitNameHash;

    public:
        BitSet1();

        ~BitSet1();

        BitSet1(const Util::BitSet& other):BitSet(other) {
            Q_ASSERT(tracksSameBitsAs(other));
        }

        BitSet1(const BitSet1& other):BitSet(other) {}

        template<typename T> BitSet1(const T& value):BitSet(&bitNameHash) {
            setBit(value, true);
        }

        template<typename T1, typename... T2> BitSet1(const T1& value1, const T2&... values):BitSet(&bitNameHash) {
            setBit(value1, true);
            setBits(values...);
        }

        static bool assignBit(const QString& bitName);
};

class BitSet2:public Util::BitSet {
    public:
        BitSet2();

        ~BitSet2();

        static bool assignBit(const QString& bitName);

    private:
        static BitNameHash bitNameHash;
};

class TestBitSet:public QObject {
    Q_OBJECT

    public:
        TestBitSet();

        ~TestBitSet() override;

    private:
        static const unsigned numberIterations = 100000;

    private slots:
        void initTestCase();
        void testConstructors();
        void testSingleBitSetClearMethods();
        void testTemplateMethods();
        void testIntersectionMethod();
        void testUnionMethod();
        void testMethodOperators();
        void testComparisonOperators();
        void testOtherOperators();
        void testForwardIterator();
        void testReverseIterator();
};

#endif
