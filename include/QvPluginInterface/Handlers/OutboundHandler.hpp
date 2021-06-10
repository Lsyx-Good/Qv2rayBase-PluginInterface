#pragma once

#include "Connections/ConnectionsBaseTypes.hpp"

#include <optional>

namespace Qv2rayPlugin::Outbound
{
    enum IOBOUND_DATA_TYPE
    {
        IO_DISPLAYNAME = 0,
        IO_PROTOCOL = 1,
        IO_ADDRESS = 2,
        IO_PORT = 3,
        IO_SNI = 4
    };
    typedef QMap<IOBOUND_DATA_TYPE, QVariant> PluginIOBoundData;

    struct PluginOutboundDescriptor
    {
        QString ConnectionName;
        QString Protocol;
        QJsonObject Outbound;
        QJsonObject StreamSettings;
    };

    class IOutboundHandler
    {
      public:
        explicit IOutboundHandler(){};
        virtual std::optional<QString> Serialize(const PluginOutboundDescriptor &outbound) const = 0;
        virtual std::optional<PluginOutboundDescriptor> Deserialize(const QString &link) const = 0;

        virtual std::optional<PluginIOBoundData> GetOutboundInfo(const QString &protocol, const QJsonObject &outbound) const = 0;
        virtual bool SetOutboundInfo(const QString &protocol, QJsonObject &outbound, const PluginIOBoundData &info) const = 0;

        virtual QList<QString> SupportedProtocols() const = 0;
        virtual QList<QString> SupportedLinkPrefixes() const = 0;
    };
} // namespace Qv2rayPlugin::Outbound
