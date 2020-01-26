#ifndef ESL_EXAMPLES_LOGGER_H_
#define ESL_EXAMPLES_LOGGER_H_

#include <esl/logging/Logger.h>

namespace esl {
namespace examples {

extern esl::logging::Logger<> logger;
void loggerInitialize();
void loggerReplay();

} /* namespace examples */
} /* namespace esl */

#endif /* ESL_EXAMPLES_LOGGER_H_ */
