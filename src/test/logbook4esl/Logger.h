#ifndef LOGBOOK4ESL_LOGGER_H_
#define LOGBOOK4ESL_LOGGER_H_

#include <esl/monitoring/Logger.h>

namespace logbook4esl {
inline namespace v1_6 {

#ifdef LOGBOOK4ESL_MONITORING_LEVEL_DEBUG
using Logger = esl::monitoring::Logger<esl::monitoring::Level::TRACE>;
#else
using Logger = esl::monitoring::Logger<esl::monitoring::Level::WARN>;
#endif

} /* inline namespace v1_6 */
} /* namespace logbook4esl */

#endif /* LOGBOOK4ESL_LOGGER_H_ */
