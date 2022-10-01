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
* This header provides a small set of useful functions to manipulate colors.
***********************************************************************************************************************/

/* .. sphinx-project ineutil */

#ifndef UTIL_COLOR_FUNCTIONS_H
#define UTIL_COLOR_FUNCTIONS_H

#include <QColor>

#include "util_common.h"

namespace Util {
    /**
     * Type used to define color values.
     */
    typedef QColor Color;

    /**
     * Method that blends two color values.
     *
     * \param[in] color1 The first color to blend.
     *
     * \param[in] color2 The second color to blend.
     *
     * \return Returns the blended color.
     */
    UTIL_PUBLIC_API Color blend(const Color& color1, const Color& color2);

    /**
     * Method that blends three color values.
     *
     * \param[in] color1 The first color to blend.
     *
     * \param[in] color2 The second color to blend.
     *
     * \param[in] color3 The third color to blend.
     *
     * \return Returns the blended color.
     */
    UTIL_PUBLIC_API Color blend(const Color& color1, const Color& color2, const Color& color3);

    /**
     * Method that blends four color values.
     *
     * \param[in] color1 The first color to blend.
     *
     * \param[in] color2 The second color to blend.
     *
     * \param[in] color3 The third color to blend.
     *
     * \param[in] color4 The fourth color to blend.
     *
     * \return Returns the blended color.
     */
    UTIL_PUBLIC_API Color blend(const Color& color1, const Color& color2, const Color& color3, const Color& color4);

    /**
     * Method that applies painter's algorithm to two colors.
     *
     * \param[in] firstCoat  The first color to apply.
     *
     * \param[in] secondCoat The second color to apply.
     *
     * \return Returns the resulting background color.
     */
    UTIL_PUBLIC_API Color paintersAlgorithm(const Color& firstCoat, const Color& secondCoat);

    /**
     * Method that returns a CSS compatible "rbga" color definition.
     *
     * \param[in] color The color to translate.
     *
     * \return Returns the color in CSS rgba format.
     */
    UTIL_PUBLIC_API QString toRgba(const Color& color);

    /**
     * Method that calculates the luminance for a color.
     *
     * \param[in] color The color to calculate the luminance of.
     *
     * \return The luminance.
     */
    UTIL_PUBLIC_API double luminance(const Color& color);
};

#endif
