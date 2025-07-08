#pragma once

#include "sourcetrailpluginsettingspage.h"

#include <coreplugin/actionmanager/command.h>
#include <extensionsystem/iplugin.h>

#include <QTcpServer>

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
	bool delayedInitialize() override;

public slots:
	void restartServer();
	void stopServer();

private:
	void handleMessage(QString message);
	void setStatus(const QString &message);
	void sendLocation();
	void sendPing();
	void sendEditorLocation();
	void startListening();
	void stopListening();
	void sendMessage(const QString &message);
	static void setCursor(const Utils::FilePath &file, int line, int column);

	QTcpServer m_server;
	SourcetrailPluginSettingsPage *m_page;
	SourcetrailPluginSettings m_settings;
	QAction *m_statusAction;
};

} // namespace Sourcetrail
