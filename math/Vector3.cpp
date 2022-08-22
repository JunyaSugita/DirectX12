#include "Vector3.h"
#include <cmath>	//sqrt

Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

float Vector3::length() const { return sqrt((x * x) + (y * y) + (z * z)); }

Vector3& Vector3::normalize() {
	float len = length();
	if (len != 0) {
		return *this /= length();
	}
	return *this;
}

float Vector3::dot(const Vector3& v) const { return (x * v.x) + (y * v.y) + (z * v.z); }

Vector3 Vector3::cross(const Vector3& v) const {
	return Vector3((y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y - v.x));
}

Vector3 Vector3::operator+() const { return *this; }

Vector3 Vector3::operator-() const { return Vector3(-x, -y, -z); }

Vector3& Vector3::operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

// Vector3 �N���X�ɑ����Ȃ��֐��Q
// 2�����Z�q
const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 tenp(v1);
	return tenp -= v2;
}

const Vector3 operator*(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v) { return v * s; }

const Vector3 operator*(const Vector3& v, const Vector3& s) { 
	Vector3 ans;

	ans.x = v.x * s.x;
	ans.y = v.y * s.y;
	ans.z = v.z * s.z;

	return ans; 
}

const Vector3 operator/(const Vector3& v, float s) {
	Vector3 temp(v);
	return v / s;
}

Vector3& Vector3::operator*=(const Matrix4& m2) {
	Vector3 ans;

	ans.x = x * m2.m[0][0] + y * m2.m[1][0] + z * m2.m[2][0];
	ans.y = x * m2.m[0][1] + y * m2.m[1][1] + z * m2.m[2][1];
	ans.z = x * m2.m[0][2] + y * m2.m[1][2] + z * m2.m[2][2];

	*this = ans;
	return *this;
}