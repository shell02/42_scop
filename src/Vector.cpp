#include "Vector.hpp"


Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{
	return ;
}

Vector3::Vector3(Vector3 const &copy) : x(copy.x), y(copy.y), z(copy.z)
{
	return ;	
}

Vector3::Vector3(float tab[3]): x(tab[0]), y(tab[1]), z(tab[2])
{
	return ;
}

Vector3::~Vector3()
{
	return ;
}

Vector3& Vector3::operator=(Vector3 const &copy)
{
	x = copy.x;
	y = copy.y;
	z = copy.z;
	return *this;
}

Vector3& Vector3::operator=(float tab[3])
{
	x = tab[0];
	y = tab[1];
	z = tab[2];
	return *this;
}

float Vector3::operator[](size_t n) const
{
	switch (n)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		return 0;
	};
}

Vector3 Vector3::addScalar(float n) const
{
	Vector3 tmp(*this);
	tmp.x += n;
	tmp.y += n;
	tmp.z += n;
	return tmp;
}

Vector3 Vector3::subScalar(float n) const
{
	Vector3 tmp(*this);
	tmp.x -= n;
	tmp.y -= n;
	tmp.z -= n;
	return tmp;
}

Vector3 Vector3::mulScalar(float n) const
{
	Vector3 tmp(*this);
	tmp.x *= n;
	tmp.y *= n;
	tmp.z *= n;
	return tmp;
}

Vector3 Vector3::divScalar(float n) const
{
	Vector3 tmp(*this);
	tmp.x /= n;
	tmp.y /= n;
	tmp.z /= n;
	return tmp;
}

Vector3 Vector3::addVector(Vector3 const& v) const
{
	Vector3 tmp(*this);
	tmp.x += v.x;
	tmp.y += v.y;
	tmp.z += v.z;
	return tmp;
}

Vector3 Vector3::subVector(Vector3 const& v) const
{
	Vector3 tmp(*this);
	tmp.x -= v.x;
	tmp.y -= v.y;
	tmp.z -= v.z;
	return tmp;
}

double Vector3::dotVector(Vector3 const& v) const
{
	double result = (x * v.x) + (y * v.y) + (z * v.z);	

	return result;
}

Vector3 Vector3::crossVector(Vector3 const& v) const
{
	Vector3	tmp;

	tmp.x = (y * v.z) - (z * v.y);
	tmp.y = (z * v.x) - (x * v.z);
	tmp.z = (x * v.y) - (y * v.x);
	return tmp;
}

double Vector3::getLength(void) const
{
	return sqrt((x*x) + (y*y) + (z*z));
}

Vector3 Vector3::unit(void) const
{
	Vector3	tmp(*this);
	tmp = tmp.divScalar(this->getLength());
	return tmp;
}

float Vector3::getX(void) const
{
	return x;
}

float Vector3::getY(void) const
{
	return y;
}

float Vector3::getZ(void) const
{
	return z;
}

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
	return ;
}

Vector4::Vector4(Vector4 const& copy) : x(copy.x), y(copy.y), z(copy.z), w(copy.w)
{
	return ;	
}

Vector4::Vector4(Vector3 const& v, float w) : x(v.getX()), y(v.getY()), z(v.getZ()), w(w)
{
	return ;
}

Vector4::Vector4(float tab[4]) : x(tab[0]), y(tab[1]), z(tab[2]), w(tab[3])
{
	return ;
}

Vector4::~Vector4()
{
	return ;
}

Vector4& Vector4::operator=(Vector4 const &copy)
{
	x = copy.x;
	y = copy.y;
	z = copy.z;
	w = copy.w;
	return *this;
}

Vector4& Vector4::operator=(float tab[4])
{
	x = tab[0];
	y = tab[1];
	z = tab[2];
	w = tab[3];
	return *this;
}

float Vector4::operator[](size_t n) const
{
	switch (n)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	default:
		return 0;
	};
}

Vector4 Vector4::mulMatrix(Matrix4 const &m) const
{
	Vector4 tmp(*this);
	float result;

	for (int i = 0; i < 4; i++) {
		result = 0;
		for (int j = 0; j < 4; j++) {
			float n = (j == 0 ? x : j == 1 ? y : j == 2 ? z : w);
			result += m[i * 4 + j] * n;
		}
		switch (i)
		{
			case 0:
				tmp.x = result;
				break;
			case 1:
				tmp.y = result;
				break;
			case 2:
				tmp.z = result;
				break;
			default:
				tmp.w = result;
				break;
		}
	}
	return tmp;
}

float Vector4::getX(void) const
{
	return x;
}

float Vector4::getY(void) const
{
	return y;
}

float Vector4::getZ(void) const
{
	return z;
}

float Vector4::getW(void) const
{
	return w;
}
