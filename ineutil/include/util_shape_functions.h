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
* This header provides a small set of useful functions to perform calculations on common shapes.
***********************************************************************************************************************/

/* .. sphinx-project ineutil */

#ifndef UTIL_SHAPE_FUNCTIONS_H
#define UTIL_SHAPE_FUNCTIONS_H

#include <QPointF>
#include <QLineF>
#include <QRectF>
#include <QPolygonF>

#include "util_common.h"

namespace Util {
    /**
     * Type used to define point values.
     */
    typedef QPointF Point;

    /**
     * Type used to define lines.
     */
    typedef QLineF Line;

    /**
     * Type used to define rectangles.
     */
    typedef QRectF Rectangle;

    /**
     * Type used to define arbitrary polygons.
     */
    typedef QPolygonF Polygon;

    /**
     * Method that calculates the distance between two points.
     *
     * \param[in] point1 The first point.
     *
     * \param[in] point2 The second point.
     *
     * \return Returns the distance.
     */
    double UTIL_PUBLIC_API distance(const Point& point1, const Point& point2);

    /**
     * Method that calculates the shortest distance between a point and a line.
     *
     * \param[in] point The point
     *
     * \param[in] line  The line
     *
     * \return Returns the shortest distance.
     */
    double UTIL_PUBLIC_API shortestDistance(const Point& point, const Line& line);

    /**
     * Method that calculates the shortest distance between a point and a line.
     *
     * \param[in] line  The line
     *
     * \param[in] point The point
     *
     * \return Returns the shortest distance.
     */
    double UTIL_PUBLIC_API shortestDistance(const Line& line, const Point& point);

    /**
     * Method that calculates the shortest distance between a point and a rectangle.
     *
     * \param[in]  point       The point
     *
     * \param[in]  rectangle   The rectangle
     *
     * \param[out] closestLine An optional line instance that will be populated with the cloest line.
     *
     * \return Returns the shortest distance to the edge of the rectangle.
     */
    double UTIL_PUBLIC_API shortestDistance(
        const Point&     point,
        const Rectangle& rectangle,
        Line*            closestLine = nullptr
    );

    /**
     * Method that calculates the shortest distance between a point and a rectangle.
     *
     * \param[in]  point       The point
     *
     * \param[in]  rectangle   The rectangle
     *
     * \param[out] closestLine An optional line instance that will be populated with the cloest line.
     */
    double UTIL_PUBLIC_API shortestDistance(
        const Rectangle& rectangle,
        const Point&     point,
        Line*            closestLine = nullptr
    );

    /**
     * Method that calculates the shortest distance between a point and a polygon.
     *
     * \param[in]  point       The point
     *
     * \param[in]  polygon     The polygon
     *
     * \param[out] closestLine An optional line instance that will be populated with the cloest line.
     *
     * \return Returns the shortest distance to the edge of the polygon.
     */
    double UTIL_PUBLIC_API shortestDistance(
        const Point&   point,
        const Polygon& polygon,
        Line*          closestLine = nullptr
    );

    /**
     * Method that calculates the shortest distance between a point and a polygon.
     *
     * \param[in]  point       The point
     *
     * \param[in]  polygon     The polygon
     *
     * \param[out] closestLine An optional line instance that will be populated with the cloest line.
     *
     * \return Returns the shortest distance to the edge of the polygon.
     */
    double UTIL_PUBLIC_API shortestDistance(
        const Polygon& polygon,
        const Point&   point,
        Line*          closestLine = nullptr
    );

    /**
     * Method that calculates the closest position on a line to a specified point.
     *
     * \param[in] point The point.
     *
     * \param[in] line  The line.
     *
     * \return Returns the point on the line closest to the indicated point.
     */
    Point UTIL_PUBLIC_API closestPoint(const Point& point, const Line& line);

    /**
     * Method that calculates the closest position on a line to a specified point.
     *
     * \param[in] line  The line.
     *
     * \param[in] point The point.
     *
     * \return Returns the point on the line closest to the indicated point.
     */
    Point UTIL_PUBLIC_API closestPoint(const Line& line, const Point& point);
};

#endif
