#include <logbook4esl/config/Layout.h>
#include <common4esl/config/FilePosition.h>

#include <esl/plugin/Registry.h>

#include <utility>

namespace logbook4esl {
inline namespace v1_6 {
namespace config {

Layout::Layout(const std::string& fileName, const tinyxml2::XMLElement& element)
: common4esl::config::Config(fileName, element)
{
	if(element.GetUserData() != nullptr) {
		throw common4esl::config::FilePosition::add(*this, "Element has user data but it should be empty");
	}

	for(const tinyxml2::XMLAttribute* attribute = element.FirstAttribute(); attribute != nullptr; attribute = attribute->Next()) {
		if(std::string(attribute->Name()) == "id") {
			if(id != "") {
				throw common4esl::config::FilePosition::add(*this, "Multiple definition of attribute 'id'");
			}
			id = attribute->Value();
			if(id == "") {
				throw common4esl::config::FilePosition::add(*this, "Invalid value \"\" for attribute 'id'");
			}
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
		else {
			throw common4esl::config::FilePosition::add(*this, "Unknown attribute '" + std::string(attribute->Name()) + "'");
		}
	}

	if(id == "") {
		throw common4esl::config::FilePosition::add(*this, "Missing attribute 'id'");
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

const std::string& Layout::getId() const noexcept {
	return id;
}

void Layout::save(std::ostream& oStream, std::size_t spaces) const {
	oStream << makeSpaces(spaces) << "<layout id=\"" << id << "\" implementation=\"" << implementation << "\">\n";
	for(const auto& parameter : parameters) {
		parameter.saveParameter(oStream, spaces+2);
	}
	oStream << makeSpaces(spaces) << "</layout>\n";
}

std::unique_ptr<esl::logging::Layout> Layout::create() const {
	std::vector<std::pair<std::string, std::string>> eslSettings;

	for(auto const& setting : parameters) {
		eslSettings.push_back(std::make_pair(setting.key, setting.value));
	}

	return esl::plugin::Registry::get().create<esl::logging::Layout>(implementation, eslSettings);
}

void Layout::parseInnerElement(const tinyxml2::XMLElement& element) {
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
} /* inline namespace v1_6 */
} /* namespace logbook4esl */
