#include "camera.h"
#include "utilities.h"
#include "world.h"

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat setMovementSpeed, GLfloat setTurnSpeed) :
	bIsValid(true),
	SceneComponent(startPosition),
	up(startUp),
	yaw(startYaw),
	pitch(startPitch),
	movementSpeed(setMovementSpeed),
	turnSpeed(setTurnSpeed),
	front(glm::vec3(0.0f, 0.0f, -1.0f)),
	right(0),
	worldUp(startUp),
	lastX(0.0f),
	lastY(0.0f),
	bFirstMove(true)
{
	std::shared_ptr<EventDispatcher::EventBus> eventBus = World::getEventBus();
	eventBus->subscribe(EventDispatcher::KeyPressed, &Camera::onKeyBoardInput, this);
	eventBus->subscribe(EventDispatcher::MouseMoved, &Camera::onMouseInput, this);
	eventBus->subscribe(EventDispatcher::KeyReleased, &Camera::onKeyBoardRelease, this);
	update();
}

Camera::~Camera()
{
	
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);

	//Always orthogonal worldUp and right vectors - WE BLOCK THE POSSIBILITY TO ROLL
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

	float cameraSpeed = movementSpeed * deltaTime;
	for (const auto key : keysDown)
	{
		switch (key)
		{
			case GLFW_KEY_W:
				setWorldPosition(getWorldPosition() + cameraSpeed * front);
				break;
			case GLFW_KEY_A:
				setWorldPosition(getWorldPosition() - cameraSpeed * right);
				break;
			case GLFW_KEY_S:
				setWorldPosition(getWorldPosition() - cameraSpeed * front);
				break;
			case GLFW_KEY_D:
				setWorldPosition(getWorldPosition() + cameraSpeed * right);
				break;
			case GLFW_KEY_E:
				setWorldPosition(getWorldPosition() + cameraSpeed * up);
				break;
			case GLFW_KEY_Q:
				setWorldPosition(getWorldPosition() - cameraSpeed * up);
				break;
			default:
				break;
		}
	}

	CalculateViewMatrix();
}

glm::mat4 Camera::CalculateViewMatrix()
{
	viewMatrix = glm::lookAt(getWorldPosition(), getWorldPosition() + front, up);
	return viewMatrix;
}


void Camera::onKeyBoardInput(int key)
{
	keysDown.insert(key);
	utilities::log("Camera position is : x = " + std::to_string(getWorldPosition().x) + " y = " + std::to_string(getWorldPosition().y) + " z = " + std::to_string(getWorldPosition().z));
}

void Camera::onKeyBoardRelease(int key)
{
	keysDown.erase(key);
}

void Camera::onMouseInput(double x, double y)
{
	if (bFirstMove)
	{
		lastX = x;
		lastY = y;
		bFirstMove = false;
	}

	float xOffset =  x - lastX;
	float yOffset = lastY - y;

	lastX = x;
	lastY = y;

	xOffset *= turnSpeed;
	yOffset *= turnSpeed;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	utilities::log("Camera rotation is : yaw = " + std::to_string(yaw) + " pitch = " + std::to_string(pitch));
}

ACamera::ACamera()
{
	myCameraComponent = addSceneComponent<Camera>(
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		-90.0f, 0.0f, 2.0f, 0.1f
	);
}

ACamera::~ACamera()
{

}

void ACamera::Tick()
{
	Actor::Tick();
	if (myCameraComponent)
	{
		myCameraComponent->update();
	}
}
