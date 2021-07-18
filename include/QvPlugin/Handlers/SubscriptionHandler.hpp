#pragma once

#include "QvPlugin/Common/CommonTypes.hpp"
#include "QvPlugin/Common/EditorCreatorDefs.hpp"

#include <QJsonObject>
#include <QList>

namespace Qv2rayPlugin::Subscription
{
    struct SubscriptionResult
    {
        QList<QString> links;
        QList<OutboundObject> outbounds;
        QMultiMap<QString, ProfileContent> connections;
        RoutingObject routing;
    };

    class SubscriptionProvider
    {
      public:
        virtual ~SubscriptionProvider() = default;
        virtual SubscriptionResult DecodeSubscription(const QByteArray &data) const
        {
            Q_UNUSED(data);
            Q_UNREACHABLE();
        }
        virtual SubscriptionResult FetchDecodeSubscription(const SubscriptionProviderOptions &options) const
        {
            Q_UNUSED(options);
            Q_UNREACHABLE();
        }
    };

    enum SubscribingMode
    {
        Subscribe_Decoder,
        Subscribe_FetcherAndDecoder,
    };

    struct SubscriptionProviderInfo
    {
        SubscriptionProviderId id;
        SubscribingMode mode;
        QString displayName;
        Common::EditorCreator::EditorInfoList settings;
        std::function<std::unique_ptr<SubscriptionProvider>(void)> Creator;

        template<typename T>
        static SubscriptionProviderInfo CreateDecoder(const SubscriptionProviderId &id, const QString &name)
        {
            SubscriptionProviderInfo info;
            info.id = id;
            info.displayName = name;
            info.Creator = []() { return std::make_unique<T>(); };
            info.mode = Subscribe_Decoder;
            return info;
        }

        template<typename T>
        static SubscriptionProviderInfo CreateFetcherDecoder(const SubscriptionProviderId &id, const QString &name, const Common::EditorCreator::EditorInfoList &settings)
        {
            SubscriptionProviderInfo info;
            info.id = id;
            info.displayName = name;
            info.settings = settings;
            info.Creator = []() { return std::make_unique<T>(); };
            info.mode = Subscribe_FetcherAndDecoder;
            return info;
        }
    };

    class IPluginSubscriptionInterface
    {
      public:
        virtual QList<SubscriptionProviderInfo> GetInfo() const = 0;
    };
} // namespace Qv2rayPlugin::Subscription
