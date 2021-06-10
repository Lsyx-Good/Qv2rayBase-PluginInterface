#pragma once

#include "ConnectionsSafeTypes.hpp"

#include <QHash>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <chrono>

namespace Qv2rayPlugin::Connections::_base_types
{
    inline const static ConnectionId NullConnectionId;

    inline const static GroupId NullGroupId;
    inline const static GroupId DefaultGroupId{ "000000000000" };

    inline const static RoutingId NullRoutingId;
    inline const static RoutingId DefaultRoutingId{ "000000000000" };

    struct ConnectionGroupPair
    {
        ConnectionId connectionId = NullConnectionId;
        GroupId groupId = NullGroupId;

        // clang-format off
        ConnectionGroupPair(){};
        ConnectionGroupPair(const ConnectionId &conn, const GroupId &group) { connectionId = conn, groupId = group; }
        bool operator==(const ConnectionGroupPair &rhs) const { return connectionId == rhs.connectionId && groupId == rhs.groupId; }
        bool operator!=(const ConnectionGroupPair &rhs) const { return !(*this == rhs); }
        void clear() { connectionId = NullConnectionId, groupId = NullGroupId; }
        bool isNull() const { return groupId == NullGroupId || connectionId == NullConnectionId; }
        // clang-format on
    };

    struct ConfigObjectBase
    {
        QString name;
        QVariantMap attributes;
        system_clock::time_point created = system_clock::now();
        system_clock::time_point updated = system_clock::now();
    };

    struct ConnectionObject : public ConfigObjectBase
    {
        system_clock::time_point last_connected;
        int _group_ref = 0;
    };

    struct GroupObject : public ConfigObjectBase
    {
        QList<ConnectionId> connections;
        RoutingId route_id = NullRoutingId;
    };

    struct RoutingObject : public ConfigObjectBase
    {
        int _group_ref = 0;
    };

} // namespace Qv2rayPlugin::Connections::_base_types

// Expose all basic type decls to global namespace

using namespace Qv2rayPlugin::Connections::_base_types;

Q_DECLARE_METATYPE(ConnectionGroupPair)
