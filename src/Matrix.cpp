#include "Matrix.hpp"

Matrix4::Matrix4(void)
{
	for (int i = 0; i < 16; i++)
		matrix[i] = 0;
}

Matrix4::Matrix4(float f[16])
{
	if (f == 0)
	{
		for (int i = 0; i < 16; i++)
			matrix[i] = 0;
	}
	else
	{
		for (int i = 0; i < 16; i++)
			matrix[i] = f[i];
	}
	return ;
}

Matrix4::Matrix4(Matrix4 const& copy)
{
	for (int i = 0; i < 16; i++)
		matrix[i] = copy.matrix[i];
	return ;
}

Matrix4::Matrix4(float f)
{
	for (int i = 0; i < 16; i++)
		matrix[i] = 0;
	matrix[0] = f;
	matrix[5] = f;
	matrix[10] = f;
	matrix[15] = f;
}

Matrix4::~Matrix4()
{
	return ;
}

Matrix4& Matrix4::operator=(Matrix4 const& copy)
{
	for (int i = 0; i < 16; i++)
		matrix[i] = copy.matrix[i];
	return *this;
}

float Matrix4::operator[](size_t n) const
{
	return matrix[n];
}

Matrix4 Matrix4::mulScalar(float n) const
{
	Matrix4 tmp(*this);
	for (int i = 0; i < 16; i++)
		tmp.matrix[i] *= n;
	return tmp;
}

Matrix4 Matrix4::addMatrix(Matrix4 const& m) const
{
	Matrix4 tmp(*this);
	for (int i = 0; i < 16; i++)
		tmp.matrix[i] += m.matrix[i];
	return tmp;
}

Matrix4 Matrix4::subMatrix(Matrix4 const& m) const
{
	Matrix4 tmp(*this);
	for (int i = 0; i < 16; i++)
		tmp.matrix[i] -= m.matrix[i];
	return tmp;
}

Matrix4 Matrix4::mulMatrix(Matrix4 const& m) const
{
	Matrix4	newM;
	Vector4 v;
	Vector4	result;
	float	tmp[4];

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			tmp[i] = m[i * 4 + j];
		}
		v = tmp;
		result = v.mulMatrix(*this);
		for (int i = 0; i < 4; i++) {
			newM.matrix[i * 4 + j] = result[i];
		}
	}
	return newM;
}

Matrix4 Matrix4::transpose() const
{
	Matrix4 transposeM;
	int ij;
	int ji;

	for (int i = 0; i < 4; i++) {
		for (int j = i; j < 4; j++) {
			ij = i * 4 + j;
			ji = j * 4 + i;
			transposeM.matrix[ij] = matrix[ji];
			transposeM.matrix[ji] = matrix[ij];
		}
	}
	return transposeM;
};

Matrix4 Matrix4::translation(Vector3 const& v) const
{
	// Matrix4	tmp(*this);
	Matrix4	transM(1.0f);

	transM.matrix[3] = v.getX();
	transM.matrix[7] = v.getY();
	transM.matrix[11] = v.getZ();

	// tmp = tmp.mulMatrix(transM);
	// for (int i = 0; i < 4; i++) {
	// 	for (int j = 0; j < 4; j++) {
	// 		std:: cout << transM[i*4+j] << ", ";
	// 	}
	// 	std::cout << std::endl;
	// };
	return transM;
}

Matrix4 Matrix4::rotate(float f, Vector3 const& v) const
{
	// Matrix4	newM(*this);
	Matrix4 rotateM(1.0f);
	Vector3	tmp = v.unit();

	f = f * (PI / 180);
	float cosA = cos(f);
	float sinA = sin(f);
	// float round = (int)(cos(f) * 100000 + 0.5f);
	// float cosA = (float)round / 100000;
	// round = (int)(sin(f) * 100000 + 0.5f);
	// float sinA = (float)round /100000;


	rotateM.matrix[0] = pow(tmp[0], 2) * (1 - cosA) + cosA;
	rotateM.matrix[1] = tmp[0] * tmp[1] * (1 - cosA) - tmp[2] * sinA;
	rotateM.matrix[2] = tmp[0] * tmp[2] * (1 - cosA) + tmp[1] * sinA;
	rotateM.matrix[4] = tmp[1] * tmp[0] * (1 - cosA) + tmp[2] * sinA;
	rotateM.matrix[5] = pow(tmp[1], 2) * (1 - cosA) + cosA;
	rotateM.matrix[6] = tmp[1] * tmp[2] * (1 - cosA) - tmp[0] * sinA;
	rotateM.matrix[8] = tmp[2] * tmp[0] * (1 - cosA) - tmp[1] * sinA;
	rotateM.matrix[9] = tmp[2] * tmp[1] * (1 - cosA) + tmp[0] * sinA;
	rotateM.matrix[10] = pow(tmp[2], 2) * (1 - cosA) + cosA;

	return rotateM;
}

Matrix4 Matrix4::scale(Vector3 const& v) const
{
	// Matrix4 newM(*this);
	Matrix4 scaleM(1.0f);

	scaleM.matrix[0] = v[0];
	scaleM.matrix[5] = v[1];
	scaleM.matrix[10] = v[2];

	// newM = newM.mulMatrix(scaleM);

	return scaleM;
}

Matrix4 Matrix4::model(Matrix4 const& oneM, Matrix4 const& twoM, Matrix4 const& threeM) const
{
	Matrix4	model(1.0f);

	model = model.mulMatrix(oneM).mulMatrix(twoM).mulMatrix(threeM);
	return model;
}

void Matrix4::getMatrix(float fill[16]) const
{
	for (int i = 0; i < 16; i++)
		fill[i] = matrix[i];
	return ;
}

Matrix4	Matrix4::perspective(float f, float ratio, float near, float far) {
	Matrix4 perspectiveM(1.0f);
	f = f * (PI / 180); //to radians

	const float e = tan(f / 2.0);
	const float range = near - far;

	perspectiveM.matrix[0] = 1.0f / (ratio * e);
	perspectiveM.matrix[5] = 1.0f / (e);
	perspectiveM.matrix[10] = (-near - far) / range * -1;
	perspectiveM.matrix[11] = (2.0f * far * near) / range;
	perspectiveM.matrix[14] = -1.0f;
	perspectiveM.matrix[15] = 0.0f;

	return perspectiveM;
}
