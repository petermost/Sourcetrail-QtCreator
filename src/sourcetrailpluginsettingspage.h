#pragma once

#include "sourcetrailpluginsettings.h"
#include "ui_sourcetrailpluginsettingspage.h"

#include <coreplugin/dialogs/ioptionspage.h>

#include <memory>

namespace Sourcetrail
{

class SourcetrailPluginSettingsPage : public QObject, public Core::IOptionsPage
{
	Q_OBJECT

public:
	SourcetrailPluginSettingsPage(QObject *parent);

	// IOptionsPage
	QWidget *widget() override;
	void apply() override;
	void finish() override;

signals:
	void SourcetrailPluginSettingsChanged(const Sourcetrail::SourcetrailPluginSettings &);

private:
	void settingsFromUi(SourcetrailPluginSettings &sourcetrail) const;

	std::unique_ptr<QWidget> m_widget;
	std::unique_ptr<Ui::SourcetrailPluginSettingsPage> m_page;
	SourcetrailPluginSettings m_settings;
};

} // namespace Sourcetrail
