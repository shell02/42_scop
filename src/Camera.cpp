#include "Camera.hpp"

float toRadians(float f);

Camera::Camera(Vector3 cameraPos, Vector3 cameraFront, Vector3 upVector)
	: cameraPos(cameraPos), cameraFront(cameraFront), upVector(upVector), deltatime(0), pitch(0.0f), yaw(-90.0f)
{
	return ;
}

Camera::Camera(const Camera& copy)
	: cameraPos(copy.cameraPos), cameraFront(copy.cameraFront), upVector(copy.upVector), deltatime(copy.deltatime), pitch(copy.pitch), yaw(copy.yaw)
{
	return ;
}

Camera::~Camera()
{
	return ;
}

Camera& Camera::operator=(const Camera& copy)
{
	cameraPos = copy.cameraPos;
	cameraFront = copy.cameraFront;
	upVector = copy.upVector;
	deltatime = copy.deltatime;
	pitch = copy.pitch;
	yaw = copy.yaw;
	return *this;
}

Matrix4 Camera::LookAt(void) const
{
	Matrix4 camera;
	
	Vector3 cameraDir = cameraPos.subVector(cameraPos.addVector(cameraFront)).unit();
	Vector3 cameraRight = upVector.crossVector(cameraDir).unit();
	Vector3 cameraUp = cameraDir.crossVector(cameraRight);

	float rot[16] = {
		cameraRight[0], cameraRight[1], cameraRight[2], 0,
		cameraUp[0], cameraUp[1], cameraUp[2], 0,
		cameraDir[0], cameraDir[1], cameraDir[2], 0,
		0, 0, 0, 1
	};

	float pos[16] = {
		1, 0, 0, -(cameraPos[0]),
		0, 1, 0, -(cameraPos[1]),
		0, 0, 1, -(cameraPos[2]),
		0, 0, 0, 1
	};

	// Matrix4 rotation(rot);
	// Matrix4 position(pos);
	
	camera = Matrix4(rot).mulMatrix(Matrix4(pos));

	return camera;
}

void Camera::updatePos(Vector3 newPos)
{
	cameraPos = newPos;
}

void Camera::updateFront()
{
	cameraFront = Vector3(
		cos(toRadians(yaw)) * cos(toRadians(pitch)),
		sin(toRadians(pitch)),
		sin(toRadians(yaw)) * cos(toRadians(pitch))
	).unit();
}

void Camera::updateFront(Vector3 newFront)
{
	cameraFront = newFront;
}

void Camera::updateUp(Vector3 newUp)
{
	upVector = newUp;
}

void Camera::moveLeft()
{
	float cameraSpeed = 2.5f * deltatime;
	cameraPos = cameraPos.subVector(cameraFront.crossVector(upVector).unit().mulScalar(cameraSpeed));
}

void Camera::moveRight()
{
	float cameraSpeed = 2.5f * deltatime;
	cameraPos = cameraPos.addVector(cameraFront.crossVector(upVector).unit().mulScalar(cameraSpeed));
}

void Camera::moveFore()
{
	float cameraSpeed = 2.5f * deltatime;
	cameraPos = cameraPos.addVector(cameraFront.mulScalar(cameraSpeed));
}

void Camera::moveBack()
{
	float cameraSpeed = 2.5f * deltatime;
	cameraPos = cameraPos.subVector(cameraFront.mulScalar(cameraSpeed));
}

void Camera::rotateLeft()
{
	yaw -= 0.06f;
	updateFront();
	// cameraFront = cameraFront.addVector(Vector3(cos(cameraSpeed), 0.0f, sin(cameraSpeed))).unit();

	// std::cout << "CameraFront: X " << cameraFront.getX() << " Y " << cameraFront.getY() << " Z " << cameraFront.getZ() << std::endl;

}

void Camera::rotateRight()
{
	yaw += 0.06f;
	updateFront();

	// cameraFront = cameraFront.addVector(Vector3(cameraSpeed, 0.0f, 0.0f)).unit();

}

void Camera::rotateUp()
{
	pitch += 0.06f;
	if(pitch > 89.0f)
		pitch =  89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;
	updateFront();
	// cameraFront = cameraFront.addVector(Vector3(0.0f, cameraSpeed, 0.0f)).unit();

}

void Camera::rotateDown()
{
	pitch -= 0.06f;
	if(pitch > 89.0f)
  		pitch =  89.0f;
	if(pitch < -89.0f)
  		pitch = -89.0f;
	updateFront();
	// cameraFront = cameraFront.subVector(Vector3(0.0f, cameraSpeed, 0.0f)).unit();

}

void Camera::setDeltaTime(float d)
{
	deltatime = d;
}

Vector3 Camera::getPos() const
{
	return cameraPos;
}
