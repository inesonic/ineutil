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
* This header provides several useful unit conversions required throughout the application.
***********************************************************************************************************************/

/* .. sphinx-project ineutil */

#ifndef UTIL_UNITS_H
#define UTIL_UNITS_H

#include <cmath>

namespace Util {
    /**
     * Namespace containing constants used for unit conversion.
     */
    namespace Units {
        /**
         * Number of mm per inch.
         */
        static constexpr double mmPerInch = 25.4;

        /**
         * Number of inches per mm.
         */
        static constexpr double inchesPerMm = 1.0 / mmPerInch;

        /**
         * Number of mm per cm.
         */
        static constexpr double mmPerCm = 10.0;

        /**
         * Number of cm per mm.
         */
        static constexpr double cmPerMm = 1.0 / mmPerCm;

        /**
         * Number of cm per inch.
         */
        static constexpr double cmPerInch = mmPerInch / mmPerCm;

        /**
         * Number of inches per cm.
         */
        static constexpr double inchesPerCm = 1.0 / cmPerInch;

        /**
         * Number of points per inch.
         */
        static constexpr double pointsPerInch = 72.0;

        /**
         * Number of points per mm.
         */
        static constexpr double pointsPerMm = 2.8346456692944;

        /**
         * Number of inches per point.
         */
        static constexpr double inchesPerPoint = 1.0 / pointsPerInch;

        /**
         * Number of mm per point.
         */
        static constexpr double mmPerPoint = 1.0 / pointsPerMm;

        /**
         * Number of points per cm.
         */
        static constexpr double pointsPerCm = 10.0 * pointsPerMm;

        /**
         * Number of cm per point.
         */
        static constexpr double cmPerPoint = 1.0 / pointsPerCm;

        /**
         * Standard epsilon used to compare point values.
         */
        static constexpr double pointEpsilon = 0.01;

        /**
         * Method you can use to compare two point dimension values.
         *
         * \param[in] value1 The first value to compare.
         *
         * \param[in] value2 The second value to compare.
         *
         * \return Returns true if the point values are essentially equivalent.  Returns false if the point values are
         *         not functionally equivalent.
         */
        static inline bool pointValuesEqual(double value1, double value2) {
            return fabs(value1 - value2) <= pointEpsilon;
        }

        /**
         * Method you can use to compare two point dimension values.
         *
         * \param[in] value1 The first value to compare.
         *
         * \param[in] value2 The second value to compare.
         *
         * \return Returns true if the point values are not functionally equivalent.  Returns false if the point values
         *         are functionally equivalent.
         */
        static inline bool pointValuesNotEqual(double value1, double value2) {
            return fabs(value1 - value2) > pointEpsilon;
        }
    }
}

#endif
