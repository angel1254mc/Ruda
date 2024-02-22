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
    
    while (true) {
    	
    	XFlush(rudaDI->display);
    }
    
    cin >> inputBlocking;
}
