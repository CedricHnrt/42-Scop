#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <cmath>

class Vec3 {
	public:
		float x, y, z;
        Vec3() : x(0), y(0), z(0) {}
		Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
};

class Mat4 {
	public:
		float m[16];

		static Mat4 identity();
		static Mat4 perpective(float fov, float aspect, float near, float far); // FOV in angles, converted in radians internally
		Mat4 operator*(const Mat4& other) const;

	private:
		Mat4() = default; 
};

#endif //VECTORS_HPP
