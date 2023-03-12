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

TEMPLATE = lib

########################################################################################################################
# Basic build characteristics
#

QT += core network
CONFIG += shared c++14

DEFINES += UTIL_BUILD

########################################################################################################################
# Inesonic Public includes
#

INCLUDEPATH += include
API_HEADERS = include/util_common.h \
              include/util_units.h \
              include/util_algorithm.h \
              include/util_bit_functions.h \
              include/util_bit_array.h \
              include/util_bit_set.h \
              include/util_color_functions.h \
              include/util_shape_functions.h \
              include/util_hash_functions.h \
              include/util_page_size.h \
              include/util_system.h \
              include/util_string.h \
              include/util_fuzzy_search.h \

########################################################################################################################
# Source files
#

SOURCES = source/util_bit_functions.cpp \
          source/util_bit_array.cpp \
          source/util_bit_array_private.cpp \
          source/util_bit_set.cpp \
          source/util_color_functions.cpp \
          source/util_shape_functions.cpp \
          source/util_hash_functions.cpp \
          source/util_page_size.cpp \
          source/util_system.cpp \
          source/util_string.cpp \
          source/util_fuzzy_search.cpp \

########################################################################################################################
# Inesonic private includes
#

PRIVATE_HEADERS = source/util_bit_array_private.h

########################################################################################################################
# Setup headers and installation
#

HEADERS = $$API_HEADERS $$PRIVATE_HEADERS

########################################################################################################################
# Operating System
#

win32 {
    defined(SETTINGS_PRI, var) {
        include($${SETTINGS_PRI})
    }

    LIBS += "$${WINDOWS_KIT_LIBDIR}/AdvAPI32.Lib"
}

########################################################################################################################
# Locate build intermediate and output products
#

TARGET = ineutil

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
