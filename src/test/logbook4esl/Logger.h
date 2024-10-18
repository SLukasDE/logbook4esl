#ifndef LOGBOOK4ESL_LOGGER_H_
#define LOGBOOK4ESL_LOGGER_H_

#include <esl/monitoring/Logger.h>
#include <esl/monitoring/Streams.h>

namespace logbook4esl {
inline namespace v1_6 {

#ifdef LOGBOOK4ESL_MONITORING_LEVEL_DEBUG
using Logger = esl::monitoring::Logger<esl::monitoring::Streams::Level::trace>;
#else
using Logger = esl::monitoring::Logger<esl::monitoring::Streams::Level::info>;
#endif

} /* inline namespace v1_6 */
} /* namespace logbook4esl */

#endif /* LOGBOOK4ESL_LOGGER_H_ */
