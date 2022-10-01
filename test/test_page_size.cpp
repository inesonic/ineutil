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
* This file implements tests of the Util::PageSize class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QDebug>
#include <QString>
#include <QPageSize>
#include <QList>
#include <QtTest/QtTest>

#include <util_page_size.h>

#include "test_page_size.h"

TestPageSize::TestPageSize() {}


TestPageSize::~TestPageSize() {}


void TestPageSize::initTestCase() {}


void TestPageSize::testConstructorsAndDestructors() {
    Util::PageSize pageSize1;
    QVERIFY(pageSize1.isInvalid());

    Util::PageSize pageSize2(8.5*72.0, 11.0*72.0);
    QVERIFY(pageSize2.isValid());
    QVERIFY(pageSize2.width() == 8.5*72.0);
    QVERIFY(pageSize2.height() == 11.0*72.0);
    QVERIFY(pageSize2.name() == tr("US Letter"));

    Util::PageSize pageSize3(QString("US Letter"));
    QVERIFY(pageSize3.isValid());
    QVERIFY(pageSize3.width() == 8.5*72.0);
    QVERIFY(pageSize3.height() == 11.0*72.0);
    QVERIFY(pageSize3.name() == QString("US Letter"));

    Util::PageSize pageSize4("US Letter");
    QVERIFY(pageSize4.isValid());
    QVERIFY(pageSize4.width() == 8.5*72.0);
    QVERIFY(pageSize4.height() == 11.0*72.0);
    QVERIFY(pageSize4.name() == QString("US Letter"));

    Util::PageSize pageSize5(pageSize4);
    QVERIFY(pageSize5.isValid());
    QVERIFY(pageSize5.width() == 8.5*72.0);
    QVERIFY(pageSize5.height() == 11.0*72.0);
    QVERIFY(pageSize5.name() == QString("US Letter"));

    QPageSize qPageSize(QPageSize::Letter);
    Util::PageSize pageSize6(qPageSize);
    QVERIFY(pageSize6.isValid());
    QVERIFY(pageSize6.width() == 8.5*72.0);
    QVERIFY(pageSize6.height() == 11.0*72.0);
    QVERIFY(pageSize6.name() == QString("US Letter"));
}


void TestPageSize::testValidInvalidPageSizes() {
    Util::PageSize pageSize1;
    QVERIFY(pageSize1.isInvalid()== true);
    QVERIFY(pageSize1.isValid() == false);

    Util::PageSize pageSize2("US Letter");
    QVERIFY(pageSize2.isInvalid() == false);
    QVERIFY(pageSize2.isValid() == true);
}


void TestPageSize::testAccessors() {
    Util::PageSize pageSize;

    pageSize.setPageSize(8.5*72.0, 11.0*72.0);
    QVERIFY(pageSize.isValid());
    QVERIFY(pageSize.width() == 8.5*72.0);
    QVERIFY(pageSize.height() == 11.0*72.0);
    QVERIFY(pageSize.name() == tr("US Letter"));

    pageSize.setPageFromName(QString("US Legal"));
    QVERIFY(pageSize.isValid());
    QVERIFY(pageSize.width() == 8.5*72.0);
    QVERIFY(pageSize.height() == 14.0*72.0);
    QVERIFY(pageSize.name() == tr("US Legal"));

    pageSize.setPageFromName("US Ledger");
    QVERIFY(pageSize.isValid());
    QVERIFY(pageSize.width() == 17.0*72.0);
    QVERIFY(pageSize.height() == 11.0*72.0);
    QVERIFY(pageSize.name() == tr("US Ledger"));
}


void TestPageSize::testStringConversions() {
    Util::PageSize inputPageSize;
    QString            pageAsString = inputPageSize.toString();

    bool               ok              = false;
    Util::PageSize     rebuiltPageSize = Util::PageSize::fromString(pageAsString, &ok);

    QVERIFY(ok);
    QVERIFY(rebuiltPageSize.isInvalid());

    inputPageSize.setPageFromName("US Ledger");
    pageAsString    = inputPageSize.toString();
    rebuiltPageSize = Util::PageSize::fromString(pageAsString, &ok);

    QVERIFY(ok);
    QVERIFY(rebuiltPageSize.isValid());
    QVERIFY(rebuiltPageSize.name() == QString("US Ledger"));
    QVERIFY(rebuiltPageSize.width() == 17.0*72.0);
    QVERIFY(rebuiltPageSize.height() == 11.0*72.0);

    inputPageSize.setPageSize(72.0, 72.0); // custom size.
    pageAsString    = inputPageSize.toString();
    rebuiltPageSize = Util::PageSize::fromString(pageAsString, &ok);

    QVERIFY(ok);
    QVERIFY(rebuiltPageSize.isValid());
    QVERIFY(rebuiltPageSize.name() == QString("Custom"));
    QVERIFY(rebuiltPageSize.width() == 72.0);
    QVERIFY(rebuiltPageSize.height() == 72.0);
}


void TestPageSize::testAssignmentOperator() {
    Util::PageSize pageSize1("US Legal");
    Util::PageSize pageSize2;

    QVERIFY(pageSize2.isInvalid());

    pageSize2 = pageSize1;
    QVERIFY(pageSize2.isValid());
    QVERIFY(pageSize2.width() == 8.5*72.0);
    QVERIFY(pageSize2.height() == 14.0*72.0);
    QVERIFY(pageSize2.name() == tr("US Legal"));
}


void TestPageSize::testComparisonOperator() {
    Util::PageSize pageSize1;
    Util::PageSize pageSize2("US Letter");
    Util::PageSize pageSize3("US Legal");
    Util::PageSize pageSize4;
    Util::PageSize pageSize5("US Letter");

    QVERIFY((pageSize1 == pageSize2) == false);
    QVERIFY((pageSize1 == pageSize3) == false);
    QVERIFY((pageSize1 == pageSize4) == true);
    QVERIFY((pageSize1 != pageSize2) == true);
    QVERIFY((pageSize1 != pageSize3) == true);
    QVERIFY((pageSize1 != pageSize4) == false);

    QVERIFY((pageSize2 == pageSize3) == false);
    QVERIFY((pageSize2 == pageSize4) == false);
    QVERIFY((pageSize2 == pageSize5) == true);
    QVERIFY((pageSize2 != pageSize3) == true);
    QVERIFY((pageSize2 != pageSize4) == true);
    QVERIFY((pageSize2 != pageSize5) == false);
}


void TestPageSize::testQPageSizeMethods() {
    QPageSize qPageSize1;
    Util::PageSize pageSize1(qPageSize1);

    QVERIFY(pageSize1.isInvalid());

    QPageSize qPageSize2(QPageSize::Ledger);
    Util::PageSize pageSize2(qPageSize2);

    QVERIFY(pageSize2.isValid());
    QVERIFY(pageSize2.name() == QString("US Ledger"));

    qPageSize1 = pageSize2.toQPageSize();
    QVERIFY(qPageSize1.id() == QPageSize::Ledger);
}


void TestPageSize::testNamesByRegion() {
    QList<QString> namesByRegion = Util::PageSize::names(Util::PageSize::Region::Us);

    // For now, check count and spot check for the big ones for each region.

    QVERIFY(namesByRegion.length() == 14);
    QVERIFY(namesByRegion.contains(QString("US Letter")));
    QVERIFY(namesByRegion.contains(QString("US Legal")));
    QVERIFY(namesByRegion.contains(QString("US Ledger")));
    QVERIFY(namesByRegion.contains(QString("US Executive")));

    namesByRegion = Util::PageSize::names(Util::PageSize::Region::Britain);

    QVERIFY(namesByRegion.length() == 2);
    QVERIFY(namesByRegion.contains(QString("British Imperial")));
    QVERIFY(namesByRegion.contains(QString("British Quarto")));

    namesByRegion = Util::PageSize::names(Util::PageSize::Region::Europe);

    QVERIFY(namesByRegion.length() == 24);
    QVERIFY(namesByRegion.contains(QString("ISO/JIS A0")));
    QVERIFY(namesByRegion.contains(QString("ISO/JIS A4")));

    namesByRegion = Util::PageSize::names(Util::PageSize::Region::Japan);

    QVERIFY(namesByRegion.length() == 11);
    QVERIFY(namesByRegion.contains(QString("JIS B0")));
    QVERIFY(namesByRegion.contains(QString("JIS B1")));

    namesByRegion = Util::PageSize::names(Util::PageSize::Region::China);

    QVERIFY(namesByRegion.length() == 3);
    QVERIFY(namesByRegion.contains(QString("PRC 16K")));
    QVERIFY(namesByRegion.contains(QString("PRC 32K")));

    namesByRegion = Util::PageSize::names(Util::PageSize::Region::AllRegions);
    QVERIFY(namesByRegion.length() == 54);
}
