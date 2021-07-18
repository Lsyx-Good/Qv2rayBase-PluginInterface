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
        virtual SubscriptionResult DecodeSubscription(const QByteArray &data) const = 0;
        virtual SubscriptionResult FetchDecodeSubscription(const SubscriptionProviderSettings &settings) const = 0;
    };

    struct SubscriptionProviderInfo
    {
        Common::EditorCreator::EditorInfo settingsInfo;
        SubscriptionProviderId id;
        QString displayName;
        std::function<std::unique_ptr<SubscriptionProvider>(void)> Creator;
        SubscriptionProviderInfo() = default;
        SubscriptionProviderInfo(const SubscriptionProviderId &t,                                     //
                                 const QString &n,                                                    //
                                 const std::function<std::unique_ptr<SubscriptionProvider>(void)> &c, //
                                 const Common::EditorCreator::EditorInfo &info = Common::EditorCreator::EditorInfo{})
            : settingsInfo(info), id(t), displayName(n)
        {
            Creator = c;
        }
    };

    class IPluginSubscriptionInterface
    {
      public:
        virtual QList<SubscriptionProviderInfo> GetInfo() const = 0;
    };
} // namespace Qv2rayPlugin::Subscription
