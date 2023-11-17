#include <logbook4esl/config/Logger.h>
#include <logbook4esl/monitoring/Logging.h>

#include <common4esl/config/FilePosition.h>

//#include <esl/monitoring/Logger.h>
#include <esl/monitoring/Logging.h>

namespace logbook4esl {
inline namespace v1_6 {
namespace config {

Logger::Logger(const std::string& configuration)
: common4esl::config::Config("{mem}")
{
	tinyxml2::XMLDocument xmlDocument;

	tinyxml2::XMLError xmlError = xmlDocument.Parse(configuration.c_str(), configuration.size());
	if(xmlError != tinyxml2::XML_SUCCESS) {
		throw common4esl::config::FilePosition::add(*this, xmlError);
	}

	const tinyxml2::XMLElement* element = xmlDocument.RootElement();
	if(element == nullptr) {
		throw common4esl::config::FilePosition::add(*this, "No root element");
	}

	setXMLFile(getFileName(), *element);
	loadXML(*element);
}

Logger::Logger(const boost::filesystem::path& filename)
: common4esl::config::Config(filename.generic_string())
{
	tinyxml2::XMLDocument xmlDocument;

	tinyxml2::XMLError xmlError = xmlDocument.LoadFile(filename.generic_string().c_str());
	if(xmlError != tinyxml2::XML_SUCCESS) {
		throw common4esl::config::FilePosition::add(*this, xmlError);
	}

	const tinyxml2::XMLElement* element = xmlDocument.RootElement();
	if(element == nullptr) {
		throw common4esl::config::FilePosition::add(*this, "No root element");
	}

	setXMLFile(filename.generic_string(), *element);
	loadXML(*element);
}

Logger::Logger(const std::string& filename, const tinyxml2::XMLElement& element)
: Config(filename, element)
{
	if(element.GetUserData() != nullptr) {
		throw common4esl::config::FilePosition::add(*this, "Element has user data but it should be empty");
	}

	for(const tinyxml2::XMLNode* node = element.FirstChild(); node != nullptr; node = node->NextSibling()) {
		const tinyxml2::XMLElement* innerElement = node->ToElement();

		if(innerElement == nullptr) {
			continue;
		}

		parseInnerElement(*innerElement);
	}
}

void Logger::save(std::ostream& oStream) const {
	oStream << "\n<esl-logger>\n";

	for(const auto& layout : layouts) {
		layout.second.save(oStream, 2);
	}

	for(const auto& appender : appenders) {
		appender.save(oStream, 2);
	}

	for(const auto& setting : settings) {
		setting.save(oStream, 2);
	}

	oStream << "</esl-logger>\n";
}

void Logger::install(monitoring::Logging& aLogging) const {
	/* *************** *
	 * Install layouts *
	 * *************** */
	for(const auto& layout : layouts) {
		aLogging.addLayout(layout.first, layout.second.create());
	}

	/* ***************** *
	 * Install appenders *
	 * ***************** */
	for(const auto& appender : appenders) {
		aLogging.addAppender(appender.getName(), appender.getLayoutId(), appender.create());
	}

	/* ***************** *
	 * Install level settings *
	 * ***************** */
	for(const auto& levelSetting : settings) {
		aLogging.setLevel(levelSetting.getLevel(), levelSetting.getScope());
	}
}

void Logger::loadXML(const tinyxml2::XMLElement& element) {
	if(element.Name() == nullptr) {
		throw common4esl::config::FilePosition::add(*this, "Name of XML root element is empty");
	}

	const std::string elementName(element.Name());

	if(elementName != "esl-logger") {
		throw common4esl::config::FilePosition::add(*this, "Name of XML root element is \"" + std::string(element.Name()) + "\" but should be \"esl-logger\"");
	}

	if(element.GetUserData() != nullptr) {
		throw common4esl::config::FilePosition::add(*this, "Node has user data but it should be empty");
	}

	for(const tinyxml2::XMLAttribute* attribute = element.FirstAttribute(); attribute != nullptr; attribute = attribute->Next()) {
		throw common4esl::config::FilePosition::add(*this, "Unknown attribute '" + std::string(attribute->Name()) + "'");
	}

	for(const tinyxml2::XMLNode* node = element.FirstChild(); node != nullptr; node = node->NextSibling()) {
		const tinyxml2::XMLElement* innerElement = node->ToElement();

		if(innerElement == nullptr) {
			continue;
		}

		auto oldXmlFile = setXMLFile(getFileName(), *innerElement);
		parseInnerElement(*innerElement);
		setXMLFile(oldXmlFile);
	}
}

void Logger::parseInnerElement(const tinyxml2::XMLElement& element) {
	if(element.Name() == nullptr) {
		throw common4esl::config::FilePosition::add(*this, "Element name is empty");
	}

	std::string elementName(element.Name());

	if(elementName == "appender") {
		appenders.push_back(Appender(getFileName(), element));
	}
	else if(elementName == "layout") {
		Layout layout(getFileName(), element);
		if(layouts.count(layout.getId()) != 0) {
			throw common4esl::config::FilePosition::add(*this, "Multiple definition of layout with id \"" + layout.getId() + "\"");
		}
		layouts.insert(std::make_pair(layout.getId(), layout));
	}
	else if(elementName == "setting") {
		settings.push_back(LevelSetting(getFileName(), element));
	}
	else {
		throw common4esl::config::FilePosition::add(*this, "Unknown element name \"" + elementName + "\"");
	}
}

} /* namespace config */
} /* inline namespace v1_6 */
} /* namespace logbook4esl */
