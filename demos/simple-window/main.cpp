// Your First C++ Program
#include "di.h"
#include "di_structs.h"
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>

int main() {
    std::string inputBlocking;
    
    // Start up everything needed for the window
    std::cout << "Initializing Library..." << std::endl;
    diInit();
    std::cout << "Finished Initializing Library" << std::endl;

    DI_Structure* structure = diGetStructure();

    std::cout << "Successfully got Library Struct" << std::endl;

    DI_Window* window = diCreateWindow(1200, 300, "my-basic-window", NULL, NULL);
    
    std::cout << "Finished creating window" << std::endl;
    
    while (true) {
        XFlush(structure->display);
    }
    
    std::cin >> inputBlocking;

    diTerminate();
}
