#pragma once

#include "QvPlugin/Common/QvPluginBase.hpp"

#include <QDialog>
#include <QJsonObject>
#include <QList>
#include <QMenu>
#include <QWidget>
#include <utility>

class InboundEditor;
class OutboundEditor;
class PluginManageWindow;

namespace Qv2rayPlugin::Gui
{
    struct ProtocolInfoObject
    {
        QString Protocol;
        QString DisplayName;
    };

#ifdef _QVPLUGIN_NG
    struct ProfileContentEditorInfo
    {
        QString Name;
        QString Description;
    };
#endif

    class PluginSettingsWidget : public QWidget
    {
      public:
        explicit PluginSettingsWidget(QWidget *parent) : QWidget(parent){};
        virtual ~PluginSettingsWidget() override = default;

        virtual void Load() = 0;
        virtual void Store() = 0;

      protected:
        friend class ::PluginManageWindow;
        QJsonObject settings;
    };

    class PluginMainWindowWidget : public QDialog
    {
      public:
        explicit PluginMainWindowWidget(QWidget *parent = nullptr) : QDialog(parent){};
        virtual ~PluginMainWindowWidget() override = default;
    };

    class PluginProtocolEditor : public QWidget
    {
      public:
        explicit PluginProtocolEditor(QWidget *parent = nullptr) : QWidget(parent){};
        virtual ~PluginProtocolEditor() override = default;

        virtual void Load() = 0;
        virtual void Store() = 0;

      protected:
        friend class ::InboundEditor;
        friend class ::OutboundEditor;
        IOProtocolSettings settings;
    };

#ifdef _QVPLUGIN_NG
    class PluginProfileEditor : public QWidget
    {
      public:
        explicit PluginProfileEditor(QWidget *parent = nullptr) : QWidget(parent){};
        virtual ~PluginProfileEditor() override = default;

        virtual void Load() = 0;
        virtual void Store() = 0;

      protected:
        ProfileContent content;
    };
#endif

    class Qv2rayGUIInterface
    {
      public:
#ifdef _QVPLUGIN_NG
        typedef QList<QPair<Qv2rayPlugin::Gui::ProfileContentEditorInfo, Qv2rayPlugin::Gui::PluginProfileEditor *>> ProfileEditorDescriptor;
#endif
        typedef QList<QPair<Qv2rayPlugin::Gui::ProtocolInfoObject, Qv2rayPlugin::Gui::PluginProtocolEditor *>> PluginEditorDescriptor;

        template<typename T>
        static inline QPair<ProtocolInfoObject, T *> make_editor_info(const QString &protocol, const QString &displayName)
        {
            return qMakePair(ProtocolInfoObject{ protocol, displayName }, new T());
        }

      public:
        explicit Qv2rayGUIInterface() = default;
        virtual ~Qv2rayGUIInterface() = default;

        virtual QIcon Icon() const = 0;
        virtual QList<Qv2rayPlugin::PLUGIN_GUI_COMPONENT_TYPE> GetComponents() const = 0;
        virtual std::unique_ptr<PluginSettingsWidget> GetSettingsWidget() const = 0;
        virtual PluginEditorDescriptor GetInboundEditors() const = 0;
        virtual PluginEditorDescriptor GetOutboundEditors() const = 0;
        virtual std::unique_ptr<PluginMainWindowWidget> GetMainWindowWidget() const = 0;
#ifdef _QVPLUGIN_NG
        virtual ProfileEditorDescriptor GetProfileEditors() const = 0;
#endif
    };

} // namespace Qv2rayPlugin::Gui
