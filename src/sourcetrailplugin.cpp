#include "sourcetrailplugin.h"

#include "sourcetrailconstants.h"

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/icore.h>
#include <coreplugin/messagemanager.h>
#include <cppeditor/cppeditorconstants.h>
#include <texteditor/texteditor.h>

#include <QAction>
#include <QMenu>
#include <QTcpSocket>

using namespace Core;

namespace Sourcetrail
{

SourcetrailPlugin::SourcetrailPlugin()
{
	connect(&m_server, &QTcpServer::newConnection, this, [=]()
	{
		QTcpSocket *connection = m_server.nextPendingConnection();
		connect(connection, &QAbstractSocket::disconnected, connection, &QObject::deleteLater);

		connect(connection, &QAbstractSocket::readyRead, this, [=]()
		{
			handleMessage(QString::fromUtf8(connection->readAll()));
		});
	});
}

void SourcetrailPlugin::handleMessage(QString message)
{
	message = message.remove(message.indexOf("<EOM>"), 5);
	QStringList list = message.split(">>");

	if (list[0] == "ping") {
		sendPing();
	}
	if (list[0] == "moveCursor") {
		setCursor(Utils::FilePath::fromString(list[1]), list[2].toInt(), list[3].toInt());
	}
}

SourcetrailPlugin::~SourcetrailPlugin()
{
	stopListening();
}


void SourcetrailPlugin::initialize()
{
	// Register objects in the plugin manager's object pool
	// Load settings
	// Add actions to menus
	// Connect to other plugins' signals
	// In the initialize function, a plugin can be sure that the plugins it
	// depends on have initialized their members.

	IOptionsPage::registerCategory(Utils::Id(Constants::CATEGORY_ID), Constants::CATEGORY, Utils::FilePath(Constants::CATEGORY_ICON));

	m_page = new SourcetrailPluginSettingsPage(this);
	connect(m_page, &SourcetrailPluginSettingsPage::SourcetrailPluginSettingsChanged, this, &SourcetrailPlugin::restartServer);

	// Sourcetrail menu:

	Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
	menu->menu()->setTitle(tr("Sourcetrail"));
	menu->menu()->setIcon(QIcon(Constants::CATEGORY_ICON));

	// Send location:

	QAction *sendLocationAction = new QAction(QIcon(Constants::CATEGORY_ICON), tr("Send Location"), this);
	Core::Command *sendLocationCommand = Core::ActionManager::registerAction(sendLocationAction, Constants::SEND_LOCATION_ID, Core::Context(Core::Constants::C_GLOBAL));
	sendLocationCommand->setDefaultKeySequence(QKeySequence(tr("Alt+S,Alt+S")));
	menu->addAction(sendLocationCommand);
	connect(sendLocationAction, &QAction::triggered, this, &SourcetrailPlugin::sendLocation);

	// Restart server:

	QAction *restartAction = new QAction(tr("Restart Listener"), this);
	Core::Command *restartCommand = Core::ActionManager::registerAction(restartAction, Constants::RESTART_LISTENER_ID, Core::Context(Core::Constants::C_GLOBAL));
	menu->addAction(restartCommand);
	connect(restartAction, &QAction::triggered, this, &SourcetrailPlugin::restartServer);

	// Stop server:

	QAction *stopAction = new QAction(tr("Stop Listener"), this);
	Core::Command *stopCommand = Core::ActionManager::registerAction(stopAction, Constants::STOP_LISTENER_ID, Core::Context(Core::Constants::C_GLOBAL));
	menu->addAction(stopCommand);
	connect(stopAction, &QAction::triggered, this, &SourcetrailPlugin::stopServer);

	// Action to show status:
	// Use QMenu directly (without registerAction()) to prevent the action from showing up in 'Keyboard Shortcuts'.

	m_statusAction = new QAction("", this);
	m_statusAction->setEnabled(false);
	menu->menu()->addSeparator();
	menu->menu()->addAction(m_statusAction);

	Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

	// Editor Context Menu:

	if (ActionContainer *editorContextMenu = ActionManager::actionContainer(CppEditor::Constants::M_CONTEXT)) {
		editorContextMenu->addAction(sendLocationCommand);
	}
}

bool SourcetrailPlugin::delayedInitialize()
{
	restartServer();
	return true;
}

void SourcetrailPlugin::restartServer()
{
	Utils::QtcSettings *s = Core::ICore::settings();
	m_settings.fromSettings(s);

	stopListening();
	startListening();
}

void SourcetrailPlugin::stopServer()
{
	stopListening();
}

void SourcetrailPlugin::startListening()
{
	if (m_server.listen(QHostAddress::LocalHost, m_settings.m_sourcetrailPort)) {
		setStatus(tr("Running"));
	} else {
		setStatus(m_server.errorString());
		MessageManager::writeSilently(tr("Sourcetrail: Listening on port %1 failed because %2").arg(m_settings.m_sourcetrailPort).arg(m_server.errorString()));
	}
}

void SourcetrailPlugin::stopListening()
{
	if (m_server.isListening()) {
		m_server.close();
		setStatus(tr("Stopped"));
	}
}

void SourcetrailPlugin::setStatus(const QString &message)
{
	m_statusAction->setText(tr("Listener Status: %1").arg(message));
}

void SourcetrailPlugin::setCursor(const Utils::FilePath &file, int line, int column)
{
	Core::EditorManager::openEditorAt(Utils::Link(file, line, column));
}

void SourcetrailPlugin::sendLocation()
{
	sendPing();
	sendEditorLocation();

	Utils::FilePaths list;
	Core::ICore::openFiles(list);
}

void SourcetrailPlugin::sendPing()
{
	sendMessage("ping>>Qt Creator<EOM>");
}

void SourcetrailPlugin::sendEditorLocation()
{
	IEditor *editor = Core::EditorManager::currentEditor();
	if (editor != nullptr) {
		int line = Core::EditorManager::currentEditor()->currentLine();
		int column = Core::EditorManager::currentEditor()->currentColumn();
		QString file = Core::EditorManager::currentDocument()->filePath().toUserOutput();

		QString message = "setActiveToken>>" + file + ">>" + QString::number(line) + ">>" + QString::number(column) + "<EOM>";

		sendMessage(message);
	} else {
		MessageManager::writeSilently(tr("Sourcetrail: Sending location not possible, because there is no active editor"));
	}
}

void SourcetrailPlugin::sendMessage(const QString &message)
{
	QTcpSocket senderSocket;
	senderSocket.connectToHost(m_settings.m_hostAddress, m_settings.m_pluginPort);

	if (senderSocket.waitForConnected()) {
		senderSocket.write(message.toUtf8());
		senderSocket.flush();
		senderSocket.waitForBytesWritten();
		senderSocket.close();
	} else
		setStatus(senderSocket.errorString());
}

} // namespace Sourcetrail
