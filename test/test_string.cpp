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
* This file implements tests of the String class
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QtTest/QtTest>
#include <QString>
#include <QStringList>
#include <QList>
#include <QSet>
#include <QFont>
#include <QFontDatabase>
#include <QFontMetricsF>

#include <QDebug> // Debug

#include <limits>
#include <random>

#include <util_string.h>

#include "test_string.h"

TestString::TestString() {}


TestString::~TestString() {}


void TestString::initTestCase() {}


void TestString::testProcessorSignError() {
    // Background:
    // I've caught a scenario with Xeon E5 on MacOS where the sign bit is not being updated when we negate a negative
    // zero value.  Test text checks this case in a more controlled manner.
    //
    // At this time, I don't know if this is the processor, the compiler, some interaction, or just BFO.

    long double infinity     = std::numeric_limits<long double>::infinity();
    long double negativeOne  = -1.0L;
    long double negativeZero = negativeOne / infinity;

    QCOMPARE(negativeZero, -0.0L);
    QCOMPARE(std::signbit(negativeZero), true);

    long double positiveZero = -negativeZero;
    QCOMPARE(positiveZero, +0.0L);
    QCOMPARE(std::signbit(positiveZero), false);
}


void TestString::testLongLongIntgerToUnicodeString() {
    QCOMPARE(Util::longLongIntegerToUnicodeString(123456789, Util::IntegerNumberStyle::NONE), QString("123456789"));

    QCOMPARE(
        Util::longLongIntegerToUnicodeString(
            123456789,
            Util::IntegerNumberStyle::NONE,
            Util::ignoreNumberDigits,
            2
        ),
        QString("111010110111100110100010101")
    );
    QCOMPARE(
        Util::longLongIntegerToUnicodeString(
            123456789,
            Util::IntegerNumberStyle::NONE,
            Util::ignoreNumberDigits,
            10
        ),
        QString("123456789")
    );
    QCOMPARE(
        Util::longLongIntegerToUnicodeString(
            123456789,
            Util::IntegerNumberStyle::NONE,
            Util::ignoreNumberDigits,
            16
        ),
        QString("75BCD15")
    );

    QCOMPARE(
        Util::longLongIntegerToUnicodeString(
            123456789,
            Util::IntegerNumberStyle::C_STYLE,
            Util::ignoreNumberDigits,
            2
        ),
        QString("0b111010110111100110100010101")
    );
    QCOMPARE(
        Util::longLongIntegerToUnicodeString(
            123456789,
            Util::IntegerNumberStyle::C_STYLE,
            Util::ignoreNumberDigits,
            10
        ),
        QString("123456789")
    );
    QCOMPARE(
        Util::longLongIntegerToUnicodeString(
            123456789,
            Util::IntegerNumberStyle::C_STYLE,
            Util::ignoreNumberDigits,
            16
        ),
        QString("0x75BCD15")
    );

    QCOMPARE(
        Util::longLongIntegerToUnicodeString(
            123456789,
            Util::IntegerNumberStyle::VERILOG_STYLE,
            Util::ignoreNumberDigits,
            2
        ),
        QString("'b111010110111100110100010101")
    );
    QCOMPARE(
        Util::longLongIntegerToUnicodeString(
            123456789,
            Util::IntegerNumberStyle::VERILOG_STYLE,
            Util::ignoreNumberDigits,
            10
        ),
        QString("'d123456789")
    );
    QCOMPARE(
        Util::longLongIntegerToUnicodeString(
            123456789,
            Util::IntegerNumberStyle::VERILOG_STYLE,
            Util::ignoreNumberDigits,
            16
        ),
        QString("'h75BCD15")
    );

    QCOMPARE(
        Util::longLongIntegerToUnicodeString(
            123456789,
            Util::IntegerNumberStyle::SUBSCRIPT_STYLE,
            Util::ignoreNumberDigits,
            2
        ),
        QString("111010110111100110100010101") + QChar(0x2082)
    );
    QCOMPARE(
        Util::longLongIntegerToUnicodeString(
            123456789,
            Util::IntegerNumberStyle::SUBSCRIPT_STYLE,
            Util::ignoreNumberDigits,
            10
        ),
        QString("123456789") + QChar(0x2081) + QChar(0x2080)
    );
    QCOMPARE(
        Util::longLongIntegerToUnicodeString(
            123456789,
            Util::IntegerNumberStyle::SUBSCRIPT_STYLE,
            Util::ignoreNumberDigits,
            16
        ),
        QString("75BCD15") + QChar(0x2081) + QChar(0x2086)
    );
}


void TestString::testFixedPoint() {
    QCOMPARE(Util::toUnicodeFixedPoint(10.0L/3.0L, 0), QString("3"));
    QCOMPARE(Util::toUnicodeFixedPoint(10.0L/3.0L, 1), QString("3.3"));
    QCOMPARE(Util::toUnicodeFixedPoint(10.0L/3.0L, 2), QString("3.33"));
    QCOMPARE(Util::toUnicodeFixedPoint(10.0L/3.0L, 3), QString("3.333"));

    QCOMPARE(Util::toUnicodeFixedPoint(20.0L/3.0L, 0), QString("7"));
    QCOMPARE(Util::toUnicodeFixedPoint(20.0L/3.0L, 1), QString("6.7"));
    QCOMPARE(Util::toUnicodeFixedPoint(20.0L/3.0L, 2), QString("6.67"));
    QCOMPARE(Util::toUnicodeFixedPoint(20.0L/3.0L, 3), QString("6.667"));


    QCOMPARE(Util::toUnicodeFixedPoint(100.0L/3.0L, 0), QString("33"));
    QCOMPARE(Util::toUnicodeFixedPoint(100.0L/3.0L, 1), QString("33.3"));
    QCOMPARE(Util::toUnicodeFixedPoint(100.0L/3.0L, 2), QString("33.33"));
    QCOMPARE(Util::toUnicodeFixedPoint(100.0L/3.0L, 3), QString("33.333"));

    QCOMPARE(Util::toUnicodeFixedPoint(200.0L/3.0L, 0), QString("67"));
    QCOMPARE(Util::toUnicodeFixedPoint(200.0L/3.0L, 1), QString("66.7"));
    QCOMPARE(Util::toUnicodeFixedPoint(200.0L/3.0L, 2), QString("66.67"));
    QCOMPARE(Util::toUnicodeFixedPoint(200.0L/3.0L, 3), QString("66.667"));


    QString r = Util::toUnicodeFixedPoint(10.0L/3.0L);
    #if (defined(Q_OS_WIN))

        QCOMPARE(r, QString("3.3333333333333335"));

    #elif (defined(Q_OS_DARWIN) || defined(Q_OS_LINUX))

        QCOMPARE(r, QString("3.3333333333333333333"));

    #else

        #error Unknown platform

    #endif

    r = Util::toUnicodeFixedPoint(1.23456000000000078L);
    QCOMPARE(r, QString("1.23456"));

    r = Util::toUnicodeFixedPoint(1.2345600000000078L);
    QCOMPARE(r, QString("1.23456"));

    r = Util::toUnicodeFixedPoint(1.234560000000078L);
    QCOMPARE(r, QString("1.23456"));

    r = Util::toUnicodeFixedPoint(1.234560000000078L);
    QCOMPARE(r, QString("1.23456"));

    r = Util::toUnicodeFixedPoint(1.23456000000078L);
    QCOMPARE(r, QString("1.23456"));

    r = Util::toUnicodeFixedPoint(1.2345600000078L);
    QCOMPARE(r, QString("1.2345600000078"));

    r = Util::toUnicodeFixedPoint(1.23456999999999978L);
    QCOMPARE(r, QString("1.23457"));

    r = Util::toUnicodeFixedPoint(1.2345699999999978L);
    QCOMPARE(r, QString("1.23457"));

    r = Util::toUnicodeFixedPoint(1.234569999999978L);
    QCOMPARE(r, QString("1.23457"));

    r = Util::toUnicodeFixedPoint(1.234569999999978L);
    QCOMPARE(r, QString("1.23457"));

    r = Util::toUnicodeFixedPoint(1.23456999999978L);
    QCOMPARE(r, QString("1.23457"));

    r = Util::toUnicodeFixedPoint(1.2345699999978L);
    QCOMPARE(r, QString("1.23457"));

    r = Util::toUnicodeFixedPoint(1.234569999978L);
    QCOMPARE(r, QString("1.234569999978"));

    long double one  = 1.0L;
    long double zero = 0.0L;

    QCOMPARE(Util::toUnicodeFixedPoint(zero),  QString("0"));
    QCOMPARE(Util::toUnicodeFixedPoint(zero, 1),  QString("0.0"));
    QCOMPARE(Util::toUnicodeFixedPoint(one/(one/zero)), QString("0"));
    QCOMPARE(Util::toUnicodeFixedPoint((-one)/(one/zero)), QString("-0"));

    QCOMPARE(Util::toUnicodeFixedPoint(zero/zero),  QString("NaN"));
    QCOMPARE(Util::toUnicodeFixedPoint(-zero/zero), QString("NaN"));
    QCOMPARE(Util::toUnicodeFixedPoint(one/zero), QString(QChar(0x221E)));
    QCOMPARE(Util::toUnicodeFixedPoint((-one)/zero), QString("-") + QChar(0x221E));
}


void TestString::testConvertToMantissaAndExponent() {
    long double mantissa;
    int         exponent;

    long double epsilon = std::numeric_limits<long double>::epsilon();
    Util::convertToMantissaAndExponent(1.25E5L, mantissa, exponent);
    QVERIFY(std::abs(mantissa - 1.25L) < 10.0 * epsilon);
    QCOMPARE(exponent, +5);

    Util::convertToMantissaAndExponent(-1.25E-5L, mantissa, exponent);
    QVERIFY(std::abs(mantissa - (-1.25L)) < 10.0 * epsilon);
    QCOMPARE(exponent, -5);

    Util::convertToMantissaAndExponent(0.0L, mantissa, exponent);
    QCOMPARE(mantissa, +0.0L);
    QCOMPARE(exponent,  0);

    Util::convertToMantissaAndExponent(-0.0L, mantissa, exponent);
    QCOMPARE(mantissa, -0.0L);
    QCOMPARE(exponent,  0);

    long double one      = 1.0L;
    long double zero     = 0.0L;
    long double nan      = zero / zero;
    long double infinity = one / zero;

    Util::convertToMantissaAndExponent(nan, mantissa, exponent);
    QCOMPARE(std::isnan(mantissa), true);
    QCOMPARE(exponent,  0);

    Util::convertToMantissaAndExponent(+infinity, mantissa, exponent);
    QCOMPARE(!std::isfinite(mantissa), true);
    QCOMPARE(std::signbit(mantissa), false);
    QCOMPARE(exponent,  0);

    Util::convertToMantissaAndExponent(-infinity, mantissa, exponent);
    QCOMPARE(!std::isfinite(mantissa), true);
    QCOMPARE(std::signbit(mantissa), true);
    QCOMPARE(exponent,  0);
}


void TestString::testLongDoubleToUnicodeString() {
    QString result = Util::longDoubleToUnicodeString(1.25E+5, Util::RealNumberStyle::FLOATING_POINT, 4);
    QCOMPARE(result, QString("125000.0000"));

    result = Util::longDoubleToUnicodeString(-1.25E-5, Util::RealNumberStyle::FLOATING_POINT, 4);
    QCOMPARE(result, QString("-0.0000"));

    result = Util::longDoubleToUnicodeString(-1.25E-5, Util::RealNumberStyle::FLOATING_POINT, 7);
    QCOMPARE(result, QString("-0.0000125"));

    result = Util::longDoubleToUnicodeString(-1.25E-5, Util::RealNumberStyle::SCIENTIFIC, Util::inferPrecision);
    QCOMPARE(result, QString("-1.25") + QChar(0x00D7) + QString("10") + QChar(0x207B) + QChar(0x2075));

    result = Util::longDoubleToUnicodeString(-1.25E-5, Util::RealNumberStyle::ENGINEERING, Util::inferPrecision);
    QCOMPARE(result, QString("-12.5") + QChar(0x00D7) + QString("10") + QChar(0x207B) + QChar(0x2076));
}


void TestString::testOffsetIntoString() {
    std::mt19937                            rng;
    std::uniform_int_distribution<unsigned> numberWordsDistribution(1U, 20U);
    std::uniform_int_distribution<unsigned> wordLengthDistribution(1U, 20U);
    std::uniform_int_distribution<unsigned> letterDistribution(33, 127);

    QFontDatabase fontDatabase;
    QStringList   fontFamilies = fontDatabase.families();
    std::uniform_int_distribution<unsigned> familyDistribution(0, fontFamilies.size() - 1);

    for (unsigned i=0 ; i<numberIterations ; ++i) {
        unsigned numberWords = numberWordsDistribution(rng);
        QString  s;

        for (unsigned wordIndex=0 ; wordIndex<numberWords ; ++wordIndex) {
            if (wordIndex != 0) {
                s += ' ';
            }

            unsigned wordLength = wordLengthDistribution(rng);
            for (unsigned characterIndex=0 ; characterIndex<wordLength ; ++characterIndex) {
                unsigned unicode = letterDistribution(rng);
                s += QChar(unicode);
            }
        }

        unsigned stringLength = static_cast<unsigned>(s.length());
        std::uniform_int_distribution<unsigned> indexDistribution(0, stringLength);
        unsigned expectedIndex = indexDistribution(rng);

        QString familyName = fontFamilies.at(familyDistribution(rng));
        QFont font(familyName);
        QFontMetricsF fontMetrics(font);

        float offset = fontMetrics.horizontalAdvance(s.left(expectedIndex));
        unsigned measuredIndex = Util::offsetIntoString(s, font, offset);

        QCOMPARE(measuredIndex, expectedIndex);

        std::uniform_int_distribution<unsigned> indexDistribution2(0, stringLength - 1);
        unsigned lowerIndex    = indexDistribution2(rng);
        unsigned upperIndex    = lowerIndex + 1;

        float lowerOffset = fontMetrics.horizontalAdvance(s.left(lowerIndex));
        float upperOffset = fontMetrics.horizontalAdvance(s.left(upperIndex));
        float offset1     = lowerOffset + (upperOffset - lowerOffset) * 0.45;
        float offset2     = lowerOffset + (upperOffset - lowerOffset) * 0.55;

        unsigned lowerMeasuredIndex = Util::offsetIntoString(s, font, offset1);
        unsigned upperMeasuredIndex = Util::offsetIntoString(s, font, offset2);

        QCOMPARE(lowerMeasuredIndex, lowerIndex);
        QCOMPARE(upperMeasuredIndex, upperIndex);
    }
}
