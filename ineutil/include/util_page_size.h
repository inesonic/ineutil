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
* This header defines the \ref Util::PageSize class.
***********************************************************************************************************************/

/* .. sphinx-project ineutil */

#ifndef UTIL_PAGE_SIZE_H
#define UTIL_PAGE_SIZE_H

#include <QtGlobal>
#include <QCoreApplication> // For Q_DECLARE_TR_FUNCTIONS
#include <QSharedPointer>
#include <QWeakPointer>
#include <QString>
#include <QList>
#include <QPageSize>

#include "util_common.h"
#include "util_hash_functions.h"

namespace Util {
    /**
     * Class that tracks information about the size of a page.
     */
    class UTIL_PUBLIC_API PageSize {
        Q_DECLARE_TR_FUNCTIONS(Util::PageSize)

        public:
            /**
             * Enumeration of different regions employing paper sizes.
             */
            enum class Region:quint8 {
                /**
                 * Indicates paper from any region.
                 */
                AllRegions,

                /**
                 * Indicates US centric paper size.
                 */
                Us,

                /**
                 * Indicates Britain.
                 */
                Britain,

                /**
                 * Indicates european paper size.
                 */
                Europe,

                /**
                 * Indicates Japanese paper size.
                 */
                Japan,

                /**
                 * Indicates china paper size.
                 */
                China
            };

            /**
             * The default page size to use when creating a page.  The default will be set based on locale.
             */
            static const PageSize defaultPageSize;

            /**
             * Constructor, creates an invalid page size.
             */
            PageSize();

            /**
             * Constructor.
             *
             * \param[in] widthPoints  The page width, in points.
             *
             * \param[in] heightPoints The page height, in points.
             */
            PageSize(double widthPoints, double heightPoints);

            /**
             * Constructor
             *
             * \param[in] name The name used to describe the page.  If the page size is unknown, the created page will
             *                 be marked as invalid.
             */
            explicit PageSize(const QString& name);

            /**
             * Constructor
             *
             * \param[in] name The name used to describe the page.  If the page size is unknown, the created page will
             *                 be marked as invalid.
             */
            explicit PageSize(const char* name);

            /**
             * Copy Constructor.
             *
             * \param[in] other The page size instance to copy.
             */
            PageSize(const PageSize& other);

            /**
             * Constructor.
             *
             * \param[in] qPageSize An instance of QPageSize that can be used to initialize this object.
             */
            PageSize(const QPageSize& qPageSize);

            ~PageSize();

            /**
             * Method that indicates if this page size is valid or invalid.
             *
             * \return Returns true if the page size is valid.  Returns false if the page size is invalid.
             */
            bool isValid() const;

            /**
             * Method that indicates if this page size is invalid or valid.
             *
             * \return Returns true if the page size is invalid.  Returns false if the page size is valid.
             */
            bool isInvalid() const;

            /**
             * Method that sets the page size based on the page dimensions, in points.
             *
             * \param[in] newWidthPoints The new page width, in points.
             *
             * \param[in] newHeightPoints The new page height, in points.
             */
            void setPageSize(double newWidthPoints, double newHeightPoints);

            /**
             * Method that sets the page size based on a standard page name.
             *
             * \param[in] name The name to assign to the page.
             */
            void setPageFromName(const QString& name);

            /**
             * Method that sets the page size based on a standard page name.
             *
             * \param[in] name The name to assign to the page.
             */
            void setPageFromName(const char* name);

            /**
             * Method that obtains the page width, in points.
             *
             * \return Returns the page width, in points.
             */
            double width() const;

            /**
             * Method that obtains the page height, in points.
             *
             * \return Returns the page height, in points.
             */
            double height() const;

            /**
             * Method that returns a standardized name for the page.  If the page does not confirm to a standard size,
             * then the method will return a string of the form "Custom" where www is the width, in points and
             * hhh is the height, in points.
             *
             * \return Returns the standard name for this page.
             */
            QString name() const;

            /**
             * Method that returns a standardized description for the page.  If the page conforms to a standard size,
             * the the description will be the name follow by the dimensions in the specified units.  If the page does
             * not conform to a standard size, the the method will return a string of the form "Custom" followed by the
             * page width and height.
             *
             * \param[in] useMetric If true, the size will be presented in metric.  If false, the size will be presented
             *                      in inches.
             *
             * \return Returns the standard description for this page.
             */
            QString description(bool useMetric) const;

            /**
             * Method that returns a QPageSize instance that aligns with this page.
             *
             * \return Returns an instance of QPageSize that best fits this page.
             */
            QPageSize toQPageSize() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance of \ref PageSize to be copied.
             *
             * \return Returns a reference to this class instance.
             */
            PageSize& operator=(const PageSize& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance of \ref PageSize to be compared against.
             *
             * \return Returns true if the two instances represent the same page size.  Returns false if the two
             *         instances represent different page sizes.
             */
            bool operator==(const PageSize& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance of \ref PageSize to be compared against.
             *
             * \return Returns true if the two instances represent different page sizes.  Returns false if the two
             *         instances represent the same page size.
             */
            bool operator!=(const PageSize& other) const;

            /**
             * Method that creates a string identifier suitable for recording details about this page size.  The
             * generated string is suitable for reconstructing this \ref PageSize instance at a later point in time
             * using the static \ref PageSize::fromString method.
             *
             * \return Returns a string representing the pageSize value.
             */
            QString toString() const;

            /**
             * Method that converts a string identifier to a \ref PageSize instance.
             *
             * \param[in]  identifier The string identifier for the page size.
             *
             * \param[out] ok         Optional pointer to an instance of bool used to determine if the string identifier is
             *                        correct.  The boolean will be set to true if the string is valid and will be set to
             *                        false if the string is invalid.  You can provide a null pointer if you don't need to
             *                        determine validity.
             *
             * \return Returns a QPageLayout described by the string.
             */
            static PageSize fromString(const QString& identifier, bool* ok = nullptr);

            /**
             * Returns a list of standard page sizes, by name.  Names are adjusted for locale.
             *
             * \param[in] region The region to obtain standard paper sizes for.
             *
             * \return Returns a list of names for standard page sizes.
             */
            static QList<QString> names(Region region = Region::AllRegions);

            /**
             * Returns a list of standard page sizes, by description.  The descriptions are adjusted for locale and
             * requested units.
             *
             * \param[in] region    The region to obtain standard paper sizes for.
             *
             * \param[in] useMetric If true, descriptions will use metric units.
             *
             * \return Returns a list of names for standard page sizes.
             */
            static QList<QString> descriptions(Region region, bool useMetric);

            /**
             * Returns a \ref PageSize instance from a name.
             *
             * \param[in] name The name of the desired page size.
             */
            static PageSize fromName(const QString& name);

            /**
             * Returns a \ref PageSize instance from a description.
             *
             * \param[in] description The name of the desired page size.
             */
            static PageSize fromDescription(const QString& description);

        private:
            /**
             * The name of the page size for the default page.  The value is locale specific.
             */
            static const QString defaultPageSizeName;

            /**
             * Method that returns the best default page for the given localization.
             *
             * \return Return a pre-configured \ref PageSize instance suitable for this domain.
             */
            static PageSize calculateDefaultPageSize();

            /**
             * Returns a Qt page size ID based on a page name.
             *
             * \param[in] pageName The page name.
             *
             * \return Returns the ID associated with the page name. A value of QPageSize::Custom is returned if the name
             *         does not match a known page.
             */
            static QPageSize::PageSizeId idFromName(const QString& pageName);

            /**
             * The underlying QPageSize instance that this class marshalls.
             */
            QPageSize currentPageSize;
    };

    /**
     * Hash function used to manage a set of \ref Util::PageSize instances.
     *
     * \param[in] pageSize A reference to the \ref Util::PageSize instance to be hashed.
     *
     * \param[in] seed   An optional seed to apply to the hash.
     *
     * \return Returns a hash for this page size instance.
     */
    UTIL_PUBLIC_API HashResult qHash(const Util::PageSize& pageSize, HashSeed seed = 0);

    /**
     * Hash function used to manage a set of smart pointers to \ref Util::PageSize instances.
     *
     * \param[in] pageSize A smart pointer to the \ref Util::PageSize instance to be hashed.
     *
     * \param[in] seed     An optional seed to apply to the hash.
     *
     * \return Returns a hash for this smart pointer instance.
     */
    UTIL_PUBLIC_API HashResult qHash(QSharedPointer<Util::PageSize> pageSize, HashSeed seed = 0);

    /**
     * Hash function used to manage a set of weak pointers to \ref Util::PageSize instances.
     *
     * \param[in] pageSize A weak pointer to the \ref Util::PageSize instance to be hashed.
     *
     * \param[in] seed     An optional seed to apply to the hash.
     *
     * \return Returns a hash for this weak pointer instance.
     */
    UTIL_PUBLIC_API HashResult qHash(QWeakPointer<Util::PageSize> pageSize, HashSeed seed = 0);
}

#endif
