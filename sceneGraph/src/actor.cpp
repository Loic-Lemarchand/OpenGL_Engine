#include "actor.h"
#include "component.h"

Actor::Actor() :
	bIsValid(true),
	myRootComponent(std::make_shared<SceneComponent>())
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

void Actor::collectRenderProxies(std::vector<RenderProxy>& outProxies)
{
	if (myRootComponent)
		collectFromComponent(*myRootComponent, outProxies);
}

void Actor::collectFromComponent(const SceneComponent& comp, std::vector<RenderProxy>& outProxies)
{

	RenderProxy renderProxyToAdd;
	if (comp.getRenderProxy(renderProxyToAdd))
	{
		outProxies.push_back(renderProxyToAdd);
	}

	//Recurse on child
	for (const auto& child : comp.getChildrenComps())
	{
		if (child)
			collectFromComponent(*child, outProxies);
	}
	
}
