#include "sourcetrailpluginsettings.h"

#include <QSettings>

static const char settingsGroup[]      = "Sourcetrail";
static const char addressKey[]         = "Address";
static const char pluginPortKey[]      = "PluginPort";
static const char sourcetrailPortKey[] = "SourcetrailPort";

namespace Sourcetrail {

void SourcetrailPluginSettings::fromSettings(Utils::QtcSettings *s)
{
    // assign defaults
    *this = SourcetrailPluginSettings();

    s->beginGroup(settingsGroup);
    m_hostAddress = s->value(addressKey, m_hostAddress).toString();
    m_pluginPort = s->value(pluginPortKey, m_pluginPort).toInt();
    m_sourcetrailPort = s->value(sourcetrailPortKey, m_sourcetrailPort).toInt();
    s->endGroup();
}

void SourcetrailPluginSettings::toSettings(Utils::QtcSettings *s) const
{
    s->beginGroup(settingsGroup);
    s->setValue(addressKey, m_hostAddress);
    s->setValue(pluginPortKey, m_pluginPort);
    s->setValue(sourcetrailPortKey, m_sourcetrailPort);
    s->endGroup();
}

bool SourcetrailPluginSettings::equals(const SourcetrailPluginSettings &stps) const
{
    return  m_hostAddress == stps.m_hostAddress
            && m_pluginPort == stps.m_pluginPort
            && m_sourcetrailPort == stps.m_sourcetrailPort;
}

} // namespace Sourcetrail
