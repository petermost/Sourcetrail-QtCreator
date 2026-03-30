#include "sourcetrailpluginsettingspage.h"

#include "sourcetrailconstants.h"

#include <coreplugin/icore.h>

namespace Sourcetrail
{

SourcetrailPluginSettingsPage::SourcetrailPluginSettingsPage(QObject *parent)
	: QObject(parent)
{
	setId(Constants::CATEGORY_ID);
	setDisplayName(tr(Constants::CATEGORY));
	setCategory(Utils::Id(Constants::CATEGORY_ID));

	// If a widget creator is set, then a settings provider will be ignored!
	// See: https://github.com/qt-creator/qt-creator/blob/efebd42b50e1d1098f7679f2cd4a476c474f2312/src/plugins/coreplugin/dialogs/ioptionspage.cpp#L450

	setWidgetCreator([this]()
	{
		SourcetrailPluginSettingsPageWidget *widget = new SourcetrailPluginSettingsPageWidget();
		connect(widget, &SourcetrailPluginSettingsPageWidget::settingsChanged, this, &SourcetrailPluginSettingsPage::settingsChanged);

		return widget;
	});
}



SourcetrailPluginSettingsPageWidget::SourcetrailPluginSettingsPageWidget()
{
	m_page = std::make_unique<Ui::SourcetrailPluginSettingsPage>();
	m_page->setupUi(this);

	Utils::QtcSettings *s = Core::ICore::settings();
	m_settings.fromSettings(s);

	setUiSettings(m_settings);
}

void SourcetrailPluginSettingsPageWidget::apply()
{
	SourcetrailPluginSettings settings = getUiSettings();

	if (m_settings != settings)	{
		m_settings = settings;
		Utils::QtcSettings *s = Core::ICore::settings();
		m_settings.toSettings(s);

		emit settingsChanged(settings);
	}
}

void SourcetrailPluginSettingsPageWidget::cancel()
{
	setUiSettings(m_settings);
}

bool SourcetrailPluginSettingsPageWidget::isDirty() const
{
	return m_settings != getUiSettings();
}

void SourcetrailPluginSettingsPageWidget::setUiSettings(const SourcetrailPluginSettings &settings)
{
	m_page->hostaddress->setText(settings.m_hostAddress);
	m_page->pluginport->setText(QString::number(settings.m_pluginPort));
	m_page->sourcetrailport->setText(QString::number(settings.m_sourcetrailPort));
}

SourcetrailPluginSettings SourcetrailPluginSettingsPageWidget::getUiSettings() const
{
	SourcetrailPluginSettings settings {
		.m_hostAddress = m_page->hostaddress->text(),
		.m_sourcetrailPort = m_page->sourcetrailport->text().toInt(),
		.m_pluginPort = m_page->pluginport->text().toInt()
	};
	return settings;
}

} // namespace Sourcetrail
