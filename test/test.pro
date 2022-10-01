##-*-makefile-*-########################################################################################################
# Copyright 2016 - 2022 Inesonic, LLC
# 
# This file is licensed under two licenses.
#
# Inesonic Commercial License, Version 1:
#   All rights reserved.  Inesonic, LLC retains all rights to this software, including the right to relicense the
#   software in source or binary formats under different terms.  Unauthorized use under the terms of this license is
#   strictly prohibited.
#
# GNU Public License, Version 2:
#   This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
#   License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
#   version.
#   
#   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
#   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
#   details.
#   
#   You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
#   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
########################################################################################################################

########################################################################################################################
# Basic build characteristics
#

TEMPLATE = app
QT += core testlib
CONFIG += testcase c++14

HEADERS = test_bit_functions.h \
          test_bit_set.h \
          test_bit_array.h \
          test_page_size.h \
          test_string.h \
          test_fuzzy_search.h \

SOURCES = test_ineutil.cpp \
          test_bit_functions.cpp \
          test_bit_set.cpp \
          test_bit_array.cpp \
          test_page_size.cpp \
          test_string.cpp \
          test_fuzzy_search.cpp \

########################################################################################################################
# Libraries
#

INEUTIL_BASE = $${OUT_PWD}/../ineutil
INCLUDEPATH += $${PWD}/../ineutil/include

INCLUDEPATH += $${BOOST_INCLUDE}

unix {
    CONFIG(debug, debug|release) {
        LIBS += -L$${INEUTIL_BASE}/build/debug/ -lineutil
        !macx {
            PRE_TARGETDEPS += $${INEUTIL_BASE}/build/debug/libineutil.so
        } else {
            PRE_TARGETDEPS += $${INEUTIL_BASE}/build/debug/libineutil.dylib
        }
    } else {
        LIBS += -L$${INEUTIL_BASE}/build/release/ -lineutil
        !macx {
            PRE_TARGETDEPS += $${INEUTIL_BASE}/build/release/libineutil.so
        } else {
            PRE_TARGETDEPS += $${INEUTIL_BASE}/build/release/libineutil.dylib
        }
    }
}

win32 {
    CONFIG(debug, debug|release) {
        LIBS += $${INEUTIL_BASE}/build/Debug/ineutil.lib
        PRE_TARGETDEPS += $${INEUTIL_BASE}/build/Debug/ineutil.lib
    } else {
        LIBS += $${INEUTIL_BASE}/build/Release/ineutil.lib
        PRE_TARGETDEPS += $${INEUTIL_BASE}/build/Release/ineutil.lib
    }
}

########################################################################################################################
# Locate build intermediate and output products
#

TARGET = test_ineutil

CONFIG(debug, debug|release) {
    unix:DESTDIR = build/debug
    win32:DESTDIR = build/Debug
} else {
    unix:DESTDIR = build/release
    win32:DESTDIR = build/Release
}

OBJECTS_DIR = $${DESTDIR}/objects
MOC_DIR = $${DESTDIR}/moc
RCC_DIR = $${DESTDIR}/rcc
UI_DIR = $${DESTDIR}/ui
