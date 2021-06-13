#pragma once

#include "Common/CommonTypes.hpp"

#include <QMap>

namespace Qv2rayPlugin::Event
{
    struct SystemProxy
    {
        enum SystemProxyStateType
        {
            SetProxy,
            ClearProxy
        };
        enum SystemProxyType
        {
            SystemProxy_HTTP,
            SystemProxy_SOCKS
        };
        struct EventObject
        {
            QMap<SystemProxyType, int> PortSettings;
            SystemProxyStateType State;
            EventObject(){};
            EventObject(const QMap<SystemProxyType, int> &port, const SystemProxyStateType &type) : PortSettings(port), State(type){};
        };
    };

    struct ConnectionStats
    {
        struct EventObject
        {
            ConnectionId Connection;
            StatisticsObject Statistics;
        };
    };

    struct Connectivity
    {
        enum EventType
        {
            Connecting,
            Connected,
            Disconnecting,
            Disconnected
        };
        struct EventObject
        {
            EventType Type;
            ConnectionGroupPair Connection;
            QMap<QString, PluginIOBoundData> InboundData;
            QMap<QString, PluginIOBoundData> OutboundData;
            EventObject(){};
            EventObject(const EventType &event, const ConnectionGroupPair &conn, const QMap<QString, PluginIOBoundData> &in = {},
                        const QMap<QString, PluginIOBoundData> &out = {})
                : Type(event), Connection(conn), InboundData(in), OutboundData(out){};
        };
    };

    struct ConnectionEntry
    {
        enum EventType
        {
            Created,
            Edited,
            Renamed,
            LinkedWithGroup,
            RemovedFromGroup,
            FullyRemoved
        };
        struct EventObject
        {
            EventType Type;
            GroupId Group;
            ConnectionId Connection;
            QString OriginalName;
        };
    };

    namespace _details
    {
        template<typename... T>
        class Qp
        {
          public:
            void ProcessEvent(){};
        };

        template<typename T1, typename... T2>
        class Qp<T1, T2...> : public Qp<T2...>
        {
          public:
            using Qp<T2...>::ProcessEvent;
            virtual void ProcessEvent(const T1 &pluginEvent){ Q_UNUSED(pluginEvent) };
        };
    } // namespace _details

    class IEventHandler : public _details::Qp<Connectivity::EventObject, SystemProxy::EventObject, ConnectionEntry::EventObject, ConnectionStats::EventObject>
    {
    };
} // namespace Qv2rayPlugin::Event
