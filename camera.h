#pragma once

#include <glad/glad.h>

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "eventBus.h"
#include <unordered_set>


class Camera
{
public:
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat setMovementSpeed, GLfloat setTurnSpeed, EventDispatcher::EventBus& eventBus);
	~Camera();

	bool bIsValid;

	void update();


	//Callback functions
	void onKeyBoardInput(int key);
	void onKeyBoardRelease(int key);
	void onMouseInput(double x, double y);

	glm::mat4 CalculateViewMatrix();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	double lastX;
	double lastY;
	
	std::unordered_set<int> keysDown;
	
};