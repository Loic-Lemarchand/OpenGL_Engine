#include "inputManager.h"
#include "utilities.h"
#include <GLFW/glfw3.h>


InputManager::InputManager(EventDispatcher::EventBus& eventBus) : myEventBus(eventBus)
{

}

void InputManager::processKey(int key, int action)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		//myEventBus.pushEvent(std::make_unique<KeyPressedEvent>(key));
		myEventBus.pushEvent(EventDispatcher::makeEvent(EventDispatcher::EventType::KeyPressed, "Pressed Key : " + std::to_string(key), key));
	}
	else if (action == GLFW_RELEASE)
	{
		//myEventBus.pushEvent(std::make_unique<KeyReleasedEvent>(key));
		myEventBus.pushEvent(EventDispatcher::makeEvent(EventDispatcher::EventType::KeyReleased, "ReleasedKey : " + std::to_string(key), key));
	}
}

void InputManager::processMouse(double xPos, double yPos)
{
	myEventBus.pushEvent(EventDispatcher::makeEvent(EventDispatcher::EventType::MouseMoved, "Moved mouse", xPos, yPos));
}