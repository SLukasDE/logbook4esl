#include "logbook4esl/Example05.h"
#include "logbook4esl/Logger.h"

#include <esl/monitoring/LogbookLogging.h>
#include <esl/monitoring/MemBufferAppender.h>
#include <esl/monitoring/OStreamAppender.h>
#include <esl/monitoring/SimpleLayout.h>
#include <esl/plugin/Registry.h>

namespace logbook4esl {
inline namespace v1_6 {

namespace {
/* creating a logger class */
Logger logger("logbook4esl::Example05");
}

void Example05::run() {
	esl::plugin::Registry& registry(esl::plugin::Registry::get());
	registry.addPlugin("esl/monitoring/MemBufferAppender", esl::monitoring::MemBufferAppender::create);
	registry.addPlugin("esl/monitoring/OStreamAppender", esl::monitoring::OStreamAppender::create);
	registry.addPlugin("esl/monitoring/DefaultLayout", esl::monitoring::SimpleLayout::create);
	{
		auto logging = esl::monitoring::LogbookLogging::createNative();
		logging->addFile("logger.xml");
		registry.setObject(std::move(logging));
	}

	/* That's it, now you can use it already */
	logger.info << "Hello world!" << std::endl;
}

} /* inline namespace v1_6 */
} /* namespace logbook4esl */
