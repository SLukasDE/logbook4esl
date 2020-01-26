#ifndef ESL_EXAMPLES_LOGGING_EXAMPLE03_H_
#define ESL_EXAMPLES_LOGGING_EXAMPLE03_H_

#include <esl/logging/Logger.h>

namespace esl {
namespace examples {
namespace logging {

class Example03 {
public:
	Example03() = default;
	~Example03() = default;

	float divide(float a, float b);

private:
	static esl::logging::Logger<> logger;
};

void example03();

} /* namespace logging */
} /* namespace examples */
} /* namespace esl */

#endif /* ESL_EXAMPLES_LOGGING_EXAMPLE03_H_ */
