#pragma once

#include "sourcetrailpluginsettingspage.h"

#include <coreplugin/actionmanager/command.h>
#include <extensionsystem/iplugin.h>

class QTcpServer;
class StatusBarWidget;
class QAction;

namespace Sourcetrail
{

class SourcetrailPlugin : public ExtensionSystem::IPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Sourcetrail.json")

public:
	SourcetrailPlugin();
	~SourcetrailPlugin() override;

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
	static void setCursor(const Utils::FilePath &file, int line, int column);

	QTcpServer *m_server;
	SourcetrailPluginSettingsPage *m_page;
	SourcetrailPluginSettings m_settings;
	StatusBarWidget *m_statusBar;
	Core::Command *m_statusCommand;
};

} // namespace Sourcetrail
