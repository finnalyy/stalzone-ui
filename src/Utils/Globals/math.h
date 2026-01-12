#pragma once

#include <imgui.h>

#include <string>
#include <unordered_map>
#include <iostream>

struct Vector2
{
	float x, y;

	Vector2(float x = 0, float y = 0) : x(x), y(y) {}

	float Magnitude() const
	{
		return std::sqrt(x * x + y * y);
	}

	Vector2 Normalize() const
	{
		float mag = Magnitude();
		return Vector2(x / mag, y / mag);
	}

	static float Distance(const Vector2& v1, const Vector2& v2)
	{
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 operator*(float scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector2& v)
	{
		os << "(" << v.x << ", " << v.y << ")";
		return os;
	}

	std::string to_string() {
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}


	operator ImVec2() const {
		return ImVec2(x, y);
	}
};

struct Vector3
{
	float x, y, z;
	Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

	friend std::ostream& operator<<(std::ostream& os, const Vector3& v)
	{
		os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return os;
	}

	std::string to_string() {
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
	}

	bool operator!=(const Vector3& rhs) const
	{
		return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
	}

	bool operator==(const Vector3& rhs) const
	{
		return !((x != rhs.x) || (y != rhs.y) || (z != rhs.z));
	}

	Vector3 operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	bool isNearZero() {
		// Проверка на NaN
		if (x != x || y != y || z != z) {
			return true;
		}

		// Проверка на очень большие значения
		const float MAX_VALID = 1e10f;
		if (fabs(x) > MAX_VALID || fabs(y) > MAX_VALID || fabs(z) > MAX_VALID) {
			return true;
		}

		// Проверка на очень маленькие денормализованные числа
		const float MIN_VALID = 1e-10f;
		if ((fabs(x) < MIN_VALID && x != 0.0f) ||
			(fabs(y) < MIN_VALID && y != 0.0f) ||
			(fabs(z) < MIN_VALID && z != 0.0f)) {
			return true;
		}

		// Проверка на конкретное мусорное значение (1, 0, 1.25)
		if (fabs(x - 1.0f) < 0.001f && fabs(y) < 0.001f && fabs(z - 1.25f) < 0.001f) {
			return true;
		}

		return false;
	}

	static float Distance(const Vector3& v1, const Vector3& v2)
	{
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;
		float dz = v1.z - v2.z;
		return std::sqrt(dx * dx + dy * dy + dz * dz);
	}

	operator Vector2() const
	{
		return Vector2(x, y);
	}

	operator ImVec2() const {
		return ImVec2(x, y);
	}
};

struct Vector4 {
	float x, y, z, w;
};

struct view_matrix_t {
	float* operator[ ](int index) {
		return matrix[index];
	}

	float matrix[4][4];

	void transpose() {
		for (int i = 0; i < 4; ++i) {
			for (int j = i + 1; j < 4; ++j) {
				std::swap(matrix[i][j], matrix[j][i]);
			}
		}
	}
};


bool worldToScreenPoint(Vector3 worldPoint, view_matrix_t viewProjMtx, Vector3& screenPoint);