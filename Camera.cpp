#include "camera.h"

Camera::Camera(int width, int height, int depth, Vector3 position) {
	this->position = position;
	this->width = width;
	this->height = height;
	this->depth = depth;
	origin = Vector3(position.x + width / 2, position.y + height / 2, position.z - depth);
}
Vector3 Camera::GetNormalAtPoint(Vector3 point) {
	return (point - origin).normalized();
}