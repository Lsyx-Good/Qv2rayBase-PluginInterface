#pragma once

#include "QvPlugin/Common/QvPluginBase.hpp"
#include "QvPlugin/Connections/ConnectionsBase.hpp"
#include "QvPlugin/Handlers/EventHandler.hpp"
#include "QvPlugin/Handlers/IProfilePreprocessor.hpp"
#include "QvPlugin/Handlers/KernelHandler.hpp"
#include "QvPlugin/Handlers/LatencyTestHandler.hpp"
#include "QvPlugin/Handlers/OutboundHandler.hpp"
#include "QvPlugin/Handlers/SubscriptionHandler.hpp"

#include <QDir>

namespace Qv2rayBase::Plugin
{
    class PluginManagerCore;
} // namespace Qv2rayBase::Plugin

#define Qv2rayInterface_IID "com.github.Qv2ray.Qv2rayPluginInterface"

namespace Qv2rayPlugin
{
    using namespace Qv2rayPlugin::Outbound;
    using namespace Qv2rayPlugin::Kernel;
    using namespace Qv2rayPlugin::Event;
    using namespace Qv2rayPlugin::Subscription;
    using namespace Qv2rayPlugin::Latency;

    namespace Gui
    {
        class PluginGUIInterface;
    }
    class Qv2rayInterface;

    ///
    /// \brief PluginInstance always points to the instance of current plugin.
    ///
    inline Qv2rayInterface *PluginInstance;

    ///
    /// \brief TPluginInstance always points to the instance of current plugin, with casts to the type of given plugin.
    /// Cast to a wrong type will result in an undefined behavior.
    ///
    template<typename T>
    inline T *TPluginInstance()
    {
        return static_cast<T *>(PluginInstance);
    }

    ///
    /// \brief The Qv2rayInterface class is the main entry for every Qv2ray plugins.
    ///
    class Qv2rayInterface
    {
        friend class Qv2rayBase::Plugin::PluginManagerCore;

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

        virtual std::shared_ptr<Qv2rayPlugin::Outbound::IOutboundProcessor> OutboundHandler() const final
        {
            return m_OutboundHandler;
        }
        virtual std::shared_ptr<Qv2rayPlugin::Event::IEventHandler> EventHandler() const final
        {
            return m_EventHandler;
        }
        virtual std::shared_ptr<Qv2rayPlugin::Kernel::IKernelHandler> KernelInterface() const final
        {
            return m_KernelInterface;
        }
        virtual std::shared_ptr<Qv2rayPlugin::Subscription::ISubscriptionHandler> SubscriptionAdapter() const final
        {
            return m_SubscriptionInterface;
        }
        virtual std::shared_ptr<Qv2rayPlugin::Latency::ILatencyHandler> LatencyTestHandler() const final
        {
            return m_LatencyTestHandler;
        }
        virtual std::shared_ptr<Qv2rayPlugin::Profile::IProfilePreprocessor> ProfilePreprocessor() const final
        {
            return m_ProfilePreprocessor;
        }
        virtual Gui::PluginGUIInterface *GetGUIInterface() const final
        {
            return m_GUIInterface;
        }
        virtual QJsonObject GetSettings() const final
        {
            return m_Settings;
        }
        virtual Qv2rayPlugin::Connections::IProfileManager *ProfileManager() const final
        {
            return m_ProfileManager;
        }

        ///
        /// \brief A signal that'll be connected to Qv2ray to provide logging function
        ///
        virtual void PluginLog(QString) = 0;

        ///
        /// \brief PluginErrorMessageBox shows an error messagebox to the user with title and message.
        /// \param title The title of that messagebox
        /// \param message The content of message
        ///
        virtual void PluginErrorMessageBox(QString title, QString message) = 0;

        ///
        /// \brief SettingsUpdated will be called by Qv2ray once the plugin setting is updated.
        ///
        virtual void SettingsUpdated() = 0;

        QDir WorkingDirectory()
        {
            return m_WorkingDirectory;
        }

      protected:
        explicit Qv2rayInterface()
        {
            PluginInstance = this;
        }

        QJsonObject m_Settings;
        QDir m_WorkingDirectory;

        std::shared_ptr<Qv2rayPlugin::Profile::IProfilePreprocessor> m_ProfilePreprocessor;
        std::shared_ptr<Qv2rayPlugin::Outbound::IOutboundProcessor> m_OutboundHandler;
        std::shared_ptr<Qv2rayPlugin::Event::IEventHandler> m_EventHandler;
        std::shared_ptr<Qv2rayPlugin::Kernel::IKernelHandler> m_KernelInterface;
        std::shared_ptr<Qv2rayPlugin::Subscription::ISubscriptionHandler> m_SubscriptionInterface;
        std::shared_ptr<Qv2rayPlugin::Latency::ILatencyHandler> m_LatencyTestHandler;

        // Not defined as a shared_ptr since not all plugins need QtGui
        Gui::PluginGUIInterface *m_GUIInterface;

      private:
        Qv2rayPlugin::Connections::IProfileManager *m_ProfileManager;
    };
} // namespace Qv2rayPlugin

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Qv2rayPlugin::Qv2rayInterface, Qv2rayInterface_IID)
QT_END_NAMESPACE

#define QvPluginLog(msg) ::Qv2rayPlugin::PluginInstance->PluginLog(msg)
#define QvPluginMessageBox(title, msg) ::Qv2rayPlugin::PluginInstance->PluginErrorMessageBox(title, msg)
