#ifndef COLOR_H
#define COLOR_H

using namespace std;

class Color {
public:
	int r, g, b, intensity;

	Color(int r, int g, int b, int intensity = 100);
};

#endif