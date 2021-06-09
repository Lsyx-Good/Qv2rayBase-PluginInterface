#pragma once

#include <QHash>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <chrono>

namespace Qv2rayPlugin::connections
{
    using namespace std::chrono;
    template<typename, typename BASETYPE_T>
    struct SafeJsonType : public BASETYPE_T
    {
        // clang-format off
        template<class... Args> explicit SafeJsonType(Args... args) : BASETYPE_T(args...) {};
        const BASETYPE_T &raw() const { return *this; }
        // clang-format on
    };

    template<typename T>
    struct IDType
    {
        // clang-format off
        explicit IDType() : m_id("null"){};
        explicit IDType(const QString &id) : m_id(id){};
        bool operator==(const IDType<T> &rhs) const { return m_id == rhs.m_id; }
        bool operator!=(const IDType<T> &rhs) const { return m_id != rhs.m_id; }
        const QString toString() const { return m_id; }
        bool isNull() const { return m_id == "null"; }
        // clang-format on

      private:
        QString m_id;
    };

#define DeclareSafeJson(BASE, CLASS)                                                                                                                                     \
    class __##CLASS##__;                                                                                                                                                 \
    typedef SafeJsonType<__##CLASS##__, BASE> CLASS;

#define DeclareSafeID(type)                                                                                                                                              \
    class __##type;                                                                                                                                                      \
    typedef IDType<__##type> type

    DeclareSafeJson(QJsonObject, INBOUNDSETTING);
    DeclareSafeJson(QJsonObject, OUTBOUNDSETTING);
    DeclareSafeJson(QJsonObject, INBOUND);
    DeclareSafeJson(QJsonObject, OUTBOUND);
    DeclareSafeJson(QJsonObject, CONFIGROOT);
    DeclareSafeJson(QJsonObject, ROUTING);
    DeclareSafeJson(QJsonObject, ROUTERULE);
    DeclareSafeJson(QJsonArray, OUTBOUNDS);
    DeclareSafeJson(QJsonArray, INBOUNDS);

    DeclareSafeID(GroupId);
    DeclareSafeID(ConnectionId);
    DeclareSafeID(RoutingId);

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
        RoutingId route_id;
    };

    struct RoutingObject : public ConfigObjectBase
    {
        int _group_ref = 0;
    };

} // namespace Qv2rayPlugin::connections

template<typename T>
inline size_t qHash(Qv2rayPlugin::connections::IDType<T> key)
{
    return ::qHash(key.toString());
}

Q_DECLARE_METATYPE(Qv2rayPlugin::connections::ConnectionId)
Q_DECLARE_METATYPE(Qv2rayPlugin::connections::GroupId)
Q_DECLARE_METATYPE(Qv2rayPlugin::connections::RoutingId)
Q_DECLARE_METATYPE(Qv2rayPlugin::connections::ConnectionGroupPair)
