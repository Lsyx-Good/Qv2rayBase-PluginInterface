#pragma once

#include "Connections/ConnectionsBaseTypes.hpp"

#include <optional>

namespace Qv2rayPlugin::Outbound
{
    class IOutboundHandler
    {
      public:
        explicit IOutboundHandler(){};
        virtual std::optional<QString> Serialize(const QString &name, const OutboundObject &outbound) const = 0;
        virtual std::optional<std::pair<QString, OutboundObject>> Deserialize(const QString &link) const = 0;

        virtual std::optional<PluginIOBoundData> GetOutboundInfo(const QString &protocol, const OutboundSettings &outbound) const = 0;
        virtual bool SetOutboundInfo(const QString &protocol, OutboundSettings &outbound, const PluginIOBoundData &info) const = 0;

        virtual QList<QString> SupportedProtocols() const = 0;
        virtual QList<QString> SupportedLinkPrefixes() const = 0;
    };
} // namespace Qv2rayPlugin::Outbound
