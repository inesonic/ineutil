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
* This file implements tests of the fuzzy search functions class
***********************************************************************************************************************/

#include <QtGlobal>
#include <QObject>
#include <QtTest/QtTest>
#include <QString>
#include <QStringList>
#include <QList>
#include <QSet>
#include <QFont>
#include <QFontDatabase>
#include <QFontMetricsF>

#include <QDebug> // Debug

#include <limits>
#include <random>

#include <util_string.h>
#include <util_fuzzy_search.h>

#include "test_fuzzy_search.h"

TestFuzzySearch::TestFuzzySearch() {}


TestFuzzySearch::~TestFuzzySearch() {}


void TestFuzzySearch::initTestCase() {}


void TestFuzzySearch::testTokenizedString() {
    //                                        0  1 2    3     4   5     6    7  8    9   10     8     11   12 13
    Util::TokenizedString s1(Util::String("This is a test -- Do not trust that it will work -- It musn't fail."));

    QCOMPARE(s1.length(), 15);
    QCOMPARE(
        s1.tokenList(),
        QList<Util::TokenizedString::Token>()
            << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 8 << 11 << 12 << 13
    );

    Util::TokenizedValue v1 = s1;

    QCOMPARE(v1.length(), 15);
    QCOMPARE(
        v1.tokenList(),
        QList<Util::TokenizedString::Token>()
            << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 8 << 11 << 12 << 13
    );

    Util::TokenizedString s2 = v1;

    QCOMPARE(s2.length(), 15);
    QCOMPARE(
        s2.tokenList(),
        QList<Util::TokenizedString::Token>()
            << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 8 << 11 << 12 << 13
    );

    Util::String approximate = s2.approximateString();
    QCOMPARE(approximate, Util::String("this is a test do not trust that it will work it musn't fail ."));

    //                                     2    3   14    0    9   5   12 13  8    9  4  15    16    17 13
    Util::TokenizedString s3(Util::String("A test like this will not fail.   It will do the right thing."));

    QCOMPARE(s3.length(), 15);
    QCOMPARE(
        s3.tokenList(),
        QList<Util::TokenizedString::Token>()
            << 2 << 3 << 14 << 0 << 9 << 5 << 12 << 13 << 8 << 9 << 4 << 15 << 16 << 17 << 13
    );

    //                                     2    3   14    0    9   5   12 13  8    9    18  4  15    16    17 13
    Util::TokenizedString s4(Util::String("A test like this will not fail.   It will maybe do the right thing."));

    QCOMPARE(s4.length(), 16);
    QCOMPARE(
        s4.tokenList(),
        QList<Util::TokenizedString::Token>()
            << 2 << 3 << 14 << 0 << 9 << 5 << 12 << 13 << 8 << 9 << 18 << 4 << 15 << 16 << 17 << 13
    );

    QCOMPARE(s1 == s2, true);
    QCOMPARE(s1 != s2, false);
    QCOMPARE(s1 < s2,  false);
    QCOMPARE(s1 > s2,  false);
    QCOMPARE(s1 <= s2, true);
    QCOMPARE(s1 >= s2, true);

    QCOMPARE(s1 == s3, false);
    QCOMPARE(s1 != s3, true);
    QCOMPARE(s1 < s3,  true);
    QCOMPARE(s1 > s3,  false);
    QCOMPARE(s1 <= s3, true);
    QCOMPARE(s1 >= s3, false);

    QCOMPARE(s1 == s4, false);
    QCOMPARE(s1 != s4, true);
    QCOMPARE(s1 < s4,  true);
    QCOMPARE(s1 > s4,  false);
    QCOMPARE(s1 <= s4, true);
    QCOMPARE(s1 >= s4, false);

    QCOMPARE(s3 == s4, false);
    QCOMPARE(s3 != s4, true);
    QCOMPARE(s3 < s4,  true);
    QCOMPARE(s3 > s4,  false);
    QCOMPARE(s3 <= s4, true);
    QCOMPARE(s3 >= s4, false);
}


void TestFuzzySearch::testFuzzySearch() {
    struct PatternStructure {
        Util::FuzzySearchEngine::GroupId groupId;
        const char*                      str;
    };

    PatternStructure testStrings[] = {
        { 1, "Once upon a midnight dreary, while I pondered, weak and weary,"       }, //  0
        { 1, "Over many a quaint and curious volume of forgotten lore—"             }, //  1
        { 1,     "While I nodded, nearly napping, suddenly there came a tapping,"   }, //  2
        { 1, "As of some one gently rapping, rapping at my chamber door."           }, //  3
        { 1, "\"'Tis some visitor,\" I muttered, \"tapping at my chamber door—"     }, //  4
        { 1,            "Only this and nothing more.\""                             }, //  5

        { 2,     "Ah, distinctly I remember it was in the bleak December;"          }, //  6
        { 2, "And each separate dying ember wrought its ghost upon the floor."      }, //  7
        { 2,     "Eagerly I wished the morrow;—vainly I had sought to borrow"       }, //  8
        { 2,     "From my books surcease of sorrow—sorrow for the lost Lenore—"     }, //  9
        { 2, "For the rare and radiant maiden whom the angels name Lenore—"         }, // 10
        { 2,             "Nameless here for evermore."                              }, // 11

        { 3,    "And the silken, sad, uncertain rustling of each purple curtain"    }, // 12
        { 3, "Thrilled me—filled me with fantastic terrors never felt before;"      }, // 13
        { 3,     "So that now, to still the beating of my heart, I stood repeating" }, // 14
        { 3,     "\"'Tis some visitor entreating entrance at my chamber door—"      }, // 15
        { 3, "Some late visitor entreating entrance at my chamber door;—"           }, // 16
        { 3,             "This it is and nothing more.\""                           }, // 17
        { 0, nullptr }
    };

    QList<Util::String> stopWords;
    Util::FuzzySearchEngine engine(stopWords);

    Util::FuzzySearchEngine::PatternId patternId = 0;
    PatternStructure*                 s          = testStrings;
    while (s->str != nullptr) {
        engine.registerPattern(Util::String(s->str), s->groupId, patternId);
        ++patternId;
        ++s;
    }

    QList<Util::FuzzySearchEngine::PatternId> patterns = engine.search();
    QCOMPARE(patterns.size(), 18);
    QCOMPARE(
        patterns,
        QList<Util::FuzzySearchEngine::PatternId>()
            <<  0 <<  1 <<  2 <<  3 <<  4 <<  5 <<  6 <<  7 <<  8
            <<  9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17
    );

    patterns = engine.search(Util::TokenizedString(), QList<Util::FuzzySearchEngine::GroupId>() << 2 << 3);
    QCOMPARE(patterns.size(), 12);
    QCOMPARE(
        patterns,
        QList<Util::FuzzySearchEngine::PatternId>()
            <<  6 <<  7 <<  8 <<  9 << 10 << 11
            << 12 << 13 << 14 << 15 << 16 << 17
    );

    patterns = engine.search(Util::String("while i"), QList<Util::FuzzySearchEngine::GroupId>() << 1 << 2);
    QCOMPARE(patterns.size(), 5);
    QCOMPARE(patterns, QList<Util::FuzzySearchEngine::PatternId>() << 0 << 2 << 8 << 4 << 6);

    patterns = engine.search(Util::String("while i entreating"));
    QCOMPARE(patterns.size(), 8);
    QCOMPARE(patterns, QList<Util::FuzzySearchEngine::PatternId>() << 0 << 2 << 8 << 4 << 6 << 14 << 15 << 16);
}
