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
* This file implements a small collection of system abstraction functions.
***********************************************************************************************************************/

#include <QString>
#include <QtGlobal>
#include <QNetworkInterface>

#if (defined(Q_OS_WIN))

    #include <Windows.h>
    #include <WinBase.h>
    #include <FileAPI.h>
    #include <Lmcons.h>

#elif (defined(Q_OS_LINUX) || defined(Q_OS_DARWIN))

    #include <unistd.h>

#endif

#include "util_system.h"

namespace Util {
    QString username() {
        QString result;

        #if (defined(Q_OS_WIN))

            TCHAR   usernameArray[UNLEN + 1];
            DWORD   usernameArraySize = UNLEN + 1;
            int     success = GetUserName(usernameArray, &usernameArraySize);

            if (success != 0) {

                #if (defined(UNICODE))

                    result = QString::fromWCharArray(usernameArray);

                #else

                    result = QString::fromLocal8Bit(usernameArray);

                #endif

            } else {
                result = qgetenv("USERNAME");
            }

        #elif (defined(Q_OS_LINUX) || defined(Q_OS_DARWIN))

            char* usernameArray = getlogin();
            if (usernameArray != nullptr) {
                result = QString::fromLocal8Bit(usernameArray);
            } else {
                result = qgetenv("USER");
            }

        #else

            #error Unknown platform

        #endif

        return result;
    }


    QList<std::uint64_t> macAddresses() {
        QList<std::uint64_t> result;

        QList<QNetworkInterface>                 interfaces           = QNetworkInterface::allInterfaces();
        QList<QNetworkInterface>::const_iterator interfaceIterator    = interfaces.constBegin();
        QList<QNetworkInterface>::const_iterator interfaceEndIterator = interfaces.constEnd();

        while (interfaceIterator != interfaceEndIterator) {
            QStringList                 bytes           = interfaceIterator->hardwareAddress().split(":");
            QStringList::const_iterator byteIterator    = bytes.constBegin();
            QStringList::const_iterator byteEndIterator = bytes.constEnd();
            std::uint64_t               macAddress      = 0;

            while (byteIterator != byteEndIterator) {
                bool     isOk;
                unsigned byte = byteIterator->toUInt(&isOk, 16);
                if (byte > 255) {
                    isOk = false;
                }

                if (isOk) {
                    macAddress = (macAddress << 8) | byte;
                }

                ++byteIterator;
            }

            result.append(macAddress);
            ++interfaceIterator;
        }

        return result;
    }
}
