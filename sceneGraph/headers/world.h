#pragma once

#include <vector>
#include <memory>

class Actor;
namespace EventDispatcher 
{
	class EventBus;
}


class World
{
public:
	World(const World& otherWorld) = delete;
	World& operator=(const World&) = delete;
	~World();
	void Tick();

	void Render();

	static std::shared_ptr<EventDispatcher::EventBus> getEventBus() { return myEventBus; }
	static void setEventBus(std::shared_ptr<EventDispatcher::EventBus> bus) { myEventBus = bus; }

	static World& getWorld() 
	{ 
	    static World instance;
	    return instance; 
	}

	template<typename T, typename... Args>
	std::shared_ptr<T> spawnActor(Args&&... args) {
		auto actor = std::make_shared<T>(std::forward<Args>(args)...);
		myActors.push_back(actor);
		return actor;
	}

	std::vector<std::shared_ptr<Actor>>& getActors() { return myActors; }
	
private:
	World();

	std::vector<std::shared_ptr<Actor>> myActors;
	static std::shared_ptr<EventDispatcher::EventBus> myEventBus;
};