#ifndef LOGBOOK4ESL_CONFIG_LEVELSETTING_H_
#define LOGBOOK4ESL_CONFIG_LEVELSETTING_H_

#include <common4esl/config/Config.h>

#include <esl/logging/Level.h>

#include <tinyxml2/tinyxml2.h>

#include <string>
#include <ostream>

namespace logbook4esl {
namespace config {

class LevelSetting : public common4esl::config::Config {
public:
	LevelSetting(const std::string& fileName, const tinyxml2::XMLElement& element);

	esl::logging::Level getLevel() const;
	const std::string& getScope() const;

	void save(std::ostream& oStream, std::size_t spaces) const;

private:
	esl::logging::Level level = esl::logging::Level::SILENT;
	std::string scope;
};

} /* namespace config */
} /* namespace logbook4esl */

#endif /* LOGBOOK4ESL_CONFIG_LEVELSETTING_H_ */
