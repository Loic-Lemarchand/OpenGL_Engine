#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>

namespace EventDispatcher
{
	#define BIT(x) (1 << x)

	enum EventType
	{
		None = 0,
		KeyPressed = BIT(0),
		KeyReleased = BIT(1),
		WindowResized = BIT(2)
	};

	class Event
	{
	public:
		Event(EventType type, std::string name) : myType(type), myName(name)  {}
		virtual ~Event() = default;

		virtual EventType getType() const { return myType;  }
		virtual std::string getName() const { return myName;  }

	private:

		EventType myType;

		std::string myName;

	};

	class EventBus
	{
	public:
		EventBus();
		~EventBus();

		void pushEvent(std::unique_ptr<Event> event);

		void dispatchEvents();

		void clearEvents();

		void subscribe(EventType type, std::function<void(const Event&)> callback);


	private:

		std::vector<std::unique_ptr<Event>> myEventQueue;

		std::unordered_map<EventType, std::vector<std::function<void(const Event&)>>> mySubscribers;
	};

	

}