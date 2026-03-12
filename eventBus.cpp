#include "eventBus.h"

namespace EventDispatcher{
	EventBus::EventBus()
	{

	}


	EventBus::~EventBus()
	{
		clearEvents();
	}

	void EventBus::pushEvent(std::unique_ptr<Event> event)
	{
		myEventQueue.push_back(std::move(event));
	}

	void EventBus::subscribe(EventType type, std::function<void(const Event&)> callback)
	{
		mySubscribers[type].push_back(callback);
	}

	void EventBus::dispatchEvents()
	{
		for (auto& event : myEventQueue)
		{
			EventType type = event->getType();
			if (mySubscribers.contains(type))
			{
				for (auto& callback : mySubscribers[type])
				{
					callback(*event);
				}
			}
		}
		clearEvents();
	}

	void EventBus::clearEvents()
	{
		myEventQueue.clear();
	}
}
