#include "RudaDI/di.h"
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <string>

unsigned int width = 1920;
unsigned int height = 1080;
int player_width = 100;
int player_height = 100;
int x = width / 2 - player_width / 2;
int y = height / 2 - player_height / 2;
bool forward = false;
bool backward = false;
bool left = false;
bool right = false;

void key_callback(DI_Window *window, int keycode, bool pressed, int mods)
{

    if (keycode == DI_W)
    {
        std::cout << "Forward Key: " << pressed << std::endl;
        forward = pressed;
    }
    if (keycode == DI_A)
    {
        left = pressed;
    }
    if (keycode == DI_S)
    {
        backward = pressed;
    }
    if (keycode == DI_D)
    {
        right = pressed;
    }
}

void handleMovement()
{
    if (forward)
    {
        y -= 5;
    }
    if (backward)
    {
        y += 5;
    }
    if (left)
    {
        x -= 5;
    }
    if (right)
    {
        x += 5;
    }
}

int main()
{
    std::string inputBlocking;
    // Start up everything needed for the window
    diInit();

    DI_Window *window = diCreateWindow("simple-window", 1920, 1080, nullptr, nullptr);

    diSetKeyCallback(window, key_callback);

    std::cout << "Hello World" << std::endl;
    int *windowSize = diGetWindowSize(window);

    while (true)
    {
        diPollEvents();
        handleMovement();
        diClearWindow();
        diDrawRectangle(x, y, player_width, player_height, 255, 255, 255);
        diFlush();
        usleep(10000);
    }

    std::cin >> inputBlocking;
}
