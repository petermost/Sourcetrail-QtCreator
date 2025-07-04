#pragma once

#include "sourcetrail_global.h"
#include "sourcetrailpluginsettingspage.h"

#include <extensionsystem/iplugin.h>

class QTcpServer;
class StatusBarWidget;
class QAction;

namespace Core { class Command; }

namespace Sourcetrail {

class SourcetrailPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Sourcetrail.json")

public:
    SourcetrailPlugin();
    ~SourcetrailPlugin();

    void initialize() override;
    void extensionsInitialized() override;
    bool delayedInitialize() override;
    ShutdownFlag aboutToShutdown() override;

public slots:
    void restartServer();
    void stopServer();

private:
    void handleMessage(QString message);
    void triggerAction();
    void sendPing();
    void sendLocation();
    void startListening();
    void stopListening();
    void sendMessage(QString message);
    void setCursor(const Utils::FilePath &file, int line, int column);

    QTcpServer *m_server;
    SourcetrailPluginSettingsPage *m_page;
    SourcetrailPluginSettings m_settings;
    StatusBarWidget *m_statusBar;
    Core::Command *m_statusCommand;
};

} // namespace Sourcetrail
