#pragma once

#include "eventBus.h"

class InputManager
{
public:
	InputManager(EventDispatcher::EventBus& eventBus);

	void processKey(int key, int action);
	void processMouse(double xPos, double yPos);

	EventDispatcher::EventBus& getEventBus() { return myEventBus; }

private:
	EventDispatcher::EventBus& myEventBus;
};

// Aliases to the generic event
using KeyPressedEvent = EventDispatcher::PayloadEvent<int>;
using KeyReleasedEvent = EventDispatcher::PayloadEvent<int>;
using MouseEvent = EventDispatcher::PayloadEvent<double, double>;
using WindowResizeEvent = EventDispatcher::PayloadEvent<int, int>;