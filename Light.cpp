#include "light.h"

Light::Light(Vector3 pos, Color c) {
	position = pos;
	color = c;
}

Color Light::getColor() {
	return color;
}