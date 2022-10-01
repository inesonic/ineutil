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
* This header defines a small collection of string functions.
***********************************************************************************************************************/

/* .. sphinx-project ineutil */

#ifndef UTIL_STRING_H
#define UTIL_STRING_H

#include <QChar>
#include <QString>
#include <QFont>

#include "util_common.h"
#include "util_hash_functions.h"

namespace Util {
    /**
     * The unicode character type.
     */
    typedef QChar Char;

    /**
     * String type.
     */
    typedef QString String;

    /**
     * The font type.
     */
    typedef QFont Font;

    #if (QT_VERSION < 0x060000)

        typedef QStringRef StringView;

    #else

        typedef QStringView StringView;

    #endif

    /**
     * Value indicating that the number of integer digits should be ignored.
     */
    static constexpr unsigned ignoreNumberDigits = 0;

    /**
     * Value indicate the default base.  Base prefixes and suffixes will not be applied to this base.
     */
    static constexpr unsigned defaultIntegerBase = 10;

    /**
     * Value indicating that the precision should be inferred.
     */
    static constexpr unsigned inferPrecision = static_cast<unsigned>(-1);

    /**
     * Enumeration of supported boolean presentation styles.
     */
    enum class BooleanStyle {
        /**
         * Indicates an unknown/invalid boolean presentation style.
         */
        INVALID,

        /**
         * Indicates lower-case true/false.
         */
        LOWER_CASE_TRUE_FALSE,

        /**
         * Indicates title case true/false.
         */
        TITLE_CASE_TRUE_FALSE,

        /**
         * Indicates upper-case true/false.
         */
        UPPER_CASE_TRUE_FALSE,

        /**
         * Indicates lower-case yes/no.
         */
        LOWER_CASE_YES_NO,

        /**
         * Indicates title case yes/no.
         */
        TITLE_CASE_YES_NO,

        /**
         * Indicates upper-case yes/no.
         */
        UPPER_CASE_YES_NO,

        /**
         * Indicates 1/0 values.
         */
        NUMERIC_1_0
    };

    /**
     * Enumeration of supported integer styles.
     */
    enum class IntegerNumberStyle {
        /**
         * Value indicating an invalid prefix value.
         */
        INVALID,

        /**
         * Value indicating no prefix value.
         */
        NONE,

        /**
         * Value indicating C style base prefix values (e.g. 0x12AF).  Note that this will default to NONE for
         * bases other than 2, and 16.
         */
        C_STYLE,

        /**
         * Value indicating Verilog style (e.g. 'h12AF).    Note that this will default to NONE for bases other
         * than 2, 8, and 16.
         */
        VERILOG_STYLE,

        /**
         * Value indicating base subscript style.
         */
        SUBSCRIPT_STYLE
    };

    /**
     * Enumeration of supported real value notations.
     */
    enum class RealNumberStyle {
        /**
         * Value indicating an invalid notation.
         */
        INVALID,

        /**
         * Value indicating traditional floating point format, for example "12345.6".
         */
        FLOATING_POINT,

        /**
         * Value indicating natural exponential notation, for example "1.23456x10^4".
         */
        SCIENTIFIC,

        /**
         * Value indicating engineering notation, for example "12.3456x10^3".
         */
        ENGINEERING,

        /**
         * Value indicating computer centric exponential notation, for example "1.23456E4".
         */
        COMPUTER_SCIENTIFIC,

        /**
         * Value indicating the most concise format should be used.
         */
        CONCISE,

        /**
         * Value indicating the most concise computer centric format should be used.
         */
        COMPUTER_CONCISE
    };

    /**
     * Function you can use to translate a boolean to a string in one of a number of possible notations.
     *
     * \param[in] value        The value to be converted to a string.
     *
     * \param[in] booleanStyle The style to be applied.
     *
     * \return Returns the number translated to a string.
     */
    UTIL_PUBLIC_API String booleanToUnicodeString(bool value, BooleanStyle booleanStyle);

    /**
     * Function you can use to translate a long long integer to a Unicode string in one of a number of possible
     * notations.
     *
     *  param[in] value              The value to be converted to a string.
     *
     *  param[in] integerNumberStyle The number style to be applied.
     *
     *  param[in] numberDigits       The number of digits to insert into the generated value.  A value of
     *                               ignoreNumberDigits will cause the number to use the minimum number of digits
     *                               required to represent the value.
     *
     *  param[in] base               The number base.
     *
     *  param[in] upperCase          If true, alphapbetic digits will be displayed in upper-case.  If false, alphabetic
     *                               digits will be displayed in lower case.
     *
     * \return Returns the number translated to a string.
     */
    UTIL_PUBLIC_API String longLongIntegerToUnicodeString(
        long long          value,
        IntegerNumberStyle integerNumberStyle,
        unsigned           numberDigits = ignoreNumberDigits,
        unsigned           base = defaultIntegerBase,
        bool               upperCase = true
    );

    /**
     * Function you can use to convert a long double value to a unicode string in fixed point notation.  Unicode will
     * be used to represent infinity.
     *
     * \param[in] value     The value to be converted.
     *
     * \param[in] precision The precision to apply.
     *
     * \return Returns the value converted to a string.
     */
    UTIL_PUBLIC_API String toUnicodeFixedPoint(long double value, unsigned precision = inferPrecision);

    /**
     * Function you can use to translate a value into a mantissa and floating point value suitable for use in either
     * scientific notation or engineering notation.
     *
     * \param[in]  value               The value to be translated into its components.
     *
     * \param[out] mantissa            Location to store the translated mantissa.  This value will be positive
     *                                 infinity if the provided value is infinte.  This value will be NaN if the
     *                                 provided value is NaN.
     *
     * \param[out] exponent            Location to store thr translated exponent.   This value will be 0 for infinite
     *                                 and NaN values.
     *
     * \param[in]  engineeringNotation Boolean value holding true if the value should be converted to a mantissa and
     *                                 exponent suitable for use in engineering notation.
     */
    UTIL_PUBLIC_API void convertToMantissaAndExponent(
        long double  value,
        long double& mantissa,
        int&         exponent,
        bool         engineeringNotation = false
    );

    /**
     * Function you can use to translate a long double to a Unicode string in one of a number of possible notations.
     *
     *  param[in] value           The value to be converted to a string.
     *
     *  param[in] realNumberStyle The number style to be applied.
     *
     *  param[in] precision       The desired precision.  You can use inferPrecision to indicate that this function
     *                            should pick an appropriate precision.
     *
     *  param[out] mantissaString String that will contain the mantissa on exit.
     *
     *  param[out] exponentString String that will contain the exponent on exit.  The exponent will be an empty string
     *                            if no exponent is needed.
     */
    UTIL_PUBLIC_API void longDoubleToMantissaAndExponentStrings(
        long double     value,
        RealNumberStyle realNumberStyle,
        unsigned        precision,
        String&         mantissaString,
        String&         exponentString
    );

    /**
     * Function you can use to translate a long double to a Unicode string in one of a number of possible notations.
     *
     *  param[in] value           The value to be converted to a string.
     *
     *  param[in] realNumberStyle The number style to be applied.
     *
     *  param[in] precision       The desired precision.  You can use inferPrecision to indicate that this function
     *                            should pick an appropriate precision.
     *
     *  param[in] upperCase       If true, the "E" used in computer scientific notation will be upper-case.  If false,
     *                            the "e" used in computer scientific notation will be lower-case.
     *
     * \return Returns the number translated to a string.
     */
    UTIL_PUBLIC_API String longDoubleToUnicodeString(
        long double     value,
        RealNumberStyle realNumberStyle,
        unsigned        precision,
        bool            upperCase = true
    );

    /**
     * Funciton you can use to calculate the character offset into a string based on a distance from the string's
     * left edge.  This function is essentially the inverse of QFontMetricsF::horizontalAdvance
     *
     * \param[in] str      The string to be tested.
     *
     * \param[in] font     The font used to render the string.
     *
     * \param[in] distance The distance in pixels from the string's left edge.
     *
     * \return Returns the character offset closest to the specified distance.
     */
    UTIL_PUBLIC_API unsigned offsetIntoString(const String& str, const Font& font, float distance);

    /**
     * Hash function for the BooleanStyle enumeration.
     *
     * \param[in] booleanValue The value to be hashed.
     *
     * \param[in] seed         An optional seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    UTIL_PUBLIC_API HashResult qHash(BooleanStyle booleanValue, HashSeed seed = 0);

    /**
     * Hash function for the IntegerNumberStyle enumeration.
     *
     * \param[in] integerValue The value to be hashed.
     *
     * \param[in] seed         An optional seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    UTIL_PUBLIC_API HashResult qHash(IntegerNumberStyle integerValue, HashSeed seed = 0);

    /**
     * Hash function for the RealNumberStyle enumeration.
     *
     * \param[in] realValue The value to be hashed.
     *
     * \param[in] seed      An optional seed to apply to the hash.
     *
     * \return Returns a hash of the provided value.
     */
    UTIL_PUBLIC_API HashResult qHash(RealNumberStyle realValue, HashSeed seed = 0);

    #if (QT_VERSION >= 0x060000)

        /**
         * Overloaded comparison operator.
         *
         * \param[in] a The first value to compare.
         *
         * \param[in] b The second value to compare.
         *
         * \return Returns true if the strings are equal.  Returns false if the strings are not equal.
         */
        bool operator==(const StringView& a, const String& b);

        /**
         * Overloaded comparison operator.
         *
         * \param[in] a The first value to compare.
         *
         * \param[in] b The second value to compare.
         *
         * \return Returns true if the strings are equal.  Returns false if the strings are not equal.
         */
        bool operator==(const StringView& a, const char* b);

        /**
         * Overloaded comparison operator.
         *
         * \param[in] a The first value to compare.
         *
         * \param[in] b The second value to compare.
         *
         * \return Returns true if the strings are equal.  Returns false if the strings are not equal.
         */
        bool operator==(const StringView& a, const Char* b);

        /**
         * Overloaded comparison operator.
         *
         * \param[in] a The first value to compare.
         *
         * \param[in] b The second value to compare.
         *
         * \return Returns true if the strings are equal.  Returns false if the strings are not equal.
         */
        bool operator==(const String& a, const StringView& b);

        /**
         * Overloaded comparison operator.
         *
         * \param[in] a The first value to compare.
         *
         * \param[in] b The second value to compare.
         *
         * \return Returns true if the strings are equal.  Returns false if the strings are not equal.
         */
        bool operator==(const char* a, const StringView& b);

        /**
         * Overloaded comparison operator.
         *
         * \param[in] a The first value to compare.
         *
         * \param[in] b The second value to compare.
         *
         * \return Returns true if the strings are equal.  Returns false if the strings are not equal.
         */
        bool operator==(const Char* a, const StringView& b);

        /**
         * Overloaded comparison operator.
         *
         * \param[in] a The first value to compare.
         *
         * \param[in] b The second value to compare.
         *
         * \return Returns true if the strings are not equal.  Returns false if the strings are equal.
         */
        bool operator!=(const StringView& a, const String& b);

        /**
         * Overloaded comparison operator.
         *
         * \param[in] a The first value to compare.
         *
         * \param[in] b The second value to compare.
         *
         * \return Returns true if the strings are not equal.  Returns false if the strings are equal.
         */
        bool operator!=(const StringView& a, const char* b);

        /**
         * Overloaded comparison operator.
         *
         * \param[in] a The first value to compare.
         *
         * \param[in] b The second value to compare.
         *
         * \return Returns true if the strings are not equal.  Returns false if the strings are equal.
         */
        bool operator!=(const StringView& a, const Char* b);

        /**
         * Overloaded comparison operator.
         *
         * \param[in] a The first value to compare.
         *
         * \param[in] b The second value to compare.
         *
         * \return Returns true if the strings are not equal.  Returns false if the strings are equal.
         */
        bool operator!=(const String& a, const StringView& b);

        /**
         * Overloaded comparison operator.
         *
         * \param[in] a The first value to compare.
         *
         * \param[in] b The second value to compare.
         *
         * \return Returns true if the strings are not equal.  Returns false if the strings are equal.
         */
        bool operator!=(const char* a, const StringView& b);

        /**
         * Overloaded comparison operator.
         *
         * \param[in] a The first value to compare.
         *
         * \param[in] b The second value to compare.
         *
         * \return Returns true if the strings are not equal.  Returns false if the strings are equal.
         */
        bool operator!=(const Char* a, const StringView& b);

    #endif
}

#endif
