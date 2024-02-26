#include "../../include/Ruda/RudaDI/di_structs.h"
#include "../../include/Ruda/RudaDI/di.h"

DI_Structure::~DI_Structure() {
	for (auto el : windows)
		delete el;
	if (this->helperWindowHandle)
		XFree(&(this->helperWindowHandle));
	if (this->display)
		XFree(this->display);
}

DI_Structure* diGetStructure() {
	return structure;
}
