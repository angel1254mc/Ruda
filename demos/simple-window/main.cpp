// Your First C++ Program
#include "rudadi.h"
#include <iostream>
#include <stdlib.h>

int main() {
    string inputBlocking;
    RudaDI* rudaDI = new RudaDI();
    // Start up everything needed for the window
    rudaDI->rudaInit();
    
    RudaDIWindow* window = rudaDI->rudaCreateWindow(300, 200, "my-basic-window", NULL, NULL);
    
    cin >> inputBlocking;
}
