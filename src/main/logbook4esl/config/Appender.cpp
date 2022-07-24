#include <logbook4esl/config/Appender.h>
#include <common4esl/config/FilePosition.h>

#include <esl/plugin/exception/PluginNotFound.h>
#include <esl/plugin/Registry.h>
#include <esl/utility/String.h>

#include <iostream>
#include <stdexcept>
#include <utility>

namespace logbook4esl {
namespace config {

Appender::Appender(const std::string& fileName, const tinyxml2::XMLElement& element)
: common4esl::config::Config(fileName, element)
{
	if(element.GetUserData() != nullptr) {
		throw common4esl::config::FilePosition::add(*this, "Element has user data but it should be empty");
	}

	bool recordDefined = false;

	for(const tinyxml2::XMLAttribute* attribute = element.FirstAttribute(); attribute != nullptr; attribute = attribute->Next()) {
		if(std::string(attribute->Name()) == "name") {
			if(!name.empty()) {
				throw common4esl::config::FilePosition::add(*this, "Multiple definition of attribute 'name'");
			}
			name = esl::utility::String::toLower(attribute->Value());
			if(name.empty()) {
				throw common4esl::config::FilePosition::add(*this, "Value \"\" of attribute 'name' is invalid.");
			}
		}
		else if(std::string(attribute->Name()) == "record") {
			if(recordDefined) {
				throw common4esl::config::FilePosition::add(*this, "Multiple definition of attribute 'record'");
			}
			std::string recordLevelStr = esl::utility::String::toUpper(attribute->Value());

			if(recordLevelStr == "ALL") {
				recordLevel = esl::logging::Appender::RecordLevel::ALL;
			}
			else if(recordLevelStr == "SELECTED") {
				recordLevel = esl::logging::Appender::RecordLevel::SELECTED;
			}
			else if(recordLevelStr == "OFF") {
				recordLevel = esl::logging::Appender::RecordLevel::OFF;
			}
			else {
				throw common4esl::config::FilePosition::add(*this, "Value \"" + std::string(attribute->Value()) + "\" of attribute 'record' is invalid. "
						"Valid values are \"all\", \"selected\" and \"off\"");
			}
			recordDefined = true;
		}
		else if(std::string(attribute->Name()) == "implementation") {
			if(implementation != "") {
				throw common4esl::config::FilePosition::add(*this, "Multiple definition of attribute 'implementation'");
			}
			implementation = attribute->Value();
			if(implementation == "") {
				throw common4esl::config::FilePosition::add(*this, "Invalid value \"\" for attribute 'implementation'");
			}
		}
		else if(std::string(attribute->Name()) == "layout") {
			if(layoutId != "") {
				throw common4esl::config::FilePosition::add(*this, "Multiple definition of attribute 'layout'");
			}
			layoutId = attribute->Value();
			if(layoutId == "") {
				throw common4esl::config::FilePosition::add(*this, "Invalid value \"\" for attribute 'layout'");
			}
		}
		else {
			throw common4esl::config::FilePosition::add(*this, "Unknown attribute '" + std::string(attribute->Name()) + "'");
		}
	}

	if(layoutId == "") {
		throw common4esl::config::FilePosition::add(*this, "Missing attribute 'layout'");
	}
	if(implementation == "") {
		throw common4esl::config::FilePosition::add(*this, "Missing attribute 'implementation'");
	}

	for(const tinyxml2::XMLNode* node = element.FirstChild(); node != nullptr; node = node->NextSibling()) {
		const tinyxml2::XMLElement* innerElement = node->ToElement();

		if(innerElement == nullptr) {
			continue;
		}

		parseInnerElement(*innerElement);
	}
}

const std::string& Appender::getName() const {
	return name;
}

const std::string& Appender::getLayoutId() const {
	return layoutId;
}

void Appender::save(std::ostream& oStream, std::size_t spaces) const {
	oStream << makeSpaces(spaces) << "<appender";

	if(!name.empty()) {
		oStream << " name=\"" << name << "\"";
	}

	if(!implementation.empty()) {
		oStream << " implementation=\"" << implementation << "\"";
	}

	switch(recordLevel) {
	case esl::logging::Appender::RecordLevel::ALL:
		oStream << " record=\"ALL\"";
		break;
	case esl::logging::Appender::RecordLevel::SELECTED:
		oStream << " record=\"SELECTED\"";
		break;
	case esl::logging::Appender::RecordLevel::OFF:
		oStream << " record=\"OFF\"";
		break;
	}

	oStream << " layout=\"" << layoutId << "\"";

	if(parameters.empty()) {
		oStream << "/>\n";
	}
	else {
		oStream << ">\n";
		for(const auto& parameter : parameters) {
			parameter.saveParameter(oStream, spaces+2);
		}
		oStream << makeSpaces(spaces) << "</appender>\n";
	}
}

std::unique_ptr<esl::logging::Appender> Appender::create() const {
	std::vector<std::pair<std::string, std::string>> eslSettings;
	for(auto const& setting : parameters) {
		eslSettings.push_back(std::make_pair(setting.key, setting.value));
	}

	std::unique_ptr<esl::logging::Appender> appender;
	try {
		appender = esl::plugin::Registry::get().create<esl::logging::Appender>(implementation, eslSettings);
	}
	catch(const esl::plugin::exception::PluginNotFound& e) {
		throw common4esl::config::FilePosition::add(*this, e);
	}
	catch(const std::runtime_error& e) {
		throw common4esl::config::FilePosition::add(*this, e);
	}
	catch(const std::exception& e) {
		throw common4esl::config::FilePosition::add(*this, e);
	}
	catch(...) {
		throw common4esl::config::FilePosition::add(*this, "Could not create logging appender for implementation '" + implementation + "' because an unknown exception occurred.");
	}

	if(!appender) {
		throw common4esl::config::FilePosition::add(*this, "Could not create logging appender for implementation '" + implementation + "' because interface method create() returns nullptr.");
	}
	appender->setRecordLevel(recordLevel);

	return appender;
}

void Appender::parseInnerElement(const tinyxml2::XMLElement& element) {
	if(element.Name() == nullptr) {
		throw common4esl::config::FilePosition::add(*this, "Element name is empty");
	}

	std::string elementName(element.Name());

	if(elementName == "parameter") {
		parameters.push_back(common4esl::config::Setting(getFileName(), element, false));
	}
	else {
		throw common4esl::config::FilePosition::add(*this, "Unknown element name \"" + elementName + "\"");
	}
}

} /* namespace config */
} /* namespace common4esl */
