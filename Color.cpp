#include "color.h"

Color::Color() {
	r = 255;
	g = 255;
	b = 255;
}
Color::Color(float r, float g, float b) {
	this->r = r;
	this->g = g;
	this->b = b;
}
Color Color::Clamp() {
	float max = std::max(this->r, this->g);
	max = std::max(max, this->b);
	return Color((this->r / max) * 255, (this->g / max) * 255, (this->b / max) * 255);
}
Color Color::ToAlbedo() {
	return this->Clamp() / 255;
}
Color Color::operator*(const float f) {
	return Color(this->r * f, this->g * f, this->b * f);
}
Color Color::operator*(const Color c) {
	return Color(this->r * c.r, this->g * c.g, this->b * c.b);
}
Color Color::operator/(const float f) {
	return Color(this->r / f, this->g / f, this->b / f);
}