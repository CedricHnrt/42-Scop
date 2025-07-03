#include "matrix.hpp"
#include "../include/matrix.hpp" // REMOVE

Vec3 Vec3::cross(const Vec3 &a, const Vec3 &b) {
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

Vec3 Vec3::normalize(const Vec3 &v) {
	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length == 0.0f) {
		return {0, 0, 0}; // Avoid division by zero
	}
	return {v.x / length, v.y / length, v.z / length};
}

float Vec3::dot(const Vec3 &a, const Vec3 &b) {
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

Vec3 Vec3::operator+(const Vec3& rhs) const {
	return {this->x + rhs.x, this->y + rhs.y, this->z + rhs.z};
}

Vec3 Vec3::operator-(const Vec3& rhs) const {
	return {this->x - rhs.x, this->y - rhs.y, this->z - rhs.z};
}

Vec3& Vec3::operator+=(const Vec3& rhs) {
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
	return *this;
}

Vec3 &Vec3::operator/=(const float rhs) {
	this->x /= rhs;
	this->y /= rhs;
	this->z /= rhs;
	return *this;
}


Mat4 Mat4::identity() {
	const float identity[16] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1};

	return Mat4(identity);
}

Mat4 Mat4::perpective(float fov, const float aspect, const float near, const float far) {
	fov *= M_PI / 180.0f; // Convert FOV from degrees to radians
	const float f = 1.0f / tanf(fov / 2.0f);
	const float nf = 1.0f / (far - near);
	
	const float perspective[16] = {
		f / aspect,	0,	0,						0,
		0,			f,	0,						0,
		0,			0,	-(far + near) * nf,		-1,
		0,			0,	-(2 * far * near) *	nf,	0};

	return Mat4(perspective);
}

Mat4 Mat4::rotateY(float angle) {
	const float c = cosf(angle);
	const float s = sinf(angle);

	const float rotationY[16] = {
		c, 0, s, 0,
		0, 1, 0, 0,
		-s, 0, c, 0,
		0, 0, 0, 1};
	return Mat4(rotationY);
}


Mat4 Mat4::lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up) {
	const Vec3 f = Vec3::normalize(center - eye);
	const Vec3 s = Vec3::normalize(Vec3::cross(f, up));
	const Vec3 u = Vec3::cross(s, f);
	
	const float lookAt[16] = {
		s.x,				u.x,				-f.x,				0,
		s.y,				u.y,				-f.y,				0,
		s.z,				u.z,				-f.z,				0,
		-Vec3::dot(s, eye),	-Vec3::dot(u, eye),	Vec3::dot(f, eye),	1};

	return Mat4(lookAt);
}


Mat4 Mat4::operator*(const Mat4& other) const {
	float result[16];
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			result[col * 4 + row] = 0;
			for (int k = 0; k < 4; ++k) {
				result[col * 4 + row] += this->m[k * 4 + row] * other.m[col * 4 + k];
			}
		}
	}
	return Mat4(result);
}

const float *Mat4::data() const {
	return m;
}

Mat4::Mat4(const float value[16]) {
	for (int i = 0; i < 16; i++)
		m[i] = value[i];
}
