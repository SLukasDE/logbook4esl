#ifndef LOGBOOK4ESL_EXAMPLE04_H_
#define LOGBOOK4ESL_EXAMPLE04_H_

namespace logbook4esl {
inline namespace v1_6 {

struct Example04 final {
	//Example04() = delete;

	static void run();

private:
	float divide(float a, float b);
	static void loggerReplay();
};

} /* inline namespace v1_6 */
} /* namespace logbook4esl */

#endif /* LOGBOOK4ESL_EXAMPLE04_H_ */
