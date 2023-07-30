#include <logbook4esl/config/LevelSetting.h>

#include <common4esl/config/FilePosition.h>

#include <esl/utility/String.h>

namespace logbook4esl {
inline namespace v1_6 {
namespace config {

LevelSetting::LevelSetting(const std::string& fileName, const tinyxml2::XMLElement& element)
: Config(fileName, element)
{
	if(element.GetUserData() != nullptr) {
		throw common4esl::config::FilePosition::add(*this, "Element has user data but it should be empty");
	}

	bool levelDefined = false;

	for(const tinyxml2::XMLAttribute* attribute = element.FirstAttribute(); attribute != nullptr; attribute = attribute->Next()) {
		if(std::string(attribute->Name()) == "scope") {
			if(scope != "") {
				throw common4esl::config::FilePosition::add(*this, "Multiple definition of attribute 'scope'");
			}
			scope = attribute->Value();
			if(scope == "") {
				throw common4esl::config::FilePosition::add(*this, "Value \"\" of attribute 'scope' is invalid.");
			}
		}
		else if(std::string(attribute->Name()) == "level") {
			if(levelDefined) {
				throw common4esl::config::FilePosition::add(*this, "Multiple definition of attribute 'level'");
			}
			std::string levelStr = esl::utility::String::toUpper(attribute->Value());

			if(levelStr == "SILENT") {
				level = esl::logging::Level::SILENT;
			}
			else if(levelStr == "ERROR") {
				level = esl::logging::Level::ERROR;
			}
			else if(levelStr == "WARN") {
				level = esl::logging::Level::WARN;
			}
			else if(levelStr == "INFO") {
				level = esl::logging::Level::INFO;
			}
			else if(levelStr == "DEBUG") {
				level = esl::logging::Level::DEBUG;
			}
			else if(levelStr == "TRACE") {
				level = esl::logging::Level::TRACE;
			}
			else {
				throw common4esl::config::FilePosition::add(*this, "Value \"" + levelStr + "\" of attribute 'level' is invalid. "
						"Valid values are \"SILENT\", \"ERROR\", \"WARN\", \"INFO\", \"DEBUG\" and \"TRACE\"");
			}
			levelDefined = true;
		}
		else {
			throw common4esl::config::FilePosition::add(*this, "Unknown attribute '" + std::string(attribute->Name()) + "'");
		}
	}

	if(scope == "") {
		throw common4esl::config::FilePosition::add(*this, "Missing attribute 'scope'");
	}
	if(!levelDefined) {
		throw common4esl::config::FilePosition::add(*this, "Missing attribute 'level'");
	}
}

esl::logging::Level LevelSetting::getLevel() const {
	return level;
}

const std::string& LevelSetting::getScope() const {
	return scope;
}

void LevelSetting::save(std::ostream& oStream, std::size_t spaces) const {
	switch(level) {
	case esl::logging::Level::SILENT:
		oStream << makeSpaces(spaces) << "<setting scope=\"" << scope << "\" level=\"silent\"/>\n";
		break;
	case esl::logging::Level::ERROR:
		oStream << makeSpaces(spaces) << "<setting scope=\"" << scope << "\" level=\"error\"/>\n";
		break;
	case esl::logging::Level::WARN:
		oStream << makeSpaces(spaces) << "<setting scope=\"" << scope << "\" level=\"warn\"/>\n";
		break;
	case esl::logging::Level::INFO:
		oStream << makeSpaces(spaces) << "<setting scope=\"" << scope << "\" level=\"info\"/>\n";
		break;
	case esl::logging::Level::DEBUG:
		oStream << makeSpaces(spaces) << "<setting scope=\"" << scope << "\" level=\"debug\"/>\n";
		break;
	case esl::logging::Level::TRACE:
		oStream << makeSpaces(spaces) << "<setting scope=\"" << scope << "\" level=\"trace\"/>\n";
		break;
	}
}

} /* namespace config */
} /* inline namespace v1_6 */
} /* namespace logbook4esl */
