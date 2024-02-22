// Your First C++ Program
#include "rudadi.h"
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>

int main() {
    string inputBlocking;
    RudaDI* rudaDI = new RudaDI();
    // Start up everything needed for the window
    rudaDI->rudaInit();
    
    RudaDIWindow* window = rudaDI->rudaCreateWindow(300, 300, "my-basic-window", NULL, NULL);
    cout << "Finished creating window" << endl;
    for (int i = 0; i < 300; i++) {
        XDrawLine(rudaDI->display, window->xWindow, window->gc, i, i, i, i);
        XFlush(rudaDI->display);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    cin >> inputBlocking;
}
