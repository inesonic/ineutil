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
* This header defines a fuzzy search engine.
***********************************************************************************************************************/

/* .. sphinx-project ineutil */

#ifndef UTIL_FUZZY_SEARCH_H
#define UTIL_FUZZY_SEARCH_H

#include <QList>
#include <QSet>

#include <cstdint>

#include "util_common.h"
#include "util_string.h"
#include "util_hash_functions.h"

namespace Util {
    /**
     * Class that contains an arbitrary tokenized value.
     */
    class UTIL_PUBLIC_API TokenizedValue {
        public:
            /**
             * Type used to represent the length of a tokenized string.
             */
            typedef std::uint8_t Length;

            /**
             * Type used as an individual token.
             */
            typedef std::uint16_t Token;

            /**
             * Value indicating the maximum number of tokens.
             */
            static constexpr unsigned maximumNumberTokens = 31;

            /**
             * Value indicating an invalid token.
             */
            static const Token invalidToken;

            TokenizedValue();

            /**
             * Constructor
             *
             * \param[in] length The length of the tokenized list.
             *
             * \param[in] tokens The tokens to assign to this tokenized value.
             */
            TokenizedValue(Length length, const Token* tokens);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied to this instance.
             */
            TokenizedValue(const TokenizedValue& other);

            ~TokenizedValue();

            /**
             * Method you can use to obtain token length.
             *
             * \return Returns the token length, in 16-bit entries.
             */
            Length length() const;

            /**
             * Method you can use to obtain the array of tokens.
             *
             * \return Returns an array of tokens.
             */
            const Token* tokens() const;

            /**
             * Method you can use to obtain a list of tokens.
             *
             * \return Returns a list of tokens.
             */
            QList<Token> tokenList() const;

            /**
             * Method you can use to add a new token.
             *
             * \param[in] newToken The new token to be added.
             *
             * \return Returns true on success, returns false if the maximum length has been exceeded.
             */
            bool addToken(Token newToken);

            /**
             * Assignment operator
             *
             * \param[in] other The instance to be copied.
             *
             * \return Returns a reference to this instance.
             */
            TokenizedValue& operator=(const TokenizedValue& other);

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instnace.
             *
             * \return Returns true if the values are equal.  Returns false if the values are not equal.
             */
            bool operator==(const TokenizedValue& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instnace.
             *
             * \return Returns true if the values are different.  Returns false if the values are equal.
             */
            bool operator!=(const TokenizedValue& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instnace.
             *
             * \return Returns true if this value should precede the other value.  Returns false if the values are
             *         equal or this value should follow the other value.
             */
            bool operator<(const TokenizedValue& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instnace.
             *
             * \return Returns true if this value should follow the other value.  Returns false if the values are
             *         equal or this value should precede the other value.
             */
            bool operator>(const TokenizedValue& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instnace.
             *
             * \return Returns true if this value should precede the other value or are equal.  Returns false if this
             *         value should follow the other value.
             */
            bool operator<=(const TokenizedValue& other) const;

            /**
             * Comparison operator.
             *
             * \param[in] other The instance to compare against this instnace.
             *
             * \return Returns true if this value should follow the other value or are equal.  Returns false if this
             *         value should precede the other value.
             */
            bool operator>=(const TokenizedValue& other) const;

        private:
            /**
             * The current length.
             */
            Length currentLength;

            /**
             * The array of tokens.
             */
            Token currentTokens[maximumNumberTokens];
    };

    /**
     * Class that contains a tokenized string.
     */
    class UTIL_PUBLIC_API TokenizedString:public TokenizedValue {
        public:
            TokenizedString();

            /**
             * Constructor
             *
             * \param[in] str             The string to be tokenized.
             *
             * \param[in] assignNewTokens If true, new tokens will be created for this string.  If false, unrecognized
             *                            keywords will be assigned an invalid token number.
             */
            TokenizedString(const String& str, bool assignNewTokens = true);

            /**
             * Constructor
             *
             * \param[in] tv The tokenized value to assign.
             */
            TokenizedString(const TokenizedValue& tv);

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied to this instance.
             */
            TokenizedString(const TokenizedString& other);

            ~TokenizedString();

            /**
             * Method you can call to create an approximated string from this tokenized string.  Spacing and case will
             * be modified and some punctuation will be removed.
             *
             * \return Returns an approximated string for this tokenized string.
             */
            String approximateString() const;

            /**
             * Method you can use to obtain the token for a given keyword.
             *
             * \param[in] keyword        The keyword to obtain the token for.
             *
             * \param[in] assignNewToken If true, a new token will be assigned for the keyword.  If false, a new token
             *                           will not be assigned if the keyword is not already known.
             *
             * \return Returns the token.  An invalid token is returned if the keyword is not defined.
             */
            static Token tokenForKeyword(const String& keyword, bool assignNewToken = true);

        private:
            /**
             * Method that adds a new keyword.
             *
             * \param[in] keyword         The string to be tokenized.
             *
             * \param[in] assignNewTokens If true, new tokens will be created for this string.  If false, unrecognized
             *                            keywords will be assigned an invalid token number.
             *
             * \return Returns true on success, returns false if the string contains too many keywords.
             */
            bool addKeyword(const String& keyword, bool assignNewTokens);

            /**
             * Hash of tokens by keyword.
             */
            static QHash<String, Token> tokensByKeyword;

            /**
             * Hash of keywords by tokenized value.
             */
            static QList<String> keywordsByToken;
    };

    /**
     * Class you can use as a fuzzy search engine.
     */
    class UTIL_PUBLIC_API FuzzySearchEngine {
        public:
            /**
             * Type used to identify a pattern.
             */
            typedef std::uint16_t PatternId;

            /**
             * Type used to represent a group or category for the given pattern.
             */
            typedef std::uint8_t GroupId;

            /**
             * Value used to represent an invalid pattern ID.
             */
            static const PatternId invalidPatternId;

            /**
             * Value used to represent an invalid group ID.
             */
            static const GroupId invalidGroupId;

            FuzzySearchEngine();

            /**
             * Constructor
             *
             * \param[in] locale The two letter (lower case) name of the locale.  This will pre-configure the engine
             *                   with a pre-defined list of stop words based on a known locale.
             */
            FuzzySearchEngine(const String& locale);

            /**
             * Constructor
             *
             * \param[in] stopWords List of stop words to apply to a fuzzy search.  Stop words are ignored when
             *                      performing pattern matching.
             */
            FuzzySearchEngine(const QList<String>& stopWords);

            ~FuzzySearchEngine();

            /**
             * Method that resets the search engine database.
             */
            void clear();

            /**
             * Method that registers a new pattern with the search engine.
             *
             * \param[in] pattern   The pattern to be registered with the search engine.  Stop words will be extracted
             *                      from the pattern.
             *
             * \param[in] groupId   A group to assign this pattern to.
             *
             * \param[in] patternId The ID to assign to this pattern.  This ID will be returned in the list of
             *                      matching patterns.
             */
            void registerPattern(const TokenizedString& pattern, GroupId groupId, PatternId patternId);

            /**
             * Method that generates a list of pattern IDs that match a given pattern and list of groups.
             *
             * \param[in] searchPattern The pattern to be searched.  Stop words will be removed from the pattern.
             *
             * \param[in] groupIds      A list of group Ids that should be included.  An empty list means that all
             *                          groups should be included.
             */
            QList<PatternId> search(
                const TokenizedString& searchPattern = TokenizedString(),
                const QList<GroupId>   groupIds      = QList<GroupId>()
            ) const;

        private:
            /**
             * Method that is used to perform common initialization across all constructors.
             *
             * \param[in] locale The locale to initialize the search engine over.
             */
            void configure(const String& locale);

            /**
             * Method that removes stop words from a tokenized value.
             *
             * \param[in] rawPattern The pattern to have stop words removed from it.
             *
             * \return Returns a newly constructed tokenized value.
             */
            TokenizedString removeStopWordsFrom(const TokenizedString& rawPattern) const;

            /**
             * Method that performs a search within a single group.
             *
             * \param[in]     searchPattern        The pattern to be searched
             *
             * \param[in]     groupId              The ID of the group to be searched.
             *
             * \param[in,out] hitCountsByPatternId A hash of hit rates by pattern ID.
             */
            void searchGroup(
                const TokenizedString&      searchPattern,
                GroupId                     groupId,
                QHash<PatternId, unsigned>& hitCountsByPatternId
            ) const;

            /**
             * Table of english language stop words.  The list should be terminated with a null pointer.
             */
            static const char* englishStopWords[];

            /**
             * Set of known stop words.
             */
            QSet<TokenizedValue::Token> currentStopWords;

            /**
             * Map of pattern IDs by token and value.
             */
            QMap<GroupId, QHash<TokenizedValue, QList<PatternId>>> patternIdsByValueByGroupId;

            /**
             * Lists of pattern IDs by group
             */
            QMap<GroupId, QList<PatternId>> patternIdsByGroupId;
    };

    /**
     * Method that calculates a hash of a tokenized value.
     *
     * \param[in] tokenizedValue The value to calculate the hash for.
     *
     * \param[in] seed           An optional seed to apply to the tokenized value.
     *
     * \return Returns a hash of this tokenized value.
     */
    UTIL_PUBLIC_API unsigned qHash(const TokenizedValue& tokenizedValue, Util::HashSeed seed = 0);
}

#endif
