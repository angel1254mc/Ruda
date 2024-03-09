#include "RudaDI/di.h"
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <string>

void key_callback(DI_Window* window, int keycode, bool pressed, int mods) {
        std::cout << "Window that triggered Callback : " << diGetWindowTitle(window) << std::endl;

        std::cout << "Keycode : " << keycode << std::endl;
        std::cout << "Pressed? : " << (pressed == 1 ? "true" : "false") << std::endl;
}

int main() {
    std::string inputBlocking;
    // Start up everything needed for the window
    diInit();
    
    DI_Window* window = diCreateWindow("simple-window", 1920, 1080, nullptr, nullptr);

    diSetKeyCallback(window, key_callback);

    int* windowSize = diGetWindowSize(window);
    
    while (true) {
        diPollEvents();
    	diDrawLine(0,0,windowSize[0],windowSize[1], 0, 255, 255);
    	diFlush();
    }
    
    std::cin >> inputBlocking;
}
