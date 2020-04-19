#include <esl/module/Library.h>
#include <logbook4esl/Module.h>

extern "C" esl::module::Module* esl__module__library__getModule(const std::string& moduleName) {
	return logbook4esl::getModulePointer(moduleName);
}
