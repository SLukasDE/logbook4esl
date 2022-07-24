#ifndef LOGBOOK4ESL_EXAMPLE04_H_
#define LOGBOOK4ESL_EXAMPLE04_H_

namespace logbook4esl {

struct Example04 final {
	//Example04() = delete;

	static void run();

private:
	float divide(float a, float b);
	static void loggerReplay();
};

} /* namespace logbook4esl */

#endif /* LOGBOOK4ESL_EXAMPLE04_H_ */
