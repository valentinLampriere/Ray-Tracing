#include "color.h"

Color::Color(int r, int g, int b, int intensity) {
	this->r = r % 255;
	this->g = g % 255;
	this->b = b % 255;
	this->intensity = intensity%1000;
}

