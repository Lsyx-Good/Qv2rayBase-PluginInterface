#pragma once

#include "Connections/ConnectionsBase.hpp"
#include "Handlers/EventHandler.hpp"
#include "Handlers/KernelHandler.hpp"
#include "Handlers/OutboundHandler.hpp"
#include "Handlers/SubscriptionHandler.hpp"
#include "QvPluginBase.hpp"

namespace Qv2ray::components::plugins
{
    class PluginAPIHost;
    class PluginManagerCore;
} // namespace Qv2ray::components::plugins

namespace Qv2rayPlugin
{
    class Qv2rayGUIInterface;
    class Qv2rayInterface;

    ///
    /// \brief PluginInstance always points to the
    ///
    inline Qv2rayInterface *PluginInstance;

    ///
    /// \brief The Qv2rayInterface class is the main entry for every Qv2ray plugins.
    ///
    class Qv2rayInterface
    {
        friend class PluginOutboundHandler;
        friend class PluginKernel;
        friend class PluginKernelInterface;
        friend class PluginEventHandler;
        friend class Qv2rayGUIInterface;
        friend class SubscriptionDecoder;
        friend class SubscriptionInterface;
        friend class Qv2ray::components::plugins::PluginAPIHost;
        friend class Qv2ray::components::plugins::PluginManagerCore;

      public:
        /// \internal
        const int QvPluginInterfaceVersion = QV2RAY_PLUGIN_INTERFACE_VERSION;
        virtual ~Qv2rayInterface() = default;

        ///
        /// \brief GetMetadata gets metadata of a plugin
        /// \return A QvPluginMetadata structure containing plugin information
        ///
        virtual const QvPluginMetadata GetMetadata() const = 0;

        ///
        /// \brief InitializePlugin should be reimplemented by the plugin writer, this is called only once when
        /// a plugin is found and loaded after a checking for interface version.
        /// A plugin should initialize its outboundHandler, eventHandler, kernelInterface, subscriptionInterface accordingly.
        /// In case of a GUI plugin, the guiInterface should also be initialized.
        /// \return a boolean value indicating if the initialization succeeds.
        ///
        virtual bool InitializePlugin() = 0;

        virtual std::shared_ptr<PluginOutboundHandler> OutboundHandler() const final
        {
            return m_OutboundHandler;
        }
        virtual std::shared_ptr<PluginEventHandler> EventHandler() const final
        {
            return m_EventHandler;
        }
        virtual std::shared_ptr<PluginKernelInterface> KernelInterface() const final
        {
            return m_KernelInterface;
        }
        virtual std::shared_ptr<SubscriptionInterface> SubscriptionAdapter() const final
        {
            return m_SubscriptionInterface;
        }
        virtual Qv2rayGUIInterface *GetGUIInterface() const final
        {
            return m_GUIInterface;
        }
        virtual QJsonObject GetSettings() const final
        {
            return m_Settings;
        }
        virtual connections::IConnectionManager *ConnectionManager() const final
        {
            return m_ConnectionManager;
        }

        ///
        /// \brief A signal that'll be connected to Qv2ray to provide logging function
        ///
        virtual void PluginLog(QString) const = 0;

        ///
        /// \brief PluginErrorMessageBox shows an error messagebox to the user with title and message.
        /// \param title The title of that messagebox
        /// \param message The content of message
        ///
        virtual void PluginErrorMessageBox(QString title, QString message) const = 0;

        ///
        /// \brief SettingsUpdated will be called by Qv2ray once the plugin setting is updated.
        ///
        virtual void SettingsUpdated() = 0;

      protected:
        explicit Qv2rayInterface()
        {
            PluginInstance = this;
        }

        QJsonObject m_Settings;
        QString m_WorkingDirectory;

        std::shared_ptr<PluginOutboundHandler> m_OutboundHandler;
        std::shared_ptr<PluginEventHandler> m_EventHandler;
        std::shared_ptr<PluginKernelInterface> m_KernelInterface;
        std::shared_ptr<SubscriptionInterface> m_SubscriptionInterface;

        // Not defined as a shared_ptr since not all plugins need QtGui
        Qv2rayGUIInterface *m_GUIInterface;

      private:
        connections::IConnectionManager *m_ConnectionManager;
    };
} // namespace Qv2rayPlugin

QT_BEGIN_NAMESPACE
#define Qv2rayInterface_IID "com.github.Qv2ray.Qv2rayPluginInterface"
Q_DECLARE_INTERFACE(Qv2rayPlugin::Qv2rayInterface, Qv2rayInterface_IID)
QT_END_NAMESPACE
