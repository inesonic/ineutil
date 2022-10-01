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
* This file implements a small collection of string functions.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QString>
#include <QChar>
#include <QHash>
#include <QFont>
#include <QFontMetricsF>
#include <QSet>

#include <cmath>
#include <limits>
#include <sstream>
#include <ios>
#include <iomanip>

#include "util_hash_functions.h"
#include "util_string.h"

namespace Util {
    static const String unicodeExponent = Char(0x00D7) + String("10");

    static String toSuperscript(const String& value) {
        String result;

        for (String::const_iterator it=value.constBegin(),end=value.constEnd() ; it!=end ; ++it) {
            Char character = *it;

            if (character == Char('-')) {
                result += Char(0x207B);
            } else if (character == Char('+')) {
                result += Char(0x207A);
            } else if (character.isDigit()) {
                unsigned digit = character.digitValue();
                if (digit == 1) {
                    result += Char(0x00B9);
                } else if (digit == 2) {
                    result += Char(0x00B2);
                } else if (digit == 3) {
                    result += Char(0x00B3);
                } else {
                    result += Char(0x2070 + digit);
                }
            } else {
                result += QChar(0x00BF);
            }
        }

        return result;
    }


    static long double roundTo(long double value, int fractionalDigits) {
        long double powerOf10 = std::pow(10.0L, static_cast<long double>(fractionalDigits));
        return std::round(powerOf10 * value) / powerOf10;
    }


    String booleanToUnicodeString(bool value, BooleanStyle booleanStyle) {
        String result;

        switch (booleanStyle) {
            case BooleanStyle::INVALID:
            case BooleanStyle::LOWER_CASE_TRUE_FALSE: {
                const char* booleanString = value ? QT_TR_NOOP("true") : QT_TR_NOOP("false");
                result = QCoreApplication::translate("Util::booleanToUnicodeString", booleanString);
                break;
            }

            case BooleanStyle::TITLE_CASE_TRUE_FALSE: {
                const char* booleanString = value ? QT_TR_NOOP("True") : QT_TR_NOOP("False");
                result = QCoreApplication::translate("Util::booleanToUnicodeString", booleanString);
                break;
            }

            case BooleanStyle::UPPER_CASE_TRUE_FALSE: {
                const char* booleanString = value ? QT_TR_NOOP("TRUE") : QT_TR_NOOP("FALSE");
                result = QCoreApplication::translate("Util::booleanToUnicodeString", booleanString);
                break;
            }

            case BooleanStyle::LOWER_CASE_YES_NO: {
                const char* booleanString = value ? QT_TR_NOOP("yes") : QT_TR_NOOP("no");
                result = QCoreApplication::translate("Util::booleanToUnicodeString", booleanString);
                break;
            }

            case BooleanStyle::TITLE_CASE_YES_NO: {
                const char* booleanString = value ? QT_TR_NOOP("Yes") : QT_TR_NOOP("No");
                result = QCoreApplication::translate("Util::booleanToUnicodeString", booleanString);
                break;
            }

            case BooleanStyle::UPPER_CASE_YES_NO: {
                const char* booleanString = value ? QT_TR_NOOP("YES") : QT_TR_NOOP("NO");
                result = QCoreApplication::translate("Util::booleanToUnicodeString", booleanString);
                break;
            }

            case BooleanStyle::NUMERIC_1_0: {
                const char* booleanString = value ? QT_TR_NOOP("1") : QT_TR_NOOP("0");
                result = QCoreApplication::translate("Util::booleanToUnicodeString", booleanString);
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return result;
    }


    QString longLongIntegerToUnicodeString(
            long long          value,
            IntegerNumberStyle integerNumberStyle,
            unsigned           numberDigits,
            unsigned           base,
            bool               upperCase
        ) {
        String result;

        String sign;
        if (value < 0) {
            value = -value;
            sign  = String("-");
        }

        result = String::number(value, base);

        if (upperCase) {
            result = result.toUpper();
        }

        unsigned naturalLength = static_cast<unsigned>(result.length());
        if (naturalLength < numberDigits) {
            String leadingZeros = String("0").repeated(numberDigits - naturalLength);
            result = leadingZeros + result;
        }

        switch (integerNumberStyle) {
            case IntegerNumberStyle::INVALID:
            case IntegerNumberStyle::NONE: {
                break;
            }

            case IntegerNumberStyle::C_STYLE: {
                if (base == 2) {
                    result = String("0b") + result;
                } else if (base == 16) {
                    result = String("0x") + result;
                }

                break;
            }

            case IntegerNumberStyle::VERILOG_STYLE: {
                if (base == 2) {
                    result = String("'b") + result;
                } else if (base == 8) {
                    result = String("'o") + result;
                } else if (base == 10) {
                    result = String("'d") + result;
                } else if (base == 16) {
                    result = String("'h") + result;
                }

                break;
            }

            case IntegerNumberStyle::SUBSCRIPT_STYLE: {
                unsigned b = base;
                String   suffix;
                while (b != 0) {
                    unsigned digit = b % 10;
                    suffix = QChar(0x2080 + digit) + suffix;
                    b /= 10;
                }

                result += suffix;

                break;
            }
        }

        result = sign + result;
        return result;
    }


    String toUnicodeFixedPoint(long double value, unsigned precision) {
        String      result;

        if (std::isnan(value)) {
            result = String("NaN");
        } else {
            bool        negative       = std::signbit(value);
            long double magnitudeValue = negative ? -value : value;

            if (!std::isfinite(value)) {
                result = String(Char(0x221E));
            } else {
                unsigned maximumNumberDigits = std::numeric_limits<long double>::digits10;
                unsigned usedPrecision;
                if (precision == inferPrecision) {
                    float logValue = magnitudeValue != static_cast<long double>(0) ? std::log10(magnitudeValue) : 0.0F;
                    int   exponent = static_cast<int>(logValue);

                    if (logValue < 0 && 1.0F * exponent != logValue) {
                        --exponent;
                    }

                    usedPrecision = std::max(0, -exponent);
                    while (roundTo(magnitudeValue, usedPrecision) != roundTo(magnitudeValue, usedPrecision + 6) &&
                           usedPrecision < std::numeric_limits<long double>::max_digits10                          ) {
                        ++usedPrecision;
                    }
                } else if (precision > maximumNumberDigits) {
                    usedPrecision = maximumNumberDigits;
                } else {
                    usedPrecision = precision;
                }

                std::locale locale("");
                std::stringstream stream;
                stream.imbue(locale);
                stream << std::fixed << std::setprecision(usedPrecision) << value;

                result = QString::fromStdString(stream.str());

                if (result.endsWith(String(".")) || result.endsWith(String(","))) {
                    result = result.left(result.length() - 1);
                }
            }

            // There's a bug on MacOS that causes -(-0) to remain -0.  We address that bug here.
            if (negative && result.at(0) != QChar('-')) {
                result = QString("-") + result;
            }
        }

        return result;
    }


    void convertToMantissaAndExponent(
            long double  value,
            long double& mantissa,
            int&         exponent,
            bool         engineeringNotation
        ) {
        if (std::isnan(value) || !std::isfinite(value)) {
            mantissa   = value;
            exponent   = 0;
        } else {
            bool negative = std::signbit(value);

            long double magnitudeValue = negative ? -value : value;
            float       logValue       = magnitudeValue != 0.0F ? std::log10(magnitudeValue) : 0.0F;

            exponent = static_cast<int>(logValue);
            if (logValue < 0 && 1.0F * exponent != logValue) {
                --exponent;
            }

            if (engineeringNotation) {
                unsigned exponentAdjustment = 0;
                if (exponent > 0) {
                    exponentAdjustment = static_cast<unsigned>(exponent % 3);
                } else if (exponent < 0) {
                    exponentAdjustment = static_cast<unsigned>(3 + (exponent % 3)) % 3;
                }

                exponent -= exponentAdjustment;
            }

            mantissa = value * std::pow(10.0L, static_cast<long double>(-exponent));
        }
    }


    void longDoubleToMantissaAndExponentStrings(
            long double     value,
            RealNumberStyle realNumberStyle,
            unsigned        precision,
            String&         mantissaString,
            String&         exponentString
        ) {
        switch (realNumberStyle) {
            case RealNumberStyle::FLOATING_POINT: {
                mantissaString = toUnicodeFixedPoint(value, precision);
                exponentString.clear();

                break;
            }

            case RealNumberStyle::SCIENTIFIC:
            case RealNumberStyle::COMPUTER_SCIENTIFIC: {
                long double mantissa;
                int         exponent;

                convertToMantissaAndExponent(value, mantissa, exponent);
                mantissaString = toUnicodeFixedPoint(mantissa, precision);
                exponentString = String::number(exponent);

                break;
            }

            case RealNumberStyle::ENGINEERING: {
                long double mantissa;
                int         exponent;

                convertToMantissaAndExponent(value, mantissa, exponent, true);

                mantissaString = toUnicodeFixedPoint(mantissa, precision);
                exponentString = String::number(exponent);

                break;
            }

            case RealNumberStyle::INVALID:
            case RealNumberStyle::CONCISE: {
                long double mantissa;
                int         exponent;

                convertToMantissaAndExponent(value, mantissa, exponent);

                mantissaString = toUnicodeFixedPoint(mantissa, precision);
                exponentString = QString::number(exponent);;

                QString fixedPoint = toUnicodeFixedPoint(value, precision);
                QString scientific = mantissaString + unicodeExponent + exponentString;

                if (fixedPoint.length() <= scientific.length()) {
                    mantissaString = fixedPoint;
                    exponentString.clear();
                }

                break;
            }

            case RealNumberStyle:: COMPUTER_CONCISE: {
                long double mantissa;
                int         exponent;

                convertToMantissaAndExponent(value, mantissa, exponent);

                mantissaString = toUnicodeFixedPoint(mantissa, precision);
                exponentString = QString::number(exponent);

                QString fixedPoint = toUnicodeFixedPoint(value, precision);
                QString scientific = String("%1E%2").arg(mantissaString).arg(exponentString);

                if (fixedPoint.length() <= scientific.length()) {
                    mantissaString = fixedPoint;
                    exponentString.clear();
                }

                break;
            }
        }
    }


    String longDoubleToUnicodeString(
            long double     value,
            RealNumberStyle realNumberStyle,
            unsigned        precision,
            bool            upperCase
        ) {
        String result;

        QString mantissaString;
        QString exponentString;
        longDoubleToMantissaAndExponentStrings(value, realNumberStyle, precision, mantissaString, exponentString);

        if (!exponentString.isEmpty()) {
            if (realNumberStyle == RealNumberStyle::COMPUTER_CONCISE    ||
                realNumberStyle == RealNumberStyle::COMPUTER_SCIENTIFIC    ) {
                result =   upperCase
                         ? String("%1E%2").arg(mantissaString).arg(exponentString)
                         : String("%1e%2").arg(mantissaString).arg(exponentString);
            } else {
                result = mantissaString + unicodeExponent + toSuperscript(exponentString);
            }
        } else {
            result = mantissaString;
        }

        return result;
    }


    unsigned offsetIntoString(const String& str, const Font& font, float distance) {
        unsigned result;

        unsigned stringLength = static_cast<unsigned>(str.length());
        if (distance <= 0 || stringLength == 0) {
            result = 0;
        } else {
            QFontMetricsF fontMetrics(font);

            // We use Newton-Raphson method to quickly locate the closest position.  Sometimes, due to integer
            // round-off, Newton-Raphson does not converge.  When we detect this, we switch to a binary search
            // algorithm.  This approach has the advantage of getting us close fast while avoiding the convergense
            // issue we see with Newton-Raphson.

            float    stringExtent     = fontMetrics.horizontalAdvance(str);
            unsigned lowerIndex       = 0;
            float    lowerExtent      = 0;
            unsigned upperIndex       = stringLength;
            float    upperExtent      = stringExtent;
            bool     useBinarySearch  = false;
            unsigned guessIndex;
            float    guessExtent;

            do {
                if (useBinarySearch) {
                    guessIndex = (upperIndex + lowerIndex) / 2;
                } else {
                    float inverseSlope = (1.0 * (upperIndex - lowerIndex)) / (upperExtent - lowerExtent);

                    guessIndex = static_cast<unsigned>(inverseSlope * (distance - lowerExtent) + lowerIndex);
                    if (guessIndex > stringLength) {
                        guessIndex = stringLength;
                    }
                }

                guessExtent = fontMetrics.horizontalAdvance(str.left(guessIndex));

                if (guessExtent < distance) {
                    if (guessIndex == lowerIndex) {
                        useBinarySearch = true;
                    } else {
                        lowerIndex  = guessIndex;
                        lowerExtent = guessExtent;
                    }
                } else {
                    if (guessIndex == upperIndex) {
                        useBinarySearch = true;
                    } else {
                        upperIndex  = guessIndex;
                        upperExtent = guessExtent;
                    }
                }
            } while ((upperIndex - lowerIndex) > 1);

            if (std::abs(distance - upperExtent) < std::abs(distance - lowerExtent)) {
                result = upperIndex;
            } else {
                result = lowerIndex;
            }
        }

        return result;
    }


    HashResult qHash(BooleanStyle value, HashSeed seed) {
        return ::qHash(static_cast<unsigned>(value), seed);
    }


    HashResult qHash(IntegerNumberStyle integerNumberStyle, HashSeed seed) {
        return ::qHash(static_cast<unsigned>(integerNumberStyle), seed);
    }


    HashResult qHash(RealNumberStyle realNumberStyle, HashSeed seed) {
        return ::qHash(static_cast<unsigned>(realNumberStyle), seed);
    }

    #if (QT_VERSION >= 0x060000)

        bool operator==(const StringView& a, const String& b) {
            return a.toString() == b;
        }

        bool operator==(const StringView& a, const char* b) {
            return a.toString() == String(b);
        }


        bool operator==(const StringView& a, const Char* b) {
            return a.toString() == String(b);
        }


        bool operator==(const String& a, const StringView& b) {
            return a == b.toString();
        }


        bool operator==(const char* a, const StringView& b) {
            return String(a) == b.toString();
        }


        bool operator==(const Char* a, const StringView& b) {
            return QString(a) == b.toString();
        }


        bool operator!=(const StringView& a, const String& b) {
            return !(a == b);
        }


        bool operator!=(const StringView& a, const char* b) {
            return !(a == b);
        }


        bool operator!=(const StringView& a, const Char* b) {
            return !(a == b);
        }


        bool operator!=(const String& a, const StringView& b) {
            return !(a == b);
        }


        bool operator!=(const char* a, const StringView& b) {
            return !(a == b);
        }


        bool operator!=(const Char* a, const StringView& b) {
            return !(a == b);
        }

    #endif
}
