#include <iostream>
#ifndef COLOR_H
#define COLOR_H

using namespace std;

class Color {
public:
	float r, g, b;

	Color() = default;
	Color(float r, float g, float b);
	Color Clamp();
	Color Clamp255();
	Color operator+(const Color c);
	Color operator*(const float f);
	Color operator*(const Color c);
	Color operator/(const float f);
	friend std::ostream& operator<<(std::ostream& output, const Color& color) {
		output << "Color(" << color.r << ", " << color.g << ", " << color.b << ")";
		return output;
	}
};

#endif