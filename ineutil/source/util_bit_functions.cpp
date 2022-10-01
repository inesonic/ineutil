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
* This file implements a small set of useful bit manipulation functions.
***********************************************************************************************************************/

#include <cstdint>

#include "util_common.h"
#include "util_bit_functions.h"

namespace Util {
    unsigned numberOnes32(std::uint32_t value) {
        std::uint32_t x = value;

        x -= ((x >> 1) & 0x55555555);
        x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
        x = (((x >> 4) + x) & 0x0F0F0F0F);
        x += (x >> 8);
        x += (x >> 16);

        return(x & 0x0000003f);
    }


    unsigned numberOnes64(std::uint64_t value) {
        std::uint64_t x = value;

        x -= ((x >> 1) & 0x5555555555555555ULL);
        x = (((x >> 2) & 0x3333333333333333ULL) + (x & 0x3333333333333333ULL));
        x = (((x >> 4) + x) & 0x0F0F0F0F0F0F0F0FULL);
        x += (x >> 8);
        x += (x >> 16);
        x += (x >> 32);

        return(x & 0x0000007f);
    }


    int msbLocation32(std::uint32_t value) {
        int msbLocation = 0;

        if (value) {
            unsigned adjustment = 32;
            std::uint32_t runningValue = value;

            while (adjustment) {
                std::uint32_t mask = ((1ULL << adjustment) - 1) << adjustment;
                if (runningValue & mask) {
                    runningValue >>= adjustment;
                    msbLocation += adjustment;
                }

                adjustment >>= 1;
            }
        } else {
            msbLocation = -1;
        }

        return msbLocation;
    }


    int msbLocation64(std::uint64_t value) {
        int msbLocation = 0;

        if (value) {
            unsigned adjustment = 64;
            std::uint64_t runningValue = value;

            while (adjustment) {
                std::uint64_t mask = ((1ULL << adjustment) - 1) << adjustment;
                if (runningValue & mask) {
                    runningValue >>= adjustment;
                    msbLocation += adjustment;
                }

                adjustment >>= 1;
            }
        } else {
            msbLocation = -1;
        }

        return msbLocation;
    }
}
