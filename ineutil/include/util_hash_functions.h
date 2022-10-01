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
* This header provides a qHash functions for common Qt and intrinsic data types.  Hash functions that are tied to
* data types in other libraries shall not be included here.
*
* Note that due to the fact that many hash functions a referenced by templated classes, versions are defined both
* inside and outside of the Util namespace.
***********************************************************************************************************************/

/* .. sphinx-project ineutil */

#ifndef UTIL_HASH_FUNCTIONS_H
#define UTIL_HASH_FUNCTIONS_H

#include <QColor>
#include <cstdint>

#include "util_common.h"

namespace Util {
    #if (QT_VERSION < 0x060000)

        /**
         * Type used to represent a hash seed.
         */
        typedef unsigned HashSeed;

        /**
         * Type used to represent the result of a hash function.
         */
        typedef unsigned HashResult;

    #else

        /**
         * Type used to represent a hash seed.
         */
        typedef size_t HashSeed;

        /**
         * Type used to represent the result of a hash function.
         */
        typedef size_t HashResult;

    #endif

    /**
     * Hash function for the Qt QColor type.
     *
     * \param[in] color The color value to be hashed.
     *
     * \param[in] seed  An optional hash seed.
     *
     * \return Returns a hash calculated from the value and seed.
     */
    UTIL_PUBLIC_API HashResult qHash(const QColor& color, HashSeed seed = 0);
}

/**
 * Hash function for the Qt QColor type.
 *
 * \param[in] color The color value to be hashed.
 *
 * \param[in] seed  An optional hash seed.
     *
     * \return Returns a hash calculated from the value and seed.
 */
UTIL_PUBLIC_API inline Util::HashResult qHash(const QColor& color, Util::HashSeed seed = 0) {
    return Util::qHash(color, seed);
}

#endif
