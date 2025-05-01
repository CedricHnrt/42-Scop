#ifndef VECTORS_HPP
#define VECTORS_HPP

class Vec3 {
	public:
		float x, y, z;
        Vec3() : x(0), y(0), z(0) {}
		Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
};

//class Vec2 {
//	public:
//		float u, v;
//		Vec2() : u(0), v(0) {}
//		Vec2(const float u, const float v) : u(u), v(v) {}
//};

#endif //VECTORS_HPP
