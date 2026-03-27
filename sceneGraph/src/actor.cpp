#include "actor.h"
#include "component.h"

Actor::Actor() :
	bIsValid(true),
	myRootComponent(nullptr)
{
}

Actor::~Actor()
{

}

void Actor::Tick()
{
	if (myRootComponent)
	{
		myRootComponent->Tick();
	}

	for(auto component : myComponents)
	{
		//Tick on components that are not in the graph (logic pure components with no transform)
		if (!component->canRecurseTick())
		{
			component->Tick();
		}
	}
}
