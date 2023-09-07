#pragma once

#include "Vector.hpp"

#define PI 3.14159265359

class Vector3;
class Vector4;

class Matrix4 {
	public:
		Matrix4(void);
		Matrix4(float f);
		Matrix4(float f[16]);
		Matrix4(Matrix4 const& copy);
		~Matrix4();

		Matrix4& operator=(Matrix4 const& copy);
		float operator[](size_t n) const;

		Matrix4 mulScalar(float n) const;

		Matrix4 addMatrix(Matrix4 const& m) const;
		Matrix4 subMatrix(Matrix4 const& m) const;
		Matrix4 mulMatrix(Matrix4 const& m) const;

		Matrix4	transpose() const;
		Matrix4	translation(Vector3 const& v) const;
		Matrix4	rotate(float f, Vector3 const& v) const;
		Matrix4	scale(Vector3 const& v) const;

		Matrix4	model(Matrix4 const& oneM = Matrix4(1.0f), Matrix4 const& twoM = Matrix4(1.0f), Matrix4 const& threeM = Matrix4(1.0f)) const;

		Matrix4	perspective(float f, float ratio, float near, float far);

		void	getMatrix(float fill[16]) const;

	private:
		float	matrix[16];
};