/***************************************************************************
 *   Copyright (C) 2010 by Dario Freddi                                    *
 *   drf@chakra-project.org                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#ifndef QAPTAUTHORIZATION_H
#define QAPTAUTHORIZATION_H

#include <PolkitQt1/Authority>
#include <PolkitQt1/Subject>

namespace QApt {
namespace Auth {

inline bool authorize(const QString &action, const QString &service)
{
    PolkitQt1::SystemBusNameSubject subject(service);

    PolkitQt1::Authority::Result result = PolkitQt1::Authority::instance()->checkAuthorizationSync(action, subject,
                                          PolkitQt1::Authority::AllowUserInteraction);

    return (result == PolkitQt1::Authority::Yes);
}

}
}

#endif
