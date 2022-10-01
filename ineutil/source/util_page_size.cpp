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
* This file implements the \ref Util::PageSize class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QSet>
#include <QPageSize>
#include <QSizeF>
#include <QList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QPageSize>
#include <QPageLayout>

#include "util_units.h"
#include "util_hash_functions.h"
#include "util_page_size.h"

namespace Util {
    struct PageData {
        bool                  imperialUnits;
        PageSize::Region      region;
        const char*           name;
        QPageSize::PageSizeId pageSizeId;
    };

    static PageData const standards[] = {
    //    imperialUnits,
    //           region,                    name,                             pageSizeId
        { true,  PageSize::Region::Us,      QT_TR_NOOP("US Letter"),          QPageSize::Letter            },
        { true,  PageSize::Region::Us,      QT_TR_NOOP("US Legal"),           QPageSize::Legal             },
        { true,  PageSize::Region::Us,      QT_TR_NOOP("US Ledger"),          QPageSize::Ledger            },
        { true,  PageSize::Region::Us,      QT_TR_NOOP("US Executive"),       QPageSize::ExecutiveStandard },
        { true,  PageSize::Region::Us,      QT_TR_NOOP("US Folio"),           QPageSize::Folio             },
        { true,  PageSize::Region::Us,      QT_TR_NOOP("US Letter Small"),    QPageSize::LetterSmall       },
        { true,  PageSize::Region::Us,      QT_TR_NOOP("ANSI C"),             QPageSize::AnsiC             },
        { true,  PageSize::Region::Us,      QT_TR_NOOP("ANSI D"),             QPageSize::AnsiD             },
        { true,  PageSize::Region::Us,      QT_TR_NOOP("ANSI E"),             QPageSize::AnsiE             },
        { true,  PageSize::Region::Us,      QT_TR_NOOP("US Architectural A"), QPageSize::ArchA             },
        { true,  PageSize::Region::Us,      QT_TR_NOOP("US Architectural B"), QPageSize::ArchB             },
        { true,  PageSize::Region::Us,      QT_TR_NOOP("US Architectural C"), QPageSize::ArchC             },
        { true,  PageSize::Region::Us,      QT_TR_NOOP("US Architectural D"), QPageSize::ArchD             },
        { true,  PageSize::Region::Us,      QT_TR_NOOP("US Architectural E"), QPageSize::ArchE             },
        { true,  PageSize::Region::Britain, QT_TR_NOOP("British Imperial"),   QPageSize::Imperial7x9       },
        { true,  PageSize::Region::Britain, QT_TR_NOOP("British Quarto"),     QPageSize::Imperial8x10      },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO/JIS A0"),         QPageSize::A0                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO/JIS A1"),         QPageSize::A1                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO/JIS A2"),         QPageSize::A2                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO/JIS A3"),         QPageSize::A3                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO/JIS A4"),         QPageSize::A4                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO/JIS A5"),         QPageSize::A5                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO/JIS A6"),         QPageSize::A6                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO/JIS A7"),         QPageSize::A7                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO/JIS A8"),         QPageSize::A8                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO/JIS A9"),         QPageSize::A9                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO/JIS A10"),        QPageSize::A10               },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO B0"),             QPageSize::B0                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO B1"),             QPageSize::B1                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO B2"),             QPageSize::B2                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO B3"),             QPageSize::B3                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO B4"),             QPageSize::B4                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO B5"),             QPageSize::B5                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO B6"),             QPageSize::B6                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO B7"),             QPageSize::B7                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO B8"),             QPageSize::B8                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO B9"),             QPageSize::B9                },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO B10"),            QPageSize::B10               },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("ISO C5E"),            QPageSize::C5E               },
        { false, PageSize::Region::Europe,  QT_TR_NOOP("DLE"),                QPageSize::DLE               },
        { false, PageSize::Region::Japan,   QT_TR_NOOP("JIS B0"),             QPageSize::JisB0             },
        { false, PageSize::Region::Japan,   QT_TR_NOOP("JIS B1"),             QPageSize::JisB1             },
        { false, PageSize::Region::Japan,   QT_TR_NOOP("JIS B2"),             QPageSize::JisB2             },
        { false, PageSize::Region::Japan,   QT_TR_NOOP("JIS B3"),             QPageSize::JisB3             },
        { false, PageSize::Region::Japan,   QT_TR_NOOP("JIS B4"),             QPageSize::JisB4             },
        { false, PageSize::Region::Japan,   QT_TR_NOOP("JIS B5"),             QPageSize::JisB5             },
        { false, PageSize::Region::Japan,   QT_TR_NOOP("JIS B6"),             QPageSize::JisB6             },
        { false, PageSize::Region::Japan,   QT_TR_NOOP("JIS B7"),             QPageSize::JisB7             },
        { false, PageSize::Region::Japan,   QT_TR_NOOP("JIS B8"),             QPageSize::JisB8             },
        { false, PageSize::Region::Japan,   QT_TR_NOOP("JIS B9"),             QPageSize::JisB9             },
        { false, PageSize::Region::Japan,   QT_TR_NOOP("JIS B10"),            QPageSize::JisB10            },
        { false, PageSize::Region::China,   QT_TR_NOOP("PRC 16K"),            QPageSize::Prc16K            },
        { false, PageSize::Region::China,   QT_TR_NOOP("PRC 32K"),            QPageSize::Prc32K            },
        { false, PageSize::Region::China,   QT_TR_NOOP("PRC 32K Big"),        QPageSize::Prc32KBig         },
        { false, PageSize::Region::Us,      "",                               QPageSize::Custom           }
    };

    const QString  PageSize::defaultPageSizeName = tr("US Letter");
    const PageSize PageSize::defaultPageSize     = PageSize::calculateDefaultPageSize();

    PageSize::PageSize() {}


    PageSize::PageSize(double widthPoints, double heightPoints) {
        currentPageSize = QPageSize(QSizeF(widthPoints, heightPoints), QPageSize::Point);

    }


    PageSize::PageSize(const QString& name) {
        QPageSize::PageSizeId id = idFromName(name);

        if (id != QPageSize::Custom) {
            currentPageSize = QPageSize(id);
        } else {
            currentPageSize = QPageSize();
        }
    }


    PageSize::PageSize(const char* name) {
        QPageSize::PageSizeId id = idFromName(QString(name));

        if (id != QPageSize::Custom) {
            currentPageSize = QPageSize(id);
        } else {
            currentPageSize = QPageSize();
        }
    }


    PageSize::PageSize(const QPageSize& qPageSize) {
        currentPageSize = qPageSize;
    }


    PageSize::PageSize(const PageSize& other) {
        currentPageSize = other.currentPageSize;
    }


    PageSize::~PageSize() {}


    bool PageSize::isValid() const {
        bool valid = currentPageSize.isValid();
        return valid;
    }


    bool PageSize::isInvalid() const {
        return !isValid();
    }


    void PageSize::setPageSize(double newWidthPoints, double newHeightPoints) {
        currentPageSize = QPageSize(QSizeF(newWidthPoints, newHeightPoints), QPageSize::Point);
    }


    void PageSize::setPageFromName(const QString& name) {
        QPageSize::PageSizeId id = idFromName(name);

        if (id != QPageSize::Custom) {
            currentPageSize = QPageSize(id);
        } else {
            currentPageSize = QPageSize();
        }
    }


    void PageSize::setPageFromName(const char* name) {
        setPageFromName(QString(name));
    }


    double PageSize::width() const {
        return currentPageSize.size(QPageSize::Point).width();
    }


    double PageSize::height() const {
        return currentPageSize.size(QPageSize::Point).height();
    }


    QString PageSize::name() const {
        QString               name;
        QPageSize::PageSizeId id = currentPageSize.id();

        if (id != QPageSize::Custom) {
            const PageData* standard = standards;
            while (standard->pageSizeId != id && standard->pageSizeId != QPageSize::Custom) {
                ++standard;
            }

            if (standard->pageSizeId != QPageSize::Custom) {
                name = tr(standard->name);
            } else {
                name = QString();
            }
        } else {
            name = tr("Custom");
        }

        return name;
    }


    QString PageSize::description(bool useMetric) const {
        QPageSize::PageSizeId id = currentPageSize.id();

        QString name;
        double  widthPoints = currentPageSize.size(QPageSize::Point).width();
        double  heightPoints = currentPageSize.size(QPageSize::Point).height();

        if (id != QPageSize::Custom) {
            const PageData* standard = standards;
            while (standard->pageSizeId != id && standard->pageSizeId != QPageSize::Custom) {
                ++standard;
            }

            if (standard->pageSizeId != QPageSize::Custom) {
                name = tr(standard->name);
            } else {
                name = tr("???");
            }
        } else {
            name = tr("Custom");
        }

        QString description;
        if (useMetric) {
            description = tr("%1 (%2 mm x %3 mm)")
                          .arg(name)
                          .arg(widthPoints * Util::Units::mmPerPoint, 0, 'g', 0) // FIXME
                          .arg(heightPoints * Util::Units::mmPerPoint, 0, 'g', 0); // FIXME
        } else {
            description = tr("%1 (%2\" x %3\")")
                          .arg(name)
                          .arg(widthPoints * Util::Units::inchesPerPoint, 0, 'g', 4)
                          .arg(heightPoints * Util::Units::inchesPerPoint, 0, 'g', 4);
        }

        return description;
    }


    QPageSize PageSize::toQPageSize() const {
        return currentPageSize;
    }


    PageSize& PageSize::operator=(const PageSize& other) {
        currentPageSize = other.currentPageSize;
        return *this;
    }


    bool PageSize::operator==(const PageSize& other) const {
        return other.currentPageSize == currentPageSize;
    }


    bool PageSize::operator!=(const PageSize& other) const {
        return other.currentPageSize != currentPageSize;
    }


    QString PageSize::toString() const {
        QString pageString;

        if (isValid()) {
            QPageSize::PageSizeId id = currentPageSize.id();

            if (id == QPageSize::Custom) {
                QSizeF size = currentPageSize.size(QPageSize::Point);
                pageString = QString("Custom %1 %2").arg(size.width()).arg(size.height());
            } else {
                pageString = name();
            }
        } else {
            pageString = QString("Invalid");
        }

        return pageString;
    }


    PageSize PageSize::fromString(const QString& identifier, bool* ok) {
        bool     isOk = true;
        PageSize pageSize;

        QRegularExpression re("^Custom  *([0-9.][0-9.]*)  *([0-9.][0-9.]*)$");
        QRegularExpressionMatch match = re.match(identifier);

        if (match.hasMatch()) {
            double height;
            double width = match.captured(1).toDouble(&isOk);

            if (isOk) {
                height = match.captured(2).toDouble(&isOk);
            }

            if (isOk) {
                pageSize = PageSize(width, height);
            }
        } else if (identifier == QString("Invalid")) { // To be explicit
            pageSize = PageSize();
        } else {
            QPageSize::PageSizeId id = PageSize::idFromName(identifier);
            if (id != QPageSize::Custom) {
                pageSize.currentPageSize = QPageSize(id);
            } else {
                isOk = false;
            }
        }

        if (ok != nullptr) {
            *ok = isOk;
        }

        return pageSize;
    }


    QList<QString> PageSize::names(PageSize::Region region) {
        QList<QString> names;
        const PageData* standard = standards;
        while (standard->pageSizeId != QPageSize::Custom) {
            if (region == PageSize::Region::AllRegions || region == standard->region) {
                names << tr(standard->name);
            }

            ++standard;
        }

        return names;
    }


    QList<QString> PageSize::descriptions(PageSize::Region region, bool useMetric) {
        QList<QString> descriptions;
        const PageData* standard = standards;
        while (standard->pageSizeId != QPageSize::Custom) {
            if (region == PageSize::Region::AllRegions || region == standard->region) {
                PageSize pageSize(tr(standard->name));
                descriptions << pageSize.description(useMetric);
            }

            ++standard;
        }

        return descriptions;
    }


    PageSize PageSize::fromName(const QString& name) {
        return PageSize(name);
    }


    PageSize PageSize::fromDescription(const QString& description) {
        PageSize                result;
        QRegularExpression      regex(tr("([a-zA-Z0-9/ ]+)\\(([0-9.]+) *([a-z\"]+) *x *([0-9.]+) *([a-z\"]+)\\).*"));
        QRegularExpressionMatch match = regex.match(description);

        if (match.hasMatch()) {
            QString name = match.captured(1).trimmed();

            if (name != tr("Custom")) {
                result = PageSize::fromName(name);
            } else {
                QString widthString  = match.captured(2);
                QString widthUnits   = match.captured(3);
                QString heightString = match.captured(4);
                QString heightUnits  = match.captured(5);

                double widthPoints;
                double heightPoints;

                if ((widthUnits == tr("mm") || widthUnits == tr("\""))   &&
                    (heightUnits == tr("mm") || heightUnits == tr("\""))    ) {
                    if (widthUnits == tr("mm")) {
                        widthPoints = widthString.toDouble() * Util::Units::pointsPerMm;
                    } else {
                        widthPoints = widthString.toDouble() * Util::Units::pointsPerInch;
                    }

                    if (heightUnits == tr("mm")) {
                        heightPoints = heightString.toDouble() * Util::Units::pointsPerMm;
                    } else {
                        heightPoints = heightString.toDouble() * Util::Units::pointsPerInch;
                    }

                    result = PageSize(widthPoints, heightPoints);
                }
            }
        }

        return result;
    }


    PageSize PageSize::calculateDefaultPageSize() {
        QPageSize::PageSizeId id = idFromName(PageSize::defaultPageSizeName);
        PageSize              defaultPageSize;

        defaultPageSize.currentPageSize = QPageSize(id);

        return defaultPageSize;
    }


    QPageSize::PageSizeId PageSize::idFromName(const QString& pageName) {
        const PageData* standard = standards;
        while (standard->pageSizeId != QPageSize::Custom && tr(standard->name) != pageName) {
            ++standard;
        }

        return standard->pageSizeId;
    }


    HashResult qHash(const Util::PageSize& pageSize, HashSeed seed) {
        return ::qHash(pageSize.toString(), seed);
    }


    HashResult qHash(QSharedPointer<Util::PageSize> pageSize, HashSeed seed) {
        return ::qHash(pageSize.data(), seed);
    }


    HashResult qHash(QWeakPointer<Util::PageSize> pageSize, HashSeed seed) {
        return ::qHash(pageSize.toStrongRef().data(), seed);
    }
}
