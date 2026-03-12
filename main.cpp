#include "window.h"
#include "utilities.h"
#include "renderer.h"
#include <memory>
#include "inputManager.h"


int main(int argc, char* argv[])
{
	
	std::unique_ptr<Renderer> renderer = nullptr;
	std::unique_ptr<EventDispatcher::EventBus> eventBus = std::make_unique<EventDispatcher::EventBus>();
	std::unique_ptr<InputManager> inputManager = std::make_unique<InputManager>(*eventBus);
	std::unique_ptr<Window> window = std::make_unique<Window>(640, 720, "My Window", *inputManager);


	if (!window->bIsValid)
	{
		utilities::log("Window is not valid");
	}

	renderer = std::make_unique<Renderer>(window->getFrameBufferWidth(), window->getFrameBufferHeight());

	if (!renderer->bIsValid)
	{
		utilities::log("Renderer is not valid");
	}

	while (!window->shoudClose() && window->bIsValid)
	{
		window->update();

		//EVENT BUS POLLING
		eventBus->dispatchEvents();

		//RENDERER WORK HERE

		renderer->update(glm::vec3(0.0f, 0.0f, -2.5f), window->triOffset, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.4f, 0.4f, 0.4f));
		//renderer->update(glm::vec3(0.0f, 0.0f, -2.5f), 45, glm::vec3(0.0f, window->triOffset, 0.0f), glm::vec3(0.4f, 0.4f, 1.0f));


		//PHYSICS COMPUTATION HERE

	
	}
}