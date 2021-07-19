#pragma once
#include <QNetworkReply>

namespace Qv2rayPlugin::Utils
{
    class INetworkRequestHelper
    {
      public:
        virtual std::tuple<QNetworkReply::NetworkError, QString, QByteArray> Get(const QUrl &url) = 0;
    };

} // namespace Qv2rayPlugin::Utils
