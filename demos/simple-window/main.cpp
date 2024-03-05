// Your First C++ Program
#include "RudaDI/di.h"
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <string>

int main() {
    std::string inputBlocking;
    // Start up everything needed for the window
    diInit();
    
    diCreateWindow("simple-window", 1920, 1080);

    print("Finished creating window");
    
    while (true) {
    	diDrawLine(0,0,structure->currentWindow->config.width,structure->currentWindow->config.height, 0, 255, 255);
    	XFlush(structure->display->xDisplay);
    }
    
    std::cin >> inputBlocking;
}
