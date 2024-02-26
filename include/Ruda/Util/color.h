#pragma once
#include "../RudaDI/di_structs.h"
struct Color {
	unsigned int r;
	unsigned int g;
	unsigned int b;
	Color(unsigned int r, unsigned int g, unsigned int b) { this->r = r; this->g = g; this->b = b;};
};
