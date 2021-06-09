#pragma once

#include "Connections/ConnectionsBaseTypes.hpp"

#include <QJsonObject>
#include <QMap>
#include <optional>

namespace Qv2rayPlugin::handlers::outbound
{
    enum class IOBOUND
    {
        DISPLAYNAME = 0,
        PROTOCOL = 1,
        ADDRESS = 2,
        PORT = 3,
        SNI = 4
    };
    typedef QMap<IOBOUND, QVariant> PluginIOBoundData;

    struct PluginOutboundDescriptor
    {
        QString ConnectionName;
        QString Protocol;
        QJsonObject Outbound;
        QJsonObject StreamSettings;
    };

    class PluginOutboundHandler
    {
      public:
        explicit PluginOutboundHandler(){};
        virtual std::optional<QString> Serialize(const PluginOutboundDescriptor &outbound) const = 0;
        virtual std::optional<PluginOutboundDescriptor> Deserialize(const QString &link) const = 0;

        virtual std::optional<PluginIOBoundData> GetOutboundInfo(const QString &protocol, const QJsonObject &outbound) const = 0;
        virtual bool SetOutboundInfo(const QString &protocol, QJsonObject &outbound, const PluginIOBoundData &info) const = 0;

        virtual QList<QString> SupportedProtocols() const = 0;
        virtual QList<QString> SupportedLinkPrefixes() const = 0;
    };
} // namespace Qv2rayPlugin::handlers::outbound
