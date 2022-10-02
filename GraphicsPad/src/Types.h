#pragma once

struct Vec2 {
	float x, y;
	Vec2 operator+(Vec2 other) { return { x + other.x, y + other.y }; }
	Vec2 operator*(Vec2 other) { return { x * other.x, y * other.y }; }
	Vec2 operator*(float val) { return { x * val, y * val }; }
};

struct Vec3 {
	float x, y, z;
};

struct Vec4 {
	float x, y, z, w;
};

struct Vertex {
	Vec2 position;
	Vec3 color;
};