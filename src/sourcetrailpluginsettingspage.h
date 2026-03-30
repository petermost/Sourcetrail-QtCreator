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

signals:
	void settingsChanged(const Sourcetrail::SourcetrailPluginSettings &);

private:
};



class SourcetrailPluginSettingsPageWidget : public Core::IOptionsPageWidget
{
	Q_OBJECT

public:
	SourcetrailPluginSettingsPageWidget();

	void apply() final;
	void cancel() final;
    bool isDirty() const final;

signals:
	void settingsChanged(const Sourcetrail::SourcetrailPluginSettings &);

private:
	void setUiSettings(const SourcetrailPluginSettings &settings);
	SourcetrailPluginSettings getUiSettings() const;

	std::unique_ptr<Ui::SourcetrailPluginSettingsPage> m_page;
	SourcetrailPluginSettings m_settings;
};

} // namespace Sourcetrail
