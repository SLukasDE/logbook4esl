#ifndef LOGBOOK4ESL_CONFIG_LEVELSETTING_H_
#define LOGBOOK4ESL_CONFIG_LEVELSETTING_H_

#include <common4esl/config/Config.h>

#include <esl/monitoring/Streams.h>

#include <tinyxml2.h>

#include <string>
#include <ostream>

namespace logbook4esl {
inline namespace v1_6 {
namespace config {

class LevelSetting : public common4esl::config::Config {
public:
	LevelSetting(const std::string& fileName, const tinyxml2::XMLElement& element);

	esl::monitoring::Streams::Level getLevel() const;
	const std::string& getScope() const;

	void save(std::ostream& oStream, std::size_t spaces) const;

private:
	esl::monitoring::Streams::Level level = esl::monitoring::Streams::Level::silent;
	std::string scope;
};

} /* namespace config */
} /* inline namespace v1_6 */
} /* namespace logbook4esl */

#endif /* LOGBOOK4ESL_CONFIG_LEVELSETTING_H_ */
