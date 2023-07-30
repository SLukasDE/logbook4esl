#ifndef LOGBOOK4ESL_LOGGER_H_
#define LOGBOOK4ESL_LOGGER_H_

#include <esl/logging/Logger.h>

namespace logbook4esl {
inline namespace v1_6 {

#ifdef LOGBOOK4ESL_LOGGING_LEVEL_DEBUG
using Logger = esl::logging::Logger<esl::logging::Level::TRACE>;
#else
using Logger = esl::logging::Logger<esl::logging::Level::WARN>;
#endif

} /* inline namespace v1_6 */
} /* namespace logbook4esl */

#endif /* LOGBOOK4ESL_LOGGER_H_ */
