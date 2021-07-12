#pragma once

#include "QvPlugin/Common/QvPluginBase.hpp"

#include <QDialog>
#include <QJsonObject>
#include <QList>
#include <QMenu>
#include <QWidget>
#include <utility>

namespace Qv2rayPlugin::Gui
{
    struct ProtocolInfoObject
    {
        QString Protocol;
        QString DisplayName;
    };

    class PluginSettingsWidget : public QWidget
    {
      public:
        explicit PluginSettingsWidget(QWidget *parent) : QWidget(parent){};
        virtual ~PluginSettingsWidget() override = default;

        virtual void SetSettings(const QJsonObject &) = 0;
        virtual QJsonObject GetSettings() = 0;
    };

    class PluginMainWindowWidget : public QDialog
    {
      public:
        explicit PluginMainWindowWidget(QWidget *parent) : QDialog(parent){};
        virtual ~PluginMainWindowWidget() override = default;
    };

#define PLUGIN_EDITOR_LOADING_SCOPE(t)                                                                                                                                   \
    isLoading = true;                                                                                                                                                    \
    t;                                                                                                                                                                   \
    isLoading = false;

#define PLUGIN_EDITOR_LOADING_GUARD                                                                                                                                      \
    if (this->isLoading)                                                                                                                                                 \
        return;

    template<typename T>
    inline bool GetProperty(const T *widget, const char *name)
    {
        const auto prop = widget->property(name);
        return prop.isValid() && prop.toBool();
    }

    class QvPluginEditor : public QWidget
    {
      public:
        explicit QvPluginEditor(QWidget *parent = nullptr) : QWidget(parent){};
        virtual ~QvPluginEditor() = default;

        virtual void SetContent(const IOProtocolSettings &) = 0;
        virtual const IOProtocolSettings GetContent() const = 0;

      protected:
        IOProtocolSettings content;
        bool isLoading = false;
    };

    class PluginGUIInterface
    {
      public:
        template<typename T>
        static inline QPair<ProtocolInfoObject, T *> make_editor_info(const QString &protocol, const QString &displayName)
        {
            return { ProtocolInfoObject{ protocol, displayName }, new T() };
        }

        using typed_plugin_editor = QPair<ProtocolInfoObject, QvPluginEditor *>;

        explicit PluginGUIInterface() = default;
        virtual ~PluginGUIInterface() = default;

        virtual QIcon Icon() const = 0;
        virtual QList<Qv2rayPlugin::PLUGIN_GUI_COMPONENT_TYPE> GetComponents() const = 0;
        virtual std::unique_ptr<PluginSettingsWidget> GetSettingsWidget() const final
        {
            return createSettingsWidgets();
        }
        virtual QList<typed_plugin_editor> GetInboundEditors() const final
        {
            return createInboundEditors();
        }
        virtual QList<typed_plugin_editor> GetOutboundEditors() const final
        {
            return createOutboundEditors();
        }
        virtual std::unique_ptr<PluginMainWindowWidget> GetMainWindowWidget() const final
        {
            return createMainWindowWidget();
        }

      protected:
        virtual std::unique_ptr<PluginSettingsWidget> createSettingsWidgets() const = 0;
        virtual QList<typed_plugin_editor> createInboundEditors() const = 0;
        virtual QList<typed_plugin_editor> createOutboundEditors() const = 0;
        virtual std::unique_ptr<PluginMainWindowWidget> createMainWindowWidget() const = 0;
    };

} // namespace Qv2rayPlugin::Gui
