#include "camera.h"

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat setMovementSpeed, GLfloat setTurnSpeed) :
	position(startPosition),
	up(startUp),
	yaw(startYaw),
	pitch(startPitch),
	movementSpeed(setMovementSpeed),
	turnSpeed(setTurnSpeed),
	front(0),
	right(0),
	worldUp(0)
{
	
}

Camera::~Camera()
{

}

void Camera::update()
{

}