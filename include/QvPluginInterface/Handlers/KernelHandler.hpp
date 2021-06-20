#pragma once
#include "Common/CommonTypes.hpp"

#include <QObject>
#include <QSet>
#include <QUuid>
#include <functional>

namespace Qv2rayPlugin::Kernel
{
    enum KernelOptionFlags
    {
        KERNEL_HTTP_ENABLED,
        KERNEL_HTTP_PORT,
        KERNEL_SOCKS_ENABLED,
        KERNEL_SOCKS_PORT,
        KERNEL_SOCKS_UDP_ENABLED,
        KERNEL_SOCKS_LOCAL_ADDRESS,
        KERNEL_LISTEN_ADDRESS
    };

    enum KernelCapabilityFlags
    {
        KERNELCAP_ROUTER = 0,
        // KERNELCAP_INBOUNDS, // Unused
        // KERNELCAP_OUTBOUNDS, // Unused
    };

    class PluginKernel : public QObject
    {
      public:
        explicit PluginKernel() : QObject(){};
        virtual ~PluginKernel() = default;
        virtual void SetConnectionSettings(const QMap<KernelOptionFlags, QVariant> &settings, const IOProtocolStreamSettings &connectionInfo) = 0;
        virtual void SetProfileContent(const ProfileContent &){};
        virtual bool Start() = 0;
        virtual bool Stop() = 0;
        virtual KernelId GetKernelId() const = 0;

      signals:
        void OnCrashed(const QString &);
        void OnKernelLog(const QString &);
        void OnStatsAvailable(quint64 upSpeed, quint64 downSpeed);
    };

    struct KernelFactory
    {
        KernelId Id;
        QString Name;
        QSet<QString> SupportedProtocols;
        QFlags<KernelCapabilityFlags> Capabilities;
        std::function<std::unique_ptr<PluginKernel>(void)> Create;
    };

    class IKernelHandler
    {
      public:
        virtual QList<KernelFactory> PluginKernels() const = 0;
    };
} // namespace Qv2rayPlugin::Kernel
