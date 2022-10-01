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
* This file implements a small set of useful functions to perform calculations on common shapes.
***********************************************************************************************************************/

#include <QPointF>
#include <QLineF>
#include <QRectF>
#include <QPolygonF>

#include <limits>
#include <cmath>
#include <algorithm>

#include "util_common.h"
#include "util_shape_functions.h"

namespace Util {
    double distance(const Point& point1, const Point& point2) {
        double dx = point1.x() - point2.x();
        double dy = point1.y() - point2.y();
        return std::sqrt(dx * dx + dy * dy);
    }


    double shortestDistance(const Point& point, const Line& line) {
        double result;
        Point  p1 = line.p1();
        Point  p2 = line.p2();

        if (p1 == p2) {
            result = distance(p1, point);
        } else {
            double x0 = point.x();
            double y0 = point.y();
            double x1 = p1.x();
            double y1 = p1.y();
            double x2 = p2.x();
            double y2 = p2.y();

            double ly = y2 - y1;
            double lx = x2 - x1;

            result = std::abs(ly * x0 - lx * y0 + x2 * y1 - y2 * x1) / std::sqrt(ly * ly + lx * lx);
        }

        return result;
    }


    double shortestDistance(const Line& line, const Point& point) {
        return shortestDistance(point, line);
    }


    double UTIL_PUBLIC_API shortestDistance(const Point& point, const Rectangle& rectangle, Line* closestLine) {
        double result;

        if (rectangle.contains(point)) {
            result = 0;

            if (closestLine != nullptr) {
                *closestLine = Line(point, point);
            }
        } else {
            Point closestPointOnRectangle(
                std::min(std::max(rectangle.left(), point.x()), rectangle.right()),
                std::min(std::max(rectangle.top(), point.y()), rectangle.bottom())
            );

            double dx = closestPointOnRectangle.x() - point.x();
            double dy = closestPointOnRectangle.y() - point.y();

            result = std::sqrt(dx * dx + dy * dy);

            if (closestLine != nullptr) {
                *closestLine = Line(closestPointOnRectangle, point);
            }
        }

        return result;
    }


    double UTIL_PUBLIC_API shortestDistance(const Rectangle& rectangle, const Point& point, Line* closestLine) {
        return shortestDistance(point, rectangle, closestLine);
    }


    double UTIL_PUBLIC_API shortestDistance(const Point& point, const Polygon& polygon, Line* closestLine) {
        double result = std::numeric_limits<double>::max();
        Line   bestLine;

        unsigned numberVertices = static_cast<unsigned>(polygon.size());
        if (numberVertices == 1) {
            result = distance(point, polygon.first());
        } else if (numberVertices > 1) {
            Point p1 = polygon.first();
            for (unsigned vertexNumber=1 ; vertexNumber<numberVertices ; ++vertexNumber) {
                Point  p2 = polygon.at(vertexNumber);
                Line   line(p1, p2);
                double d  = shortestDistance(point, line);

                if (d < result) {
                    bestLine = line;
                    result   = d;
                }

                p1 = p2;
            }
        }

        if (closestLine != nullptr) {
            *closestLine = bestLine;
        }

        return result;
    }


    double UTIL_PUBLIC_API shortestDistance(const Polygon& polygon, const Point& point, Line* closestLine) {
        return shortestDistance(point, polygon, closestLine);
    }


    Point UTIL_PUBLIC_API closestPoint(const Point& point, const Line& line) {
        Point  result;

        Point  p1 = line.p1();
        Point  p2 = line.p2();

        double x0 = point.x();
        double y0 = point.y();
        double x1 = p1.x();
        double y1 = p1.y();
        double x2 = p2.x();
        double y2 = p2.y();

        if (x1 == x2) {
            double yMin = std::min(y1, y2);
            double yMax = std::max(y1, y2);

            if        (y0 < yMin) {
                result = Point(x1, yMin);
            } else if (y0 > yMax) {
                result = Point(x1, yMax);
            } else {
                result = Point(x1, y0);
            }
        } else {
            double xMin = std::min(x1, x2);
            double xMax = std::max(x1, x2);

            if (y1 == y2) {
                if        (x0 < xMin) {
                    result = Point(xMin, y1);
                } else if (x0 > xMax) {
                    result = Point(xMax, y1);
                } else {
                    result = Point(x0, y1);
                }
            } else {
                double ml = (y2 - y1) / (x2 - y1); // slope of line.
                double bl = y1 - ml * x1;          // y offset of line.
                double bp = y0 + x0 / ml;          // y offset of perpendicular line that intersects our point.

                double xi = (ml * (bp - bl)) / (ml * ml + 1); // x coordinate of closest point.

                if (xi < xMin) {
                    result = x1 == xMin ? p1 : p2;
                } else if (xi > xMax) {
                    result = x1 == xMax ? p1 : p2;
                }

                double yi = ml * xi + bl;                     // y coordinate of closest point.
                result = Point(xi, yi);
            }
        }

        return result;
    }


    Point UTIL_PUBLIC_API closestPoint(const Line& line, const Point& point) {
        return closestPoint(point, line);
    }
}
