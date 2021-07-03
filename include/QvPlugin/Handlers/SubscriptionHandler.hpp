#pragma once

#include "QvPlugin/Common/CommonTypes.hpp"

#include <QJsonObject>
#include <QList>

namespace Qv2rayPlugin::Subscription
{
    struct SubscriptionDecodeResult
    {
        QList<QString> links;
        QList<OutboundObject> outbounds;
        QMultiMap<QString, ProfileContent> connections;
    };

    class SubscriptionDecoder
    {
      public:
        virtual ~SubscriptionDecoder() = default;
        virtual SubscriptionDecodeResult DecodeData(const QByteArray &) const = 0;
    };

    struct SubscriptionInfoObject
    {
        SubscriptionDecoderId type;
        QString displayName;
        std::function<std::unique_ptr<SubscriptionDecoder>(void)> Creator;
        SubscriptionInfoObject() = default;
        SubscriptionInfoObject(const SubscriptionDecoderId &t, const QString &n, const decltype(Creator) &c) : type(t), displayName(n)
        {
            Creator = c;
        }
        friend bool operator==(const SubscriptionInfoObject &l, const SubscriptionInfoObject &r)
        {
            return l.type == r.type && l.displayName == r.displayName;
        }
    };

    class ISubscriptionHandler
    {
      public:
        virtual QList<SubscriptionInfoObject> GetInfo() const = 0;
    };
} // namespace Qv2rayPlugin::Subscription
