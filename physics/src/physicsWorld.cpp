#include "physicsWorld.h"

#define restitution 0.5f
#define SLEEP_VELOCITY_THRESHOLD 0.001f
#define PENETRATION_SLOP 0.01f

PhysicsWorld::PhysicsWorld() :
	myGravity(glm::vec3(0.0f, -1.81f, 0.0f))
{

}

PhysicsWorld::~PhysicsWorld()
{

}

void PhysicsWorld::registerRigidBody(std::shared_ptr<RigidBodyComponent> rigidBodyComp, std::shared_ptr<SceneComponent> sceneComp)
{
	auto physicsObject = std::make_shared<PhysicsObject>();
	physicsObject->rigidBodyComponent = rigidBodyComp;
	physicsObject->sceneComponent = sceneComp;
	myPhysicsObjects.push_back(physicsObject);
}

void PhysicsWorld::Step(float dt)
{
	// Sync all collider positions (including static objects)
	for (auto& obj : myPhysicsObjects)
	{
		if (!obj->rigidBodyComponent->getColliderComponent())
			continue;
		obj->rigidBodyComponent->getColliderComponent()->setWorldPosition(obj->sceneComponent->getWorldPosition());
	}

	for (auto& physicsObjectA : myPhysicsObjects)
	{
		RigidBodyComponent& rigidBodyA = *physicsObjectA->rigidBodyComponent;
		SceneComponent& sceneCompA = *physicsObjectA->sceneComponent;

		if (rigidBodyA.getBodyType() != bodyType::Dynamic)
			continue;

		//Apply gravity
		rigidBodyA += myGravity * rigidBodyA.getMass();

		//Apply force only on dynamic objects
		rigidBodyA.Integrate(dt, physicsObjectA->sceneComponent);
		rigidBodyA.ClearForces();

		//Update collider position after integration
		if (!rigidBodyA.getColliderComponent())
			continue;
		ColliderComponent& colliderComponentA = *rigidBodyA.getColliderComponent();
		colliderComponentA.setWorldPosition(sceneCompA.getWorldPosition());

		// Detect collisions
		for (auto& physicsObjectB : myPhysicsObjects)
		{
			if (physicsObjectB == physicsObjectA)
				continue;

			RigidBodyComponent& rigidBodyB = *physicsObjectB->rigidBodyComponent;
			SceneComponent& sceneCompB = *physicsObjectB->sceneComponent;

			// Process the collision pair once : we impose A < B by memory address
			if (rigidBodyB.getBodyType() == bodyType::Dynamic
				&& physicsObjectA.get() >= physicsObjectB.get())
				continue;

			if (!rigidBodyB.getColliderComponent())
				continue;

			ColliderComponent& colliderComponentB = *rigidBodyB.getColliderComponent();

			CollisionManyFold manifold = colliderComponentA.checkCollision(colliderComponentB);
			if (!manifold.bIsColliding)
				continue;

			// Position correction with slop to avoid jitter
			float correctionMag = glm::max(manifold.penetrationDepth - PENETRATION_SLOP, 0.0f);
			glm::vec3 correction = manifold.normal * correctionMag;

			if (rigidBodyA.getBodyType() == bodyType::Dynamic && rigidBodyB.getBodyType() == bodyType::Dynamic)
			{
				sceneCompA.setWorldPosition(sceneCompA.getWorldPosition() - correction * 0.5f);
				sceneCompB.setWorldPosition(sceneCompB.getWorldPosition() + correction * 0.5f);
			}
			else if (rigidBodyA.getBodyType() != bodyType::Dynamic)
			{
				sceneCompB.setWorldPosition(sceneCompB.getWorldPosition() + correction);
			}
			else if(rigidBodyB.getBodyType() != bodyType::Dynamic)
			{
				sceneCompA.setWorldPosition(sceneCompA.getWorldPosition() - correction);
			}


			//Impulse resolve
			glm::vec3 relativeVelocity = rigidBodyA.getSpeed() - rigidBodyB.getSpeed();
			float velocityDotNormal = glm::dot(relativeVelocity, manifold.normal);
			if (velocityDotNormal > 0.0f)
				continue;

			//Wake up sleeping objects on collision
			//We use the speed of the collision to determine if our objects can wake up or not
			if (rigidBodyA.getBodyType() == bodyType::Sleeping
				&& glm::abs(velocityDotNormal) > SLEEP_VELOCITY_THRESHOLD)
				rigidBodyA.setBodyType(bodyType::Dynamic);
			if (rigidBodyB.getBodyType() == bodyType::Sleeping
				&& glm::abs(velocityDotNormal) > SLEEP_VELOCITY_THRESHOLD)
				rigidBodyB.setBodyType(bodyType::Dynamic);

			float totalInvMass = rigidBodyA.getMassInverse() + rigidBodyB.getMassInverse();

			if (totalInvMass == 0.0f)
				continue;

			//Apply impulse
			float impulseMag = -(1.0f + restitution) * velocityDotNormal / totalInvMass;
			glm::vec3 impulse = impulseMag * manifold.normal;

			if (rigidBodyA.getBodyType() == bodyType::Dynamic)
				rigidBodyA.setSpeed(rigidBodyA.getSpeed() + impulse * rigidBodyA.getMassInverse());
			if (rigidBodyB.getBodyType() == bodyType::Dynamic)
				rigidBodyB.setSpeed(rigidBodyB.getSpeed() - impulse * rigidBodyB.getMassInverse());
		}

		// Sleep: zero out near-zero velocities to prevent jitter
		if (glm::length(rigidBodyA.getSpeed()) < SLEEP_VELOCITY_THRESHOLD)
			rigidBodyA.setBodyType(bodyType::Sleeping);
	}

}
