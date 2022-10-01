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
* This header provides a small set of useful bit manipulation functions.
***********************************************************************************************************************/

/* .. sphinx-project ineutil */

#ifndef UTIL_BIT_FUNCTIONS_H
#define UTIL_BIT_FUNCTIONS_H

#include <cstdint>
#include <type_traits>

#include "util_common.h"

namespace Util {
    /**
     * Function that calculates the number of ones in a 32-bit value using the variable SWAR algorithm.  The
     * implementation is based on the write-up at http://aggregate.org/MAGIC/.
     *
     * \param[in] value The value to determine the number of ones in.
     *
     * \return Returns the number of ones in the provided value.
     */
    UTIL_PUBLIC_API unsigned numberOnes32(std::uint32_t value);

    /**
     * Function that calculates the number of ones in a 64-bit value using the variable SWAR algorithm.
     *
     * \param[in] value The value to determine the number of ones in.
     *
     * \return Returns the number of ones in the provided value.
     */
    UTIL_PUBLIC_API unsigned numberOnes64(std::uint64_t value);

    /**
     * Function that calculates the location of the MSB of a 32-bit value.
     *
     *  param[in] value The value to determine the MSB location of.
     *
     * \return Returns the power of 2 indicating the location.  A value of -1 is returned for the value 0.
     */
    UTIL_PUBLIC_API int msbLocation32(std::uint32_t value);

    /**
     * Function that calculates the location of the MSB of a 64-bit value.  The function is similar to msbLocation32
     * except that it operates on 64-bit values rather than 32-bit values.
     *
     *  param[in] value The value to determine the MSB location of.
     *
     * \return Returns the power of 2 indicating the location.  A value of -1 is returned for the value 0.
     */
    UTIL_PUBLIC_API int msbLocation64(std::uint64_t value);

    /**
     * Template function that creates a mask with a single "1" at the least significant "1" in a number.
     *
     * \param[in] value The value to generate the LSB mask for.
     *
     * \return Returns the LSB mask for the value.
     */
    template<typename T> UTIL_PUBLIC_TEMPLATE_METHOD T maskLsbOne(T value) {
        return ((~value) ^ static_cast<T>(0-value)) & value;
    }

    /**
     * Template function that creates a mask with a single "1" at the least significant "0" in a number.
     *
     * \param[in] value The value to generate the LSB mask for.
     *
     * \return Returns the LSB mask for the value.
     */
    template<typename T> UTIL_PUBLIC_TEMPLATE_METHOD T maskLsbZero(T value) {
        return (value ^ (value + 1)) & (~value);
    }

    /**
     * Template method that creates a mask with a single "1" at the most significant "1" in a number.  Shamelessly
     * lifted from http://aggregate.org/MAGIC/.  Algorithm is modified to work on values up-to 64-bits in size.
     *
     * \param[in] value The value to generate the MSB mask for.
     *
     * \return Returns the MSB mask for the value.
     */
    template<typename T> UTIL_PUBLIC_TEMPLATE_METHOD T maskMsbOne(T value) {
        if (std::is_signed<T>::value) {
            return maskMsbOne(static_cast<typename std::make_unsigned<T>::type>(value));
        } else {
            value |= (value >> 1);
            value |= (value >> 2);
            value |= (value >> 4);

            if (std::is_same<T, std::uint16_t>::value) {
                value |= (value >> 8);
            }

            if (std::is_same<T, std::uint32_t>::value) {
                value |= (value >> 8);
                value |= (value >> 16);
            }

            if (std::is_same<T, std::uint64_t>::value) {
                value |= (value >> 8);
                value |= (value >> 16);

                #if (defined(_WIN32) || defined(_WIN64))

                    // MSVC generates the crazy warning "shift count negative or too big, undefined behavior."
                    // Warning is, at best, wrong since the type of "value" is guaranteed to be 64-bits.
                    //
                    // We work around this on Windows by performing two smaller shifts.

                    value |= ((value >> 16) >> 16);

                #else

                    value |= (value >> 32);

                #endif
            }

            #if (defined(UINT128_MAX))

                // GCC supports a 128-bit type so we support it here.
                if (std::is_same<T, std::uint128_t>::value) {
                    value |= (value >> 8);
                    value |= (value >> 16);
                    value |= (value >> 32);
                    value |= (value >> 64);
                }

            #endif

            return value & ~(value >> 1);
        }
    }

    /**
     * Template method that creates a mask with a single "1" at the most significant "0" in a number.
     *
     * \param[in] value The value to generate the MSB mask for.
     *
     * \return Returns the MSB mask for the value.
     */
    template<typename T> UTIL_PUBLIC_TEMPLATE_METHOD T maskMsbZero(T value) {
        return maskMsbOne(~value);
    }

    /**
     * Template method that determines if a value is a power of 2.
     *
     * \param[in] value The value to be tested.
     *
     * \return Returns true if the value is a power of 2.  Returns false if the value is the sum of multiple powers of
     *         2.
     */
    template<typename T> UTIL_PUBLIC_TEMPLATE_METHOD bool isPowerOf2(T value) {
        return maskLsbOne(value) == value;
    }
};

#endif
