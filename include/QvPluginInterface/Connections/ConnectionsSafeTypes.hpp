#pragma once

#include <QHashFunctions>
#include <QJsonObject>
#include <QString>
#include <chrono>

namespace Qv2rayPlugin::Connections::_base_types::safetype
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
        inline bool operator==(const IDType<T> &rhs) const { return m_id == rhs.m_id; }
        inline bool operator!=(const IDType<T> &rhs) const { return m_id != rhs.m_id; }
        inline const QString toString() const { return m_id; }
        inline bool isNull() const { return m_id == "null"; }
        // clang-format on

      private:
        QString m_id;
    };

#define DeclareSafeJson(BASE, CLASS)                                                                                                                                     \
    class __##CLASS##__;                                                                                                                                                 \
    typedef Qv2rayPlugin::Connections::_base_types::safetype::SafeJsonType<__##CLASS##__, BASE> CLASS;

#define DeclareSafeID(type)                                                                                                                                              \
    class __##type;                                                                                                                                                      \
    typedef Qv2rayPlugin::Connections::_base_types::safetype::IDType<__##type> type

    DeclareSafeJson(QJsonObject, ProfileContent);

    DeclareSafeJson(QJsonObject, INBOUNDSETTING);
    DeclareSafeJson(QJsonObject, OUTBOUNDSETTING);
    DeclareSafeJson(QJsonObject, INBOUND);
    DeclareSafeJson(QJsonObject, OUTBOUND);
    DeclareSafeJson(QJsonObject, ROUTING);
    DeclareSafeJson(QJsonObject, ROUTERULE);
    DeclareSafeJson(QJsonArray, OUTBOUNDS);
    DeclareSafeJson(QJsonArray, INBOUNDS);

    DeclareSafeID(GroupId);
    DeclareSafeID(ConnectionId);
    DeclareSafeID(RoutingId);
    DeclareSafeID(PluginId);

    template<typename T>
    inline size_t qHash(const Qv2rayPlugin::Connections::_base_types::safetype::IDType<T> &key) noexcept
    {
        return ::qHash(key.toString());
    }

} // namespace Qv2rayPlugin::Connections::_base_types::safetype

using namespace Qv2rayPlugin::Connections::_base_types::safetype;

Q_DECLARE_METATYPE(ConnectionId)
Q_DECLARE_METATYPE(GroupId)
Q_DECLARE_METATYPE(RoutingId)
Q_DECLARE_METATYPE(PluginId)
