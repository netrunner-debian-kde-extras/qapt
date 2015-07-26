/***************************************************************************
 *   Copyright © 2011 Jonathan Thomas <echidnaman@kubuntu.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation; either version 2 of        *
 *   the License or (at your option) version 3 or any later version        *
 *   accepted by the membership of KDE e.V. (or its successor approved     *
 *   by the membership of KDE e.V.), which shall act as a proxy            *
 *   defined in Section 14 of version 3 of the license.                    *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "PluginInfo.h"

#include <QDebug>
#include <QStringBuilder>
#include <QStringList>

PluginInfo::PluginInfo(const QString &gstDetails)
          : m_structure(nullptr)
          , m_pluginType(InvalidType)
          , m_data(gstDetails)
          , m_isValid(true)
{
    parseDetails(gstDetails);
}

PluginInfo::~PluginInfo()
{
    gst_structure_free(m_structure);
}

void PluginInfo::parseDetails(const QString &gstDetails)
{
    QStringList parts = gstDetails.split('|');

    if (parts.count() != 5) {
        m_isValid = false;
        return;
    }

    m_version = parts.at(1);
    m_requestedBy = parts.at(2);
    m_name = parts.at(3);
    m_capsInfo = parts.at(4);

    QStringList ss;

    if (m_capsInfo.startsWith(QLatin1String("uri"))) {
        // Split URI
        ss = parts.at(4).split(QLatin1Char(' '));

        if (!ss.isEmpty()) {
            m_typeName =  parts.at(0);
        } else {
            m_isValid = false;
        }
        return;
    }

    // Everything up to the first '-' is the typeName
    m_typeName = parts.at(4).section('-', 0, 0);
    m_capsInfo.remove(m_typeName + '-');

    if (m_typeName == "encoder") {
        m_pluginType = Encoder;
    } else if (m_typeName == "decoder") {
        m_pluginType = Decoder;
    } else if (m_typeName== "urisource") {
        m_pluginType = UriSource;
    } else if (m_typeName == "urisink") {
        m_pluginType = UriSink;
    } else if (m_typeName == "element") {
        m_pluginType = Element;
    } else {
        qDebug() << "invalid plugin type";
        m_pluginType = InvalidType;
    }

    m_structure = gst_structure_new_from_string(m_capsInfo.toUtf8().constData());
    if (!m_structure) {
        qDebug() << "Failed to parse structure: " << m_capsInfo;
        m_isValid = false;
        return;
    }

    /* remove fields that are almost always just MIN-MAX of some sort
     * in order to make the caps look less messy */
    gst_structure_remove_field(m_structure, "pixel-aspect-ratio");
    gst_structure_remove_field(m_structure, "framerate");
    gst_structure_remove_field(m_structure, "channels");
    gst_structure_remove_field(m_structure, "width");
    gst_structure_remove_field(m_structure, "height");
    gst_structure_remove_field(m_structure, "rate");
    gst_structure_remove_field(m_structure, "depth");
    gst_structure_remove_field(m_structure, "clock-rate");
    gst_structure_remove_field(m_structure, "bitrate");
}

QString PluginInfo::version() const
{
    return m_version;
}

QString PluginInfo::requestedBy() const
{
    return m_requestedBy;
}

QString PluginInfo::name() const
{
    return m_name;
}

QString PluginInfo::capsInfo() const
{
    return m_capsInfo;
}

int PluginInfo::pluginType() const
{
    return m_pluginType;
}

QString PluginInfo::data() const
{
    return m_data;
}

QString PluginInfo::typeName() const
{
    return m_typeName;
}

bool PluginInfo::isValid() const
{
    return m_isValid;
}
