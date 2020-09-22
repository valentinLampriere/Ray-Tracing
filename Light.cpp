#include "light.h"

Light::Light(Vector3 pos, Color c, float intensity) {
	position = pos;
	color = c;
	this->intensity = intensity;
}

Color Light::GetColor() {
	return color * intensity;
}