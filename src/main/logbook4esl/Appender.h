/*
MIT License
Copyright (c) 2019, 2020 Sven Lukas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef LOGBOOK4ESL_APPENDER_H_
#define LOGBOOK4ESL_APPENDER_H_

#include <logbook/Appender.h>
#include <logbook/Location.h>
#include <esl/logging/Appender.h>

namespace logbook4esl {

class Appender : public logbook::Appender {
public:
	Appender(esl::logging::Appender& eslAppender);

	esl::logging::Appender& eslAppender;

protected:
	void flush() override;
	void write(const logbook::Location& location, const char* ptr, std::size_t size) override;
};

} /* namespace logbook4esl */

#endif /* LOGBOOK4ESL_APPENDER_H_ */
