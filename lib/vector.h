#ifndef VECTOR_H
#define VECTOR_H

class Vector3 {
public:
	float x;
	float y;
	float z;

	Vector3();
	Vector3(float vectorX, float vectorY, float vectorZ);
	float lengthSquare();
	float length();
	static float dot(Vector3 v1, float f);
	float dot(float f);
	static Vector3 cross(Vector3 v1, Vector3 v2);
	Vector3 cross(Vector3 v);
	static Vector3 normalize(Vector3 v1);
	Vector3 normalized();
	Vector3 operator+(const Vector3 v);
	Vector3 operator+(float f);
	Vector3 operator-(const Vector3 v);
	Vector3 operator-(float f);
	Vector3 operator/(const float f);
	Vector3 operator*(const float f);
	Vector3 operator*(const Vector3 v);
	friend std::ostream &operator<<(std::ostream &output, const Vector3 &vector);
};

#endif