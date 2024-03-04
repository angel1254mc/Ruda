// Your First C++ Program
#include "RudaDI/di.h"
// #include "RudaDI/di_structs.h"
// #include "../../src/RudaDI/init.cpp"
// #include "../../src/RudaDI/di_window.cpp"
// #include "../../src/RudaDI/draw.cpp"
// #include "../../src/RudaDI/structure.cpp"
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <string>

int main() {
    std::string inputBlocking;
    // Start up everything needed for the window
    diInit();
    
    diCreateWindow(1920, 1080, "my-basic-window", NULL, NULL);

    std::cout << "Finished creating window" << std::endl;
    
    while (true) {
    	diDrawLine(0,0,structure->currentWindow->width,structure->currentWindow->height, 0, 255, 255);
    	XFlush(structure->display->xDisplay);
    }
    
    std::cin >> inputBlocking;
}
