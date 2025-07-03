#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <cmath>

class Vec3 {
	public:
		float x, y, z;
        Vec3() : x(0), y(0), z(0) {}
		Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
		Vec3 operator+(const Vec3& rhs) const;
		Vec3 operator-(const Vec3& rhs) const;
		Vec3& operator+=(const Vec3& rhs);
		Vec3& operator/=(float rhs);
		[[nodiscard]] static Vec3 cross(const Vec3& a, const Vec3& b);
		static Vec3 normalize(const Vec3& v);
		static float dot(const Vec3& a, const Vec3& b);
};

class Mat4 {
	public:
		explicit Mat4(const float[16]);
		static Mat4 identity();
		static Mat4 perpective(float fov, float aspect, float near, float far); // FOV in angles, converted in radians internally
		static Mat4 rotateY(float angle);
		static Mat4 lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up);
		Mat4 operator*(const Mat4& other) const;
		[[nodiscard]] const float* data() const;

	private:
		float m[16]{};
	
		Mat4() = default; 
};

#endif //VECTORS_HPP
