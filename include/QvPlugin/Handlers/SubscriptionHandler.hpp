#pragma once

#include "QvPlugin/Common/CommonTypes.hpp"

#include <QJsonObject>
#include <QList>

namespace Qv2rayPlugin::Subscription
{
    struct SubscriptionInfoObject
    {
      public:
        QString type;
        QString displayName;
        explicit SubscriptionInfoObject(){};
        explicit SubscriptionInfoObject(const QString &protocol, const QString &displayName) : type(protocol), displayName(displayName){};
        friend bool operator==(const SubscriptionInfoObject &l, const SubscriptionInfoObject &r)
        {
            return l.type == r.type && l.displayName == r.displayName;
        }
    };

    class SubscriptionDecoder
    {
      public:
        struct SubscriptionDecodeResult
        {
            QList<QString> links;
            QList<std::pair<QString, ProfileContent>> connections;
        };
        virtual ~SubscriptionDecoder(){};
        virtual SubscriptionDecodeResult DecodeData(const QByteArray &) const = 0;
    };

    class ISubscriptionHandler
    {
      public:
        virtual QList<SubscriptionInfoObject> SupportedSubscriptionTypes() const = 0;
        virtual std::shared_ptr<SubscriptionDecoder> GetSubscriptionDecoder(const QString &type) const = 0;
    };
} // namespace Qv2rayPlugin::Subscription
