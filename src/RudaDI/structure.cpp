#include "../../include/Ruda/RudaDI/di_structs.h"
#include "../../include/Ruda/RudaDI/di.h"

DI_Structure::~DI_Structure() {

	initialized = false;

	// Window deletion
	for (auto el : windows)
		delete el;
	windows.clear();
	currentWindow = nullptr;

	// Monitor deletion
	for (auto el : monitors)
		delete el;
	monitors.clear();
	monitorCount = 0;

	//Cursor Deletion
	for (auto el : cursors)
		delete el;	
	cursors.clear();

	// content scale reset
	contentScaleX, contentScaleY = 0;

	// Clear struct callbacks
	callbacks.monitor = nullptr;

	// delete helper window/hidden cursor 
	if (this->helperWindowHandle)
		XFree(&(this->helperWindowHandle));
	if (this->hiddenCursorHandle)
		XFree(&(this->hiddenCursorHandle));

	// Finally, once all dependencies are freed we can 
	if (this->display)
		XFree(this->display);
}

DI_Structure* diGetStructure() {
	return structure;
}
