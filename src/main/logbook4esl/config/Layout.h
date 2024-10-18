#ifndef LOGBOOK4ESL_CONFIG_LAYOUT_H_
#define LOGBOOK4ESL_CONFIG_LAYOUT_H_

#include <common4esl/config/Config.h>
#include <common4esl/config/Setting.h>

#include <esl/monitoring/Layout.h>

#include <tinyxml2.h>

#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace logbook4esl {
inline namespace v1_6 {
namespace config {

class Layout : public common4esl::config::Config {
public:
	Layout(const std::string& fileName, const tinyxml2::XMLElement& element);

	const std::string& getId() const noexcept;

	void save(std::ostream& oStream, std::size_t spaces) const;
	std::unique_ptr<esl::monitoring::Layout> create() const;

private:
	std::string id;
	std::string implementation;
	std::vector<common4esl::config::Setting> parameters;

	void parseInnerElement(const tinyxml2::XMLElement& element);
};

} /* namespace config */
} /* inline namespace v1_6 */
} /* namespace logbook4esl */

#endif /* LOGBOOK4ESL_CONFIG_LAYOUT_H_ */
