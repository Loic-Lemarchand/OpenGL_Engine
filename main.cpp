#include "window.h"
#include "utilities.h"
#include "renderer.h"
#include <memory>
#include "inputManager.h"
#include "camera.h"


int main(int argc, char* argv[])
{

	
	std::unique_ptr<Renderer> renderer = nullptr;
	std::unique_ptr<EventDispatcher::EventBus> eventBus = std::make_unique<EventDispatcher::EventBus>();
	std::unique_ptr<InputManager> inputManager = std::make_unique<InputManager>(*eventBus);
	std::unique_ptr<Window> window = std::make_unique<Window>(1920, 1080, "My Window", *inputManager);


	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraDirection = -glm::normalize(cameraPosition - cameraTarget);

	// calculer pitch et yaw à partir de la direction
	float initPitch = glm::degrees(asin(cameraDirection.y));
	float initYaw = glm::degrees(atan2(cameraDirection.z, cameraDirection.x));

	//glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), -cameraDirection) start up
	std::unique_ptr<Camera> camera = std::make_unique<Camera>(cameraPosition, glm::vec3(0.0f, 1.0f, 0.0f), initYaw, initPitch, 2.0f, .1f, *eventBus);


	if (!window->bIsValid)
	{
		utilities::log("Window is not valid");
	}

	renderer = std::make_unique<Renderer>(window->getFrameBufferWidth(), window->getFrameBufferHeight(), camera.get());

	if (!renderer->bIsValid)
	{
		utilities::log("Renderer is not valid");
	}

	while (!window->shoudClose() && window->bIsValid)
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		window->update();

		//EVENT BUS POLLING
		eventBus->dispatchEvents();

		camera->update();

		//RENDERER WORK HERE
		renderer->update();
		//renderer->update(glm::vec3(0.0f, 0.0f, -2.5f), 45, glm::vec3(0.0f, window->triOffset, 0.0f), glm::vec3(0.4f, 0.4f, 1.0f));

		//PHYSICS COMPUTATION HERE
	}

}