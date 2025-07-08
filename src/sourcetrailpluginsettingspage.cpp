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

	Utils::QtcSettings *s = Core::ICore::settings();
	m_settings.fromSettings(s);
}

QWidget *SourcetrailPluginSettingsPage::widget()
{
	if (m_widget == nullptr) {
		m_widget = std::make_unique<QWidget>();
		m_page = std::make_unique<Ui::SourcetrailPluginSettingsPage>();
		m_page->setupUi(m_widget.get());

		m_page->hostaddress->setText(m_settings.m_hostAddress);
		m_page->pluginport->setText(QString::number(m_settings.m_pluginPort));
		m_page->sourcetrailport->setText(QString::number(m_settings.m_sourcetrailPort));
	}
	return m_widget.get();
}

void SourcetrailPluginSettingsPage::apply()
{
	if (m_page == nullptr) // page was never shown
		return;

	SourcetrailPluginSettings setting;
	settingsFromUi(setting);

	if (m_settings != setting) {
		m_settings = setting;
		Utils::QtcSettings *s = Core::ICore::settings();
		m_settings.toSettings(s);
		emit SourcetrailPluginSettingsChanged(setting);
	}
}

void SourcetrailPluginSettingsPage::finish()
{
	m_widget.reset();
	m_page.reset();
}

void SourcetrailPluginSettingsPage::settingsFromUi(SourcetrailPluginSettings &settings) const
{
	if (m_page == nullptr)
		return;

	settings.m_hostAddress = m_page->hostaddress->text();
	settings.m_pluginPort = m_page->pluginport->text().toInt();
	settings.m_sourcetrailPort = m_page->sourcetrailport->text().toInt();
}

} // namespace Sourcetrail
