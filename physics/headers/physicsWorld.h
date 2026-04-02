#pragma once

#include <vector>
#include "physicsComponent.h"

class PhysicsWorld
{
	struct PhysicsObject
	{
		std::shared_ptr<RigidBodyComponent> rigidBodyComponent;
		std::shared_ptr<SceneComponent> sceneComponent;
	};

public:
	PhysicsWorld();
	~PhysicsWorld();

	void registerRigidBody(std::shared_ptr<RigidBodyComponent> rigidBodyComp, std::shared_ptr<SceneComponent> sceneComp);
	void Step(float dt);

private:

	std::vector<std::shared_ptr<PhysicsObject>> myPhysicsObjects;
	glm::vec3 myGravity;
};