#include "../../include/Ruda/RudaDI/di.h"
#include "../../include/Ruda/RudaDI/di_structs.h"

DI_Structure::~DI_Structure() {
	delete &(currentWindow->xWindow);
	XFree(display->xDisplay);
}
