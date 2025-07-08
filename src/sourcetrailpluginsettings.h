#pragma once

#include <utils/qtcsettings.h>

#include <QObject>
#include <QString>

namespace Sourcetrail
{

class SourcetrailPluginSettings
{
public:
	void toSettings(Utils::QtcSettings *s) const;
	void fromSettings(Utils::QtcSettings *s);

	bool equals(const SourcetrailPluginSettings &bs) const;

	QString m_hostAddress = "localhost";
	int m_sourcetrailPort = 6666;
	int m_pluginPort = 6667;
};

inline bool operator==(const SourcetrailPluginSettings &t1, const SourcetrailPluginSettings &t2)
{
	return t1.equals(t2);
}
inline bool operator!=(const SourcetrailPluginSettings &t1, const SourcetrailPluginSettings &t2)
{
	return !t1.equals(t2);
}

} // namespace Sourcetrail
