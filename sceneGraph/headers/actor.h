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

	glm::vec3 getPosition() { return myPosition; }
	float getRotation() { return myRotation; }
	glm::vec3 getScale() { return myScale; }
	glm::vec3 getRotationAxis() { return myRotationAxis; }
	std::shared_ptr<SceneComponent> getRootComponent() { return myRootComponent; }

	void setPosition(glm::vec3 position) { myPosition = position; if (myRootComponent) myRootComponent->setPosition(position); }
	void setRotation(float rotation) { myRotation = rotation; if (myRootComponent) myRootComponent->setRotation(rotation); }
	void SetScale(glm::vec3 scale) { myScale = scale; if (myRootComponent) myRootComponent->SetScale(scale); }
	void SetRotationAxis(glm::vec3 rotationAxis) { myRotationAxis = rotationAxis; if (myRootComponent) myRootComponent->SetRotationAxis(rotationAxis); }

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
	glm::vec3 myPosition;
	glm::vec3 myScale;
	float myRotation;
	glm::vec3 myRotationAxis;

	std::shared_ptr<SceneComponent> myRootComponent;
	std::vector<std::shared_ptr<Component>> myComponents;

	bool bIsValid;
};