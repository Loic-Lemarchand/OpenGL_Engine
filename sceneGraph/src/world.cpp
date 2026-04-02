#include "world.h"
#include "actor.h"

std::shared_ptr<EventDispatcher::EventBus> World::myEventBus = nullptr;

World::World()
{
}

World::~World()
{
}

void World::Tick()
{
	for (auto& actor : myActors)
	{
		actor->Tick();
	}

	// Fixed physics timestep
	myPhysicsWorld.Step(1.0f / 60.0f);
}

void World::Render()
{
}
