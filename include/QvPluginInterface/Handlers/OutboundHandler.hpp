#pragma once

#include "Common/CommonTypes.hpp"

#include <optional>

namespace Qv2rayPlugin::Outbound
{
    class IOutboundHandler
    {
      public:
        explicit IOutboundHandler(){};
        virtual std::optional<QString> Serialize(const QString &name, const IOConnectionSettings &outbound) const = 0;
        virtual std::optional<std::pair<QString, IOConnectionSettings>> Deserialize(const QString &link) const = 0;

        virtual std::optional<PluginIOBoundData> GetOutboundInfo(const IOConnectionSettings &outbound) const = 0;
        virtual bool SetOutboundInfo(IOConnectionSettings &outbound, const PluginIOBoundData &info) const = 0;

        virtual QList<QString> SupportedProtocols() const = 0;
        virtual QList<QString> SupportedLinkPrefixes() const = 0;
    };
} // namespace Qv2rayPlugin::Outbound
