#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "component.h"


class Actor
{
public:
	Actor();
	~Actor();

	bool isValid() { return bIsValid; }

	virtual void Tick();

	glm::vec3 getPosition() { if (!myRootComponent) return glm::vec3(0.0f); return myRootComponent->getLocalPosition(); }
	float getRotation() { if (!myRootComponent) return 0.0f; return myRootComponent->getLocalRotation(); }
	glm::vec3 getScale() { if (!myRootComponent) return glm::vec3(0.0f); return myRootComponent->getLocalScale(); }
	glm::vec3 getRotationAxis() { if (!myRootComponent) return glm::vec3(0.0f); return myRootComponent->getLocalRotationAxis(); }
	std::shared_ptr<SceneComponent> getRootComponent() { return myRootComponent; }

	void setPosition(glm::vec3 position) { if (myRootComponent) myRootComponent->setLocalPosition(position); }
	void setRotation(float rotation) { if (myRootComponent) myRootComponent->setLocalRotation(rotation); }
	void SetScale(glm::vec3 scale) { if (myRootComponent) myRootComponent->SetLocalScale(scale); }
	void SetRotationAxis(glm::vec3 rotationAxis) { if (myRootComponent) myRootComponent->SetLocalRotationAxis(rotationAxis); }

	const std::vector<std::shared_ptr<Component>>& getComponents() const { return myComponents; }

	template<typename T, typename... Args>
	std::shared_ptr<T> addSceneComponent(Args&&... args)
	{
		auto comp = std::make_shared<T>(std::forward<Args>(args)...);
		comp->setOwner(this);
		comp->setActive(true);
		if (!myRootComponent)
			myRootComponent = comp;
		else
			myRootComponent->AddChild(comp);
		myComponents.push_back(comp);
		return comp;
		
	}

	template<typename T, typename... Args>
	std::shared_ptr<T> addActorComponent(Args&&... args)
	{
		auto comp = std::make_shared<T>(std::forward<Args>(args)...);
		comp->setOwner(this);
		comp->setActive(true);
		myComponents.push_back(comp);
		return comp;
	}

	void collectRenderProxies(std::vector<RenderProxy>& outProxies);
	void collectFromComponent(const SceneComponent& comp, std::vector<RenderProxy>& outProxies);

protected:

	std::shared_ptr<SceneComponent> myRootComponent;
	std::vector<std::shared_ptr<Component>> myComponents;

	bool bIsValid;
};