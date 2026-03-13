#pragma once

#include "eventBus.h"

class InputManager
{
public:
	InputManager(EventDispatcher::EventBus& eventBus);

	void processKey(int key, int action);

	EventDispatcher::EventBus& getEventBus() { return myEventBus; }

private:
	EventDispatcher::EventBus& myEventBus;
};


class KeyPressedEvent : public EventDispatcher::Event
{
public:
	KeyPressedEvent(int key) : Event(EventDispatcher::EventType::KeyPressed, "Pressed Key : " + std::to_string(key)), myKey(key) {};

	int getKey() const { return myKey; }

	int getCategoryFlags() const { return EventDispatcher::EventType::KeyPressed; };


private:
	int myKey;
};

class KeyReleasedEvent : public EventDispatcher::Event
{
public:
	KeyReleasedEvent(int key) : Event(EventDispatcher::EventType::KeyReleased, "Released Key : " + std::to_string(key)), myKey(key) {};

	int getKey() const { return myKey; }

	int getCategoryFlags() const { return EventDispatcher::EventType::KeyReleased; };

private:
	int myKey;
};

class MouseEvent : public EventDispatcher::Event
{
public:
	MouseEvent(double xPos, double yPos) : Event(EventDispatcher::EventType::MouseMoved, "Moved mouse to x : " + std::to_string((int)xPos) + " and y : " + std::to_string((int)yPos)), myXPos(xPos), myYPos(yPos) {};

	double getXPos() const { return myXPos; }
	double getYPos() const { return myYPos; }

	int getCategoryFlags() const { return EventDispatcher::EventType::MouseMoved; };

private:
	double myXPos, myYPos;
};



class WindowResizeEvent : public EventDispatcher::Event
{
public:
	WindowResizeEvent(int width, int height) : Event(EventDispatcher::EventType::WindowResized, "Window resized - width : " + std::to_string(width) + " height : " + std::to_string(height)), myWidth(width), myHeight(height) {};

	int getWidth() const { return myWidth; }
	int getHeight() const { return myHeight; }

	int getCategoryFlags() const { return EventDispatcher::EventType::WindowResized; };

private:
	int myWidth;
	int myHeight;
};
