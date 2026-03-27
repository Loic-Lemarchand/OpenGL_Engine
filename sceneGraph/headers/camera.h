#pragma once

#include <glad/glad.h>

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "eventBus.h"
#include "component.h"
#include "actor.h"
#include <unordered_set>
#include <memory>

class Shader;

class Camera : public SceneComponent
{
public:
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat setMovementSpeed, GLfloat setTurnSpeed);
	~Camera();

	bool bIsValid;

	void update();


	//Callback functions
	void onKeyBoardInput(int key);
	void onKeyBoardRelease(int key);
	void onMouseInput(double x, double y);

	glm::mat4 CalculateViewMatrix();

	glm::mat4 getViewMatrix() const { return viewMatrix; }

private:
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	GLfloat yaw;
	GLfloat pitch;

	double lastX;
	double lastY;
	bool bFirstMove;
	
	std::unordered_set<int> keysDown;

	glm::mat4 viewMatrix;
	
};

class ACamera : public Actor
{
public:
	ACamera();
	~ACamera();

	std::shared_ptr<Camera> getCameraComponent() { return myCameraComponent; }

	void Tick() override;
private:
	std::shared_ptr<Camera> myCameraComponent;
};