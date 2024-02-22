#include "../../include/RudaDI/di.h"


DI_Structure::~DI_Structure() {
	delete &(this->window);
	XFree(this->display);
}
