#include <esl/module/Library.h>
#include <logbook4esl/Module.h>

esl::module::Library::GetModule esl__module__library__getModule = &logbook4esl::getModule;
