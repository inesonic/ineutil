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
* This file implements a small set of useful functions to manipulate colors.
***********************************************************************************************************************/

#include <QColor>

#include "util_common.h"
#include "util_color_functions.h"

namespace Util {
    static bool sumColor(const Color& color, float& sumAlpha, float& sumRed, float& sumGreen, float& sumBlue) {
        bool result;

        if (color.isValid()) {
            float alpha = static_cast<float>(color.alphaF());

            sumAlpha += alpha;
            sumRed   += static_cast<float>(color.redF()) * alpha;
            sumGreen += static_cast<float>(color.greenF()) * alpha;
            sumBlue  += static_cast<float>(color.blueF()) * alpha;

            result = true;
        } else {
            result = false;
        }

        return result;
    }


    Color blend(const Color& color1, const Color& color2) {
        Color result;

        float    sumAlpha    = 0.0F;
        float    sumRed      = 0.0F;
        float    sumGreen    = 0.0F;
        float    sumBlue     = 0.0F;
        unsigned numberValid = 0;

        if (sumColor(color1, sumAlpha, sumRed, sumGreen, sumBlue)) {
            ++numberValid;
        }

        if (sumColor(color2, sumAlpha, sumRed, sumGreen, sumBlue)) {
            ++numberValid;
        }

        if (numberValid > 0) {
            result = QColor::fromRgbF(
                sumRed / sumAlpha,
                sumGreen / sumAlpha,
                sumBlue / sumAlpha,
                sumAlpha / static_cast<float>(numberValid)
            );
        }

        return result;
    }


    Color blend(const Color& color1, const Color& color2, const Color& color3) {
        Color result;

        float    sumAlpha    = 0.0F;
        float    sumRed      = 0.0F;
        float    sumGreen    = 0.0F;
        float    sumBlue     = 0.0F;
        unsigned numberValid = 0;

        if (sumColor(color1, sumAlpha, sumRed, sumGreen, sumBlue)) {
            ++numberValid;
        }

        if (sumColor(color2, sumAlpha, sumRed, sumGreen, sumBlue)) {
            ++numberValid;
        }

        if (sumColor(color3, sumAlpha, sumRed, sumGreen, sumBlue)) {
            ++numberValid;
        }

        if (numberValid > 0) {
            result = QColor::fromRgbF(
                sumRed / sumAlpha,
                sumGreen / sumAlpha,
                sumBlue / sumAlpha,
                sumAlpha / static_cast<float>(numberValid)
            );
        }

        return result;
    }


    Color blend(const Color& color1, const Color& color2, const Color& color3, const Color& color4) {
        Color result;

        float    sumAlpha    = 0.0F;
        float    sumRed      = 0.0F;
        float    sumGreen    = 0.0F;
        float    sumBlue     = 0.0F;
        unsigned numberValid = 0;

        if (sumColor(color1, sumAlpha, sumRed, sumGreen, sumBlue)) {
            ++numberValid;
        }

        if (sumColor(color2, sumAlpha, sumRed, sumGreen, sumBlue)) {
            ++numberValid;
        }

        if (sumColor(color3, sumAlpha, sumRed, sumGreen, sumBlue)) {
            ++numberValid;
        }

        if (sumColor(color4, sumAlpha, sumRed, sumGreen, sumBlue)) {
            ++numberValid;
        }

        if (numberValid > 0) {
            result = QColor::fromRgbF(
                sumRed / sumAlpha,
                sumGreen / sumAlpha,
                sumBlue / sumAlpha,
                sumAlpha / static_cast<float>(numberValid)
            );
        }

        return result;
    }


    Color paintersAlgorithm(const Color& firstCoat, const Color& secondCoat) {
        float firstRed    = firstCoat.redF();
        float firstGreen  = firstCoat.greenF();
        float firstBlue   = firstCoat.blueF();
        float firstAlpha  = firstCoat.alphaF();

        float secondRed   = secondCoat.redF();
        float secondGreen = secondCoat.greenF();
        float secondBlue  = secondCoat.blueF();
        float secondAlpha = secondCoat.alphaF();

        float d             = secondAlpha + firstAlpha * (1.0F - secondAlpha);
        float combinedRed   = (secondRed * secondAlpha + firstRed * firstAlpha * (1.0F - secondAlpha)) / d;
        float combinedGreen = (secondGreen * secondAlpha + firstGreen * firstAlpha * (1.0F - secondAlpha)) / d;
        float combinedBlue  = (secondBlue * secondAlpha + firstBlue * firstAlpha * (1.0F - secondAlpha)) / d;

        float combinedAlpha = secondAlpha + firstAlpha * (1.0F - secondAlpha);

        return Color::fromRgbF(combinedRed, combinedGreen, combinedBlue, combinedAlpha);
    }


    QString toRgba(const Color& color) {
        int red;
        int green;
        int blue;
        int alpha;

        color.getRgb(&red, &green, &blue, &alpha);
        return QString("rgba(%1,%2,%3,%4)").arg(red).arg(green).arg(blue).arg(alpha);
    }


    double luminance(const Color& color) {
        return 0.2126 * color.redF() + 0.7152 * color.greenF() + 0.0722 * color.blueF();
    }
}
