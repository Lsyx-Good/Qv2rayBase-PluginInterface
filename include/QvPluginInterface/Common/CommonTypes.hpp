#pragma once

#include "CommonSafeType.hpp"
#include "Utils/JsonConversion.hpp"

#include <QHash>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <chrono>

template<>
struct QJsonStructSerializer<std::chrono::system_clock::time_point>
{
    static void Deserialize(std::chrono::system_clock::time_point &t, const QJsonValue &d)
    {
        t = std::chrono::system_clock::from_time_t(d.toVariant().toLongLong());
    }
    static QJsonValue Serialize(const std::chrono::system_clock::time_point &t)
    {
        return QJsonValue::fromVariant(QVariant::fromValue<qlonglong>(std::chrono::system_clock::to_time_t(t)));
    }
};

namespace Qv2rayPlugin::Common::_base_types
{
    constexpr unsigned int LATENCY_TEST_VALUE_ERROR = 99999;
    constexpr unsigned int LATENCY_TEST_VALUE_NODATA = LATENCY_TEST_VALUE_ERROR - 1;

    const static inline ConnectionId NullConnectionId;

    const static inline GroupId NullGroupId;
    const static inline GroupId DefaultGroupId{ "000000000000" };

    const static inline RoutingId NullRoutingId;
    const static inline RoutingId DefaultRoutingId{ "000000000000" };

    const static inline KernelId NullKernelId;

    struct ConnectionGroupPair
    {
        ConnectionId connectionId = NullConnectionId;
        GroupId groupId = NullGroupId;

        // clang-format off
        ConnectionGroupPair(){};
        ConnectionGroupPair(const ConnectionId &conn, const GroupId &group) { connectionId = conn, groupId = group; }
        inline bool operator==(const ConnectionGroupPair &rhs) const { return connectionId == rhs.connectionId && groupId == rhs.groupId; }
        inline bool operator!=(const ConnectionGroupPair &rhs) const { return !(*this == rhs); }
        inline void clear() { connectionId = NullConnectionId, groupId = NullGroupId; }
        inline bool isNull() const { return groupId == NullGroupId || connectionId == NullConnectionId; }
        // clang-format on
        QJS_FUNC_JSON(F(connectionId, groupId))
    };

    struct StatisticsObject
    {
        enum StatisticsType
        {
            API_ALL_INBOUND = 0,
            API_OUTBOUND_PROXY = 1,
            API_OUTBOUND_DIRECT = 2,
        };

        // clang-format off
        struct StatsEntry { quint64 up; quint64 down; QJS_FUNC_JSON(F(up, down)) };
        StatsEntry &operator[](StatisticsType i) { while (entries.count() <= i) entries.append(StatsEntry{}); return entries[i]; }
        void clear() { entries.clear(); }
        QJsonObject toJson() const { return JsonStructHelper::Serialize(entries).toObject(); }
        void loadJson(const QJsonValue &d) { JsonStructHelper::Deserialize(entries, d); }
        // clang-format on

      private:
        QList<StatsEntry> entries;
    };

    struct BaseTaggedObject
    {
        QString name;
        QJsonObject options;
        QJS_FUNC_JSON(F(name, options))
    };

    struct BaseConfigTaggedObject : public BaseTaggedObject
    {
        system_clock::time_point created = system_clock::now();
        system_clock::time_point updated = system_clock::now();
        QJS_FUNC_JSON(F(created, updated), B(BaseTaggedObject))
    };

    struct ConnectionObject : public BaseConfigTaggedObject
    {
        system_clock::time_point last_connected;
        StatisticsObject statistics;
        long latency = LATENCY_TEST_VALUE_NODATA;
        int _group_ref = 0;
        QJS_FUNC_JSON(F(last_connected, statistics), B(BaseConfigTaggedObject))
    };

    struct SubscriptionConfigObject : public BaseTaggedObject
    {
        enum SubscriptionFilterRelation
        {
            RELATION_AND = 0,
            RELATION_OR = 1
        };
        bool isSubscription;
        QString address;
        QString type = "sip008";
        float updateInterval = 10;
        QList<QString> includeKeywords;
        QList<QString> excludeKeywords;
        SubscriptionFilterRelation includeRelation = RELATION_OR;
        SubscriptionFilterRelation excludeRelation = RELATION_AND;
        QJS_FUNC_JSON(F(isSubscription, address, type, updateInterval, includeKeywords, excludeKeywords, includeRelation, excludeRelation), B(BaseTaggedObject))
    };

    struct GroupObject : public BaseConfigTaggedObject
    {
        QList<ConnectionId> connections;
        RoutingId route_id = NullRoutingId;
        SubscriptionConfigObject subscription_config;
        QJS_FUNC_JSON(F(connections, route_id, subscription_config), B(BaseConfigTaggedObject))
    };

    struct PortRange
    {
        int from, to;
        operator QString() const
        {
            return QString::number(from) + "-" + QString::number(to);
        }
        QJS_FUNC_JSON(F(from, to))
    };

    struct RuleObject : public BaseTaggedObject
    {
        bool enabled = true;

        QStringList inboundTags;
        QString outboundTag;

        QStringList sourceAddresses;
        QStringList targetDomains;
        QStringList targetIPs;

        PortRange sourcePort;
        PortRange targetPort;

        QStringList networks;
        QStringList protocols;

        QStringList processes;

        RuleExtraSettings extraSettings = RuleExtraSettings{};
        QJS_FUNC_JSON(F(enabled, inboundTags, outboundTag, sourceAddresses, targetDomains, targetIPs, sourcePort, targetPort, networks, protocols, processes,
                        extraSettings))
    };

    struct RoutingObject : public BaseConfigTaggedObject
    {
        QList<RuleObject> rules;
        int _group_ref = 0;
        QJS_FUNC_JSON(F(rules), B(BaseConfigTaggedObject))
    };

    struct IOConnectionSettings
    {
        QString protocol;
        IOProtocolSettings protocolSettings = IOProtocolSettings{};
        IOStreamSettings streamSettings = IOStreamSettings{};
        QJS_FUNC_JSON(F(protocol, protocolSettings, streamSettings))
    };

    struct InboundObject : public BaseTaggedObject
    {
        QString listenAddress;
        QString listenPort;
        IOConnectionSettings inboundSettings;
        InboundExtraSettings extraSettings = InboundExtraSettings{};
        static InboundObject Create(QString name, QString proto, QString addr, QString port, //
                                    IOProtocolSettings protocol = IOProtocolSettings{},      //
                                    IOStreamSettings stream = IOStreamSettings{})
        {
            InboundObject in;
            in.name = name;
            in.listenAddress = addr;
            in.listenPort = port;
            in.inboundSettings.protocol = proto;
            in.inboundSettings.protocolSettings = protocol;
            in.inboundSettings.streamSettings = stream;
            return in;
        }
        QJS_FUNC_JSON(F(listenAddress, listenPort, inboundSettings, extraSettings), B(BaseTaggedObject))
    };

    struct BalancerSettings : public BaseTaggedObject
    {
        QString selectorType;
        BalancerSelectorSettings selectorSettings = BalancerSelectorSettings{};
        QJS_FUNC_JSON(F(selectorType, selectorSettings))
    };

    struct ChainSettings : public BaseTaggedObject
    {
        int chaining_port;
        QStringList chains;
        QJS_FUNC_JSON(F(chains, chaining_port), B(BaseTaggedObject))
    };

    struct OutboundObject : public BaseTaggedObject
    {
        enum OutboundObjectType
        {
            ORIGINAL,
            EXTERNAL,
            BALANCER,
            CHAIN
        };

        OutboundObject(){};
        OutboundObject(const IOConnectionSettings &settings) : objectType(ORIGINAL), outboundSettings(settings){};
        OutboundObject(const ConnectionId &external) : objectType(EXTERNAL), externalId(external){};
        OutboundObject(const BalancerSettings &balancer) : objectType(BALANCER), balancerSettings(balancer){};
        OutboundObject(const ChainSettings &chain) : objectType(CHAIN), chainSettings(chain){};

        OutboundObjectType objectType = ORIGINAL;

        KernelId kernel = NullKernelId;

        ConnectionId externalId = NullConnectionId;
        IOConnectionSettings outboundSettings;
        BalancerSettings balancerSettings;
        ChainSettings chainSettings;

        QJS_FUNC_JSON(F(objectType, kernel, externalId, outboundSettings, balancerSettings, chainSettings), B(BaseTaggedObject))
    };

    struct ProfileContent : public BaseTaggedObject
    {
        ProfileContent() : BaseTaggedObject(){};
        explicit ProfileContent(const OutboundObject &out) : BaseTaggedObject()
        {
            outbounds << out;
        };
        KernelId defaultKernel = NullKernelId;
        QList<InboundObject> inbounds;
        QList<OutboundObject> outbounds;
        RoutingObject routing;
        QJS_FUNC_JSON(F(defaultKernel, inbounds, outbounds, routing), B(BaseTaggedObject))
        static auto fromJson(const QJsonObject &o)
        {
            ProfileContent profile;
            profile.loadJson(o);
            return profile;
        };
    };

    enum IOBOUND_DATA_TYPE
    {
        IO_DISPLAYNAME = 0,
        IO_PROTOCOL = 1,
        IO_ADDRESS = 2,
        IO_PORT = 3,
        IO_SNI = 4
    };

    inline size_t qHash(const ConnectionGroupPair &c, size_t seed) noexcept
    {
        return qHashMulti(seed, c.connectionId, c.groupId);
    }

    typedef QMap<IOBOUND_DATA_TYPE, QVariant> PluginIOBoundData;
} // namespace Qv2rayPlugin::Common::_base_types

// Expose all basic type decls to global namespace
using namespace Qv2rayPlugin::Common::_base_types;

Q_DECLARE_METATYPE(ConnectionGroupPair)
Q_DECLARE_METATYPE(ConnectionObject)
Q_DECLARE_METATYPE(GroupObject)
Q_DECLARE_METATYPE(RoutingObject)
Q_DECLARE_METATYPE(ChainSettings)
Q_DECLARE_METATYPE(IOConnectionSettings)
Q_DECLARE_METATYPE(BalancerSettings)
Q_DECLARE_METATYPE(OutboundObject)
Q_DECLARE_METATYPE(ProfileContent)
