#include "RudaDI/di.h"
#include "Ruda/ruda.h"
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

    float vertices[18] = {
        -0.50f,     -0.25f,      0.00f,
         0.50f,     -0.75f,      0.00f,
         0.00f,      0.50f,      0.00f,
        -0.50f,     -0.50f,      0.00f,
         0.50f,     -0.50f,      0.00f,
         0.00,      -1.00f,      0.00f
    };
    int tri_count = ((sizeof(vertices)/sizeof(float))/3);
    uint VBO;

    rudaGenBuffer(&VBO);
    
    rudaBindVBO(&VBO);

    rudaBufferArrayData(sizeof(vertices), vertices);

    rudaEnableVertexAttribArray(0);

    rudaVertexAttribPointer(0, 3, RUDA_FLOAT, sizeof(float) * 3, reinterpret_cast<void*>(0));

    diSetKeyCallback(window, key_callback);

    int* windowSize = diGetWindowSize(window);
    
    while (true) {
        
        diDrawArrays(RUDA_TRIS, 0, tri_count);

    	//diDrawLine(0,0,windowSize[0],windowSize[1], 0, 255, 255);
        diPollEvents();
    	diFlush();
    }
    
    std::cin >> inputBlocking;
}
