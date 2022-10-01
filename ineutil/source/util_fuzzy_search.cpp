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
* This file implements a simple fuzzy search engine.
***********************************************************************************************************************/

#include <QCoreApplication>
#include <QList>
#include <QHash>
#include <QSet>
#include <QMultiMap>
#include <QRegularExpression>

#include <cstring>
#include <cstdint>

#include "util_string.h"
#include "util_hash_functions.h"
#include "util_fuzzy_search.h"

/**********************************************************************************************************************
 * Util::TokenizedValue
 */

namespace Util {
    const TokenizedValue::Token TokenizedValue::invalidToken = static_cast<TokenizedValue::Token>(-1);

    TokenizedValue::TokenizedValue() {
        currentLength = 0;
    }


    TokenizedValue::TokenizedValue(Length length, const Token* tokens) {
        currentLength = length;
        std::memcpy(currentTokens, tokens, sizeof(Token) * length);
    }


    TokenizedValue::TokenizedValue(const TokenizedValue& other) {
        currentLength = other.currentLength;
        std::memcpy(currentTokens, other.currentTokens, sizeof(Token) * currentLength);
    }


    TokenizedValue::~TokenizedValue() {}


    TokenizedValue::Length TokenizedValue::length() const {
        return currentLength;
    }


    const TokenizedValue::Token* TokenizedValue::tokens() const {
        return currentTokens;
    }


    QList<TokenizedValue::Token> TokenizedValue::tokenList() const {
        QList<Token> result;

        for (Length i=0 ; i<currentLength ; ++i) {
            result << currentTokens[i];
        }

        return result;
    }


    bool TokenizedValue::addToken(Token newToken) {
        bool success;

        if (currentLength < maximumNumberTokens) {
            currentTokens[currentLength] = newToken;
            ++currentLength;

            success = true;
        } else {
            success = false;
        }

        return success;
    }


    TokenizedValue& TokenizedValue::operator=(const TokenizedValue& other) {
        currentLength = other.currentLength;
        std::memcpy(currentTokens, other.currentTokens, sizeof(Token) * currentLength);

        return *this;
    }


    bool TokenizedValue::operator==(const TokenizedValue& other) const {
        return (
               currentLength == other.currentLength
            && std::memcmp(currentTokens, other.currentTokens, sizeof(Token) * currentLength) == 0
        );
    }


    bool TokenizedValue::operator!=(const TokenizedValue& other) const {
        return !operator==(other);
    }


    bool TokenizedValue::operator<(const TokenizedValue& other) const {
        return (
               currentLength < other.currentLength
            || (   currentLength == other.currentLength
                && std::memcmp(currentTokens, other.currentTokens, sizeof(Token) * currentLength) < 0
               )
        );
    }


    bool TokenizedValue::operator>(const TokenizedValue& other) const {
        return (
               currentLength > other.currentLength
            || (   currentLength == other.currentLength
                && std::memcmp(currentTokens, other.currentTokens, sizeof(Token) * currentLength) > 0
               )
        );
    }


    bool TokenizedValue::operator<=(const TokenizedValue& other) const {
        return !operator>(other);
    }


    bool TokenizedValue::operator>=(const TokenizedValue& other) const {
        return !operator<(other);
    }
}

/**********************************************************************************************************************
 * Util::TokenizedValue
 */

namespace Util {
    QHash<String, TokenizedValue::Token> TokenizedString::tokensByKeyword;
    QList<String>                        TokenizedString::keywordsByToken;

    TokenizedString::TokenizedString() {}


    TokenizedString::TokenizedString(const String& str, bool assignNewTokens) {
        unsigned stringLength = static_cast<unsigned>(str.length());
        String   keyword;
        for (unsigned i=0 ; i<stringLength ; ++i) {
            Char c = str.at(i);
            if (c == Char('.')) {
                if (!keyword.isEmpty()) {
                    addKeyword(keyword, assignNewTokens);
                    keyword.clear();
                }

                addKeyword(QString('.'), assignNewTokens);
            } else if ((c.isPunct() && c != Char('\'')) || c.isSpace()) {
                if (!keyword.isEmpty()) {
                    addKeyword(keyword, assignNewTokens);
                    keyword.clear();
                }
            } else {
                keyword += c;
            }
        }

        if (!keyword.isEmpty()) {
            addKeyword(keyword, assignNewTokens);
        }
    }


    TokenizedString::TokenizedString(const TokenizedValue& tv):TokenizedValue(tv) {}


    TokenizedString::TokenizedString(const TokenizedString& other):TokenizedValue(other) {}


    TokenizedString::~TokenizedString() {}


    String TokenizedString::approximateString() const {
        Length stringLength = length();

        String result;
        const Token* currentTokens = tokens();
        for (Length i=0 ; i<stringLength ; ++i) {
            if (i != 0) {
                result += QChar(' ');
            }

            result += keywordsByToken[currentTokens[i]];
        }

        return result;
    }


    TokenizedString::Token TokenizedString::tokenForKeyword(const String& keyword, bool assignNewToken) {
        String lowerCase = keyword.toLower();
        Token  token     = tokensByKeyword.value(lowerCase, invalidToken);
        if (token == invalidToken && assignNewToken) {
            Token newToken = static_cast<Token>(keywordsByToken.size());

            if (newToken != invalidToken) {
                keywordsByToken.append(lowerCase);
                tokensByKeyword.insert(lowerCase, newToken);

                token = newToken;
            }
        }

        return token;
    }


    bool TokenizedString::addKeyword(const String& keyword, bool assignNewTokens) {
        bool success;

        String lowerCase = keyword.toLower();
        if (tokensByKeyword.contains(lowerCase)) {
            Token token = tokensByKeyword.value(lowerCase);
            success = addToken(token);
        } else if (assignNewTokens) {
            Token newToken = static_cast<Token>(keywordsByToken.size());

            if (newToken != invalidToken) {
                success = addToken(newToken);
                if (success) {
                    keywordsByToken.append(lowerCase);
                    tokensByKeyword.insert(lowerCase, newToken);
                }
            } else {
                success = false;
            }
        } else {
            success = addToken(invalidToken);
        }

        return success;
    }
}

/**********************************************************************************************************************
 * Util::TokenizedValue
 */

namespace Util {
    const FuzzySearchEngine::PatternId
        FuzzySearchEngine::invalidPatternId = static_cast<FuzzySearchEngine::PatternId>(-1);

    const FuzzySearchEngine::GroupId
        FuzzySearchEngine::invalidGroupId   = static_cast<FuzzySearchEngine::GroupId>(-1);

    // The list of stope words below was shamelessly lifted from:
    //     https://meta.wikimedia.org/wiki/Stop_word_list/google_stop_word_list#English

    const char* FuzzySearchEngine::englishStopWords[] = {
        "a",            "about",        "above",        "after",        "again",        "against",      "all",
        "am",           "an",           "and",          "any",          "are",          "aren't",       "as",
        "at",           "be",           "because",      "been",         "before",       "being",        "below",
        "between",      "both",         "but",          "by",           "can't",        "cannot",       "could",
        "couldn't",     "did",          "didn't",       "do",           "does",         "doesn't",      "doing",
        "don't",        "down",         "during",       "each",         "few",          "for",          "from",
        "further",      "had",          "hadn't",       "has",          "hasn't",       "have",         "haven't",
        "having",       "he",           "he'd",         "he'll",        "he's",         "her",          "here",
        "here's",       "hers",         "herself",      "him",          "himself",      "his",          "how",
        "how's",        "i",            "i'd",          "i'll",         "i'm",          "i've",         "if",
        "in",           "into",         "is",           "isn't",        "it",           "it's",         "its",
        "itself",       "let's",        "me",           "more",         "most",         "mustn't",      "my",
        "myself",       "no",           "nor",          "not",          "of",           "off",          "on",
        "once",         "only",         "or",           "other",        "ought",        "our",          "ours",
        "ourselves",    "out",          "over",         "own",          "same",         "shan't",       "she",
        "she'd",        "she'll",       "she's",        "should",       "shouldn't",    "so",           "some",
        "such",         "than",         "that",         "that's",       "the",          "their",        "theirs",
        "them",         "themselves",   "then",         "there",        "there's",      "these",        "they",
        "they'd",       "they'll",      "they're",      "they've",      "this",         "those",        "through",
        "to",           "too",          "under",        "until",        "up",           "very",         "was",
        "wasn't",       "we",           "we'd",         "we'll",        "we're",        "we've",        "were",
        "weren't",      "what",         "what's",       "when",         "when's",       "where",        "where's",
        "which",        "while",        "who",          "who's",        "whom",         "why",          "why's",
        "with",         "won't",        "would",        "wouldn't",     "you",          "you'd",        "you'll",
        "you're",       "you've",       "your",         "yours",        "yourself",     "yourselves",   nullptr
    };

    FuzzySearchEngine::FuzzySearchEngine() {
        configure(String("en"));
    }


    FuzzySearchEngine::FuzzySearchEngine(const String& locale) {
        configure(locale);
    }


    FuzzySearchEngine::FuzzySearchEngine(const QList<String>& stopWords) {
        for (QList<String>::const_iterator it=stopWords.constBegin(), end=stopWords.constEnd() ; it!=end ; ++it) {
            TokenizedValue::Token token = TokenizedString::tokenForKeyword(*it);
            currentStopWords.insert(token);
        }
    }


    FuzzySearchEngine::~FuzzySearchEngine() {}


    void FuzzySearchEngine::clear() {
        patternIdsByValueByGroupId.clear();
        patternIdsByGroupId.clear();
    }


    void FuzzySearchEngine::registerPattern(
            const TokenizedString&       pattern,
            FuzzySearchEngine::GroupId   groupId,
            FuzzySearchEngine::PatternId patternId
        ) {
        TokenizedString               cleanedString = removeStopWordsFrom(pattern);
        unsigned                      numberTokens  = cleanedString.length();
        const TokenizedString::Token* tokens        = cleanedString.tokens();

        QHash<TokenizedValue, QList<PatternId>>& patternIdsByValue = patternIdsByValueByGroupId[groupId];
        for (unsigned left=0 ; left<numberTokens ; ++left) {
            TokenizedString s;
            for (unsigned right=left ; right<numberTokens ; ++right) {
                s.addToken(tokens[right]);
                patternIdsByValue[s] << patternId;
            }
        }

        patternIdsByGroupId[groupId].append(patternId);
    }


    QList<FuzzySearchEngine::PatternId> FuzzySearchEngine::search(
            const TokenizedString&                  searchPattern,
            const QList<FuzzySearchEngine::GroupId> groupIds
        ) const {
        TokenizedString cleanedString = removeStopWordsFrom(searchPattern);
        QList<PatternId> result;

        if (searchPattern.length() == 0) {
            if (groupIds.isEmpty()) {
                for (  QMap<GroupId, QList<PatternId>>::const_iterator
                           groupIterator    = patternIdsByGroupId.constBegin(),
                           groupEndIterator = patternIdsByGroupId.constEnd()
                     ; groupIterator != groupEndIterator
                     ; ++groupIterator
                    ) {
                    const QList<PatternId>& patternIdsThisGroup = *groupIterator;
                    result.append(patternIdsThisGroup);
                }
            } else {
                for (  QList<FuzzySearchEngine::GroupId>::const_iterator
                           groupIterator = groupIds.constBegin(),
                           groupEndIterator = groupIds.constEnd()
                     ; groupIterator != groupEndIterator
                     ; ++groupIterator
                    ) {
                    GroupId groupId = *groupIterator;
                    result.append(patternIdsByGroupId.value(groupId));
                }
            }
        } else {
            QHash<PatternId, unsigned> hitCountsByPatternId;

            if (groupIds.isEmpty()) {
                for (  QMap<GroupId, QList<PatternId>>::const_iterator
                           groupIterator    = patternIdsByGroupId.constBegin(),
                           groupEndIterator = patternIdsByGroupId.constEnd()
                     ; groupIterator != groupEndIterator
                     ; ++groupIterator
                    ) {
                    searchGroup(cleanedString, groupIterator.key(), hitCountsByPatternId);
                }
            } else {
                for (  QList<FuzzySearchEngine::GroupId>::const_iterator
                           groupIterator = groupIds.constBegin(),
                           groupEndIterator = groupIds.constEnd()
                     ; groupIterator != groupEndIterator
                     ; ++groupIterator
                    ) {
                    searchGroup(cleanedString, *groupIterator, hitCountsByPatternId);
                }
            }

            QMap<int, QMap<PatternId, int>> patternIdsByHitCount;
            for (  QHash<PatternId, unsigned>::const_iterator hitCountsIterator    = hitCountsByPatternId.constBegin(),
                                                              hitCountsEndIterator = hitCountsByPatternId.constEnd()
                 ; hitCountsIterator != hitCountsEndIterator
                 ; ++hitCountsIterator
                ) {
                PatternId patternId = hitCountsIterator.key();
                unsigned  hitCount  = hitCountsIterator.value();

                patternIdsByHitCount[-static_cast<int>(hitCount)].insert(patternId, hitCount);
            }

            for (  QMap<int, QMap<PatternId, int>>::const_iterator
                       resultIterator    = patternIdsByHitCount.constBegin(),
                       resultEndIterator = patternIdsByHitCount.constEnd()
                 ; resultIterator != resultEndIterator
                 ; ++resultIterator
                ) {
                result.append(resultIterator.value().keys());
            }
        }

        return result;
    }


    void FuzzySearchEngine::configure(const String& locale) {
        const char** stopWords;
        if (locale == String("en")) {
            stopWords = englishStopWords;
        } else {
            stopWords = englishStopWords;
        }

        while (*stopWords != nullptr) {
            TokenizedValue::Token token = TokenizedString::tokenForKeyword(String(*stopWords));
            currentStopWords.insert(token);

            ++stopWords;
        }
    }


    TokenizedString FuzzySearchEngine::removeStopWordsFrom(const TokenizedString& rawPattern) const {
        TokenizedString result;

        const TokenizedString::Token* tokens       = rawPattern.tokens();
        unsigned                      numberTokens = rawPattern.length();

        for (unsigned i=0 ; i<numberTokens ; ++i) {
            TokenizedString::Token token = tokens[i];
            if (!currentStopWords.contains(token)) {
                result.addToken(token);
            }
        }

        return result;
    }


    void FuzzySearchEngine::searchGroup(
            const TokenizedString&      searchPattern,
            GroupId                     groupId,
            QHash<PatternId, unsigned>& hitCountsByPatternId
        ) const {
        // Dictionary strings:
        //  1    Once upon a midnight dreary, while I pondered, weak and weary,
        //  2    Over many a quaint and curious volume of forgotten lore—
        //  3        While I nodded, nearly napping, suddenly there came a tapping,
        //  4    As of some one gently rapping, rapping at my chamber door.
        //  5    "’Tis some visitor," I muttered, "tapping at my chamber door—
        //  6        Only this and nothing more."
        //  7    Ah, distinctly I remember it was in the bleak December;
        //  8    And each separate dying ember wrought its ghost upon the floor.
        //  9        Eagerly I wished the morrow;—vainly I had sought to borrow
        // 10        From my books surcease of sorrow—sorrow for the lost Lenore—
        // 11    For the rare and radiant maiden whom the angels name Lenore—
        // 12        Nameless here for evermore.
        //
        // Search string:
        //     "tapping at my chamber door"
        //
        //                                   1  2  3  4  5  6  7  8  9 10 11 12
        //                                  -- -- -- -- -- -- -- -- -- -- -- --
        //     tapping:                            1     1
        //     tapping at:                               2
        //     tapping at my:                            3
        //     tapping at my chamber:                    4
        //     tapping at my chamber door:               5
        //             at:                            1  6
        //             at my:                         2  7
        //             at my chamber:                 3  8
        //             at my chamber door:            4  9
        //                my:                         5 10              1
        //                my chamber:                 6 11
        //                my chamber door:            7 12
        //                   chamber:                 8 13
        //                   chamber door:            9 14
        //                           door:           10 14
        //
        // Selected patterns will be: 5, 4, 3, 10

        unsigned                      numberTokens  = searchPattern.length();
        const TokenizedString::Token* tokens        = searchPattern.tokens();

        if (patternIdsByValueByGroupId.contains(groupId)) {
            const QHash<TokenizedValue, QList<PatternId>>&
                patternIdsByValue = patternIdsByValueByGroupId.value(groupId);

            for (unsigned left=0 ; left<numberTokens ; ++left) {
                TokenizedString s;
                for (unsigned right=left ; right<numberTokens ; ++right) {
                    s.addToken(tokens[right]);
                    QList<PatternId> patternIds = patternIdsByValue.value(s);
                    for (  QList<PatternId>::const_iterator patternIdIterator    = patternIds.constBegin(),
                                                            patternIdEndIterator = patternIds.constEnd()
                         ; patternIdIterator != patternIdEndIterator
                         ; ++patternIdIterator
                        ) {
                        ++hitCountsByPatternId[*patternIdIterator];
                    }
                }
            }
        }
    }


    unsigned qHash(const TokenizedValue& tokenizedValue, HashSeed seed) {
        const TokenizedValue::Token* tokens = tokenizedValue.tokens();
        unsigned                     length = tokenizedValue.length();

        unsigned hash = 0x12345678;
        for (unsigned i=0 ; i<length ; ++i) {
            hash ^= ::qHash(tokens[i], seed);
        }

        return hash;
    }
}
