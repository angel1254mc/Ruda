#pragma once

struct Color {
	unsigned int r;
	unsigned int g;
	unsigned int b;
	XColor xColor;
	Color(unsigned int r, unsigned int g, unsigned int b) {
		this->r = r; this->g = g; this->b = b;
		xColor.red = r * 257;
 		xColor.green = g * 257;
 		xColor.blue = b * 257;
 		xColor.flags = DoRed | DoGreen | DoBlue;
	};	
};