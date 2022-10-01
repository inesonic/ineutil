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
* This header defines a small collection of nice algorithm functions.
***********************************************************************************************************************/

/* .. sphinx-project ineutil */

#ifndef UTIL_ALGORITHM_H
#define UTIL_ALGORITHM_H

#include <QString>
#include <QHash>
#include <QList>

#include <cstdint>

#include "util_common.h"

namespace Util {
    /**
     * Template method find the maximum of two values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns the maximum of the two values.
     */
    template <typename T> UTIL_PUBLIC_TEMPLATE_METHOD T max(const T& a, const T& b) {
        return a > b ? a : b;
    }

    /**
     * Template method that finds the maximum of multiple values.
     *
     * \param[in] a   The first value to compare.
     *
     * \param[in] b   The second value to compare.
     *
     * \param[in] c2z The remaining values to compare.
     *
     * \return Returns the maximum of the supplied values.
     */
    template <typename T1, typename... T2> UTIL_PUBLIC_TEMPLATE_METHOD T1 max(
            const T1&    a,
            const T1&    b,
            const T2&... c2z
        ) {
        return max(max(a, b), c2z...);
    }

    /**
     * Template method find the minimum of two values.
     *
     * \param[in] a The first value to compare.
     *
     * \param[in] b The second value to compare.
     *
     * \return Returns the minimum of the two values.
     */
    template <typename T> UTIL_PUBLIC_TEMPLATE_METHOD T min(const T& a, const T& b) {
        return a < b ? a : b;
    }

    /**
     * Template method that finds the minimum of multiple values.
     *
     * \param[in] a   The first value to compare.
     *
     * \param[in] b   The second value to compare.
     *
     * \param[in] c2z The remaining values to compare.
     *
     * \return Returns the minimum of the supplied values.
     */
    template <typename T1, typename... T2> UTIL_PUBLIC_TEMPLATE_METHOD T1 min(
            const T1&    a,
            const T1&    b,
            const T2&... c2z
        ) {
        return min(min(a, b), c2z...);
    }
}

#endif
