#pragma once

#include "sourcetrailpluginsettings.h"
#include "ui_sourcetrailpluginsettingspage.h"

#include <coreplugin/dialogs/ioptionspage.h>

#include <QPointer>

namespace Sourcetrail
{

class SourcetrailPluginSettingsPage : public QObject, public Core::IOptionsPage
{
	Q_OBJECT

public:
	SourcetrailPluginSettingsPage(QObject *parent);
	~SourcetrailPluginSettingsPage() override;

	// IOptionsPage
	QWidget *widget() override;
	void apply() override;
	void finish() override;

signals:
	void SourcetrailPluginSettingsChanged(const Sourcetrail::SourcetrailPluginSettings &);

private:
	void settingsFromUi(SourcetrailPluginSettings &sourcetrail) const;

	Ui::SourcetrailPluginSettingsPage *m_page = nullptr;
	QPointer<QWidget> m_widget;
	SourcetrailPluginSettings m_settings;
};

} // namespace Sourcetrail
