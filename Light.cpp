#include "light.h"

Light::Light(Vector3 pos, Color c, float intensity, float radius) {
	position = pos;
	color = c;
	this->intensity = intensity;
	this->radius = radius;
}

Color Light::GetColor() {
	return color * intensity;
}
