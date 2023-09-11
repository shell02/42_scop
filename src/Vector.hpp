#pragma once

#include <cmath>
#include "Matrix.hpp"

class Matrix4;

class Vector3 {
	public:
		Vector3(float x = 0, float y = 0, float z = 0);
		Vector3(Vector3 const &copy);
		Vector3(float tab[3]);
		~Vector3();

		Vector3	&operator=(Vector3 const &copy);
		Vector3	&operator=(float tab[3]);
		float	operator[](size_t n) const;

		Vector3	addScalar(float n) const;
		Vector3	subScalar(float n) const;
		Vector3	mulScalar(float n) const;
		Vector3	divScalar(float n) const;

		Vector3	addVector(Vector3 const& v) const;
		Vector3	subVector(Vector3 const& v) const;
		double	dotVector(Vector3 const& v) const;
		Vector3	crossVector(Vector3 const& v) const;

		double	getLength(void) const;
		Vector3	unit(void) const;

		float	getX(void) const;
		float	getY(void) const;
		float	getZ(void) const;

	private:
		float	x;
		float	y;
		float	z;
};

class Vector4 {
	public:
		Vector4(float x = 0, float y = 0, float z = 0, float w = 0);
		Vector4(Vector4 const& copy);
		Vector4(float tab[4]);
		Vector4(Vector3 const& v, float w = 1);
		~Vector4();

		Vector4&	operator=(Vector4 const &copy);
		Vector4&	operator=(float tab[4]);
		float		operator[](size_t n) const;

		Vector4	mulMatrix(Matrix4 const &m) const;

		float	getX(void) const;
		float	getY(void) const;
		float	getZ(void) const;
		float	getW(void) const;

	private:
		float	x;
		float	y;
		float	z;
		float	w;
};