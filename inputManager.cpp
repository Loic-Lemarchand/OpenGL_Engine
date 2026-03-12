#include "inputManager.h"
#include <GLFW/glfw3.h>


InputManager::InputManager(EventDispatcher::EventBus& eventBus) : myEventBus(eventBus)
{

}

void InputManager::processKey(int key, int action)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		myEventBus.pushEvent(std::make_unique<KeyPressedEvent>(key));
	}
	else if (action == GLFW_RELEASE)
	{
		myEventBus.pushEvent(std::make_unique<KeyReleasedEvent>(key));
	}
}