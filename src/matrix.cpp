#include "matrix.hpp"

Mat4 Mat4::identity() {
	const float identity[16] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1};

	Mat4 m4{};
	for (int i = 0; i < 16; i++)
		m4.m[i] = identity[i];
	return m4;
}

Mat4 Mat4::perpective(float fov, const float aspect, const float near, const float far) {
	fov *= M_PI / 180.0f; // Convert FOV from degrees to radians
	const float f = 1.0f / tanf(fov / 2.0f);
	const float nf = 1.0f / (near - far);
	
	const float perspective[16] = {
		f / aspect, 0, 0, 0,
		0, f, 0, 0,
		0, 0, (far + near) * nf, -1,
		0, 0, (2 * far * near) * nf, 0
	};

	Mat4 m4 {};
	for (int i = 0; i < 16; i++)
		m4.m[i] = perspective[i];
	return m4;
}

Mat4 Mat4::operator*(const Mat4& other) const {
	float result[16];
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			result[row * 4 + col] = 0;
			for (int k = 0; k < 4; ++k) {
				result[row * 4 + col] += this->m[row * 4 + k] * other.m[k * 4 + col];
			}
		}
	}

	Mat4 m4{};
	for (int i = 0; i < 16; i++)
		m4.m[i] = result[i];
	return m4;
}