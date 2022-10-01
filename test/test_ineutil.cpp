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
* This file is the main entry point for the ineutil unit tests.
***********************************************************************************************************************/

#include <QGuiApplication>
#include <QtTest/QtTest>

#include "test_bit_functions.h"
#include "test_bit_set.h"
#include "test_bit_array.h"
#include "test_page_size.h"
#include "test_string.h"
#include "test_fuzzy_search.h"

#define TEST(_X) {                                                  \
    _X _x;                                                          \
    testStatus |= QTest::qExec(&_x, argumentCount, argumentValues); \
}

int main(int argumentCount, char** argumentValues) {
    QGuiApplication applicationInstance(argumentCount, argumentValues); // This is needed for QFont and QFontDatabase.

    int testStatus = 0;

    TEST(TestBitFunctions);
    TEST(TestBitSet);
    TEST(TestBitArray);
    TEST(TestPageSize);
    TEST(TestString);
    TEST(TestFuzzySearch);

    return testStatus;
}
