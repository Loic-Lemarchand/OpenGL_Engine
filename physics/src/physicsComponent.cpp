#include "physicsComponent.h"

RigidBodyComponent::RigidBodyComponent(bodyType bodyType, double mass) :
	myBodyType(bodyType),
	myMass(mass),
	mySpeed(0.0f),
	myResultingForce(0.0f),
	myMassInverse(1/mass),
	myAcceleration(0.0f),
	myLastFrameAcceleration(0.0f),
	myLinearDamping(0.99f)
{
	if (myBodyType == bodyType::Static)
	{
		myMassInverse = 0.0f;
	}
}

RigidBodyComponent::~RigidBodyComponent()
{
}

void RigidBodyComponent::AddForces(const std::vector<glm::vec3>& forces)
{
	for (const auto& force : forces)
	{
		myResultingForce += force;
	}
}

void RigidBodyComponent::CalculateAcceleration()
{
	// 1 - Linear Acceleration
	myLastFrameAcceleration = myAcceleration;
	glm::vec3 vecToAdd = myResultingForce * myMassInverse;
	myAcceleration = vecToAdd;

	// 2 - TODO:Angular acceleration using torques
}

void RigidBodyComponent::Integrate(float duration, std::shared_ptr<SceneComponent> sceneComponent)
{
	CalculateAcceleration();
	// 3 - TODO:Update Angular velocity


	// 4 - Impose drag TODO: (need to also add rotation later)
	glm::vec3 updatedVelocity = myAcceleration * duration;
	mySpeed += updatedVelocity;
	mySpeed *= glm::pow(myLinearDamping, duration);

	// 5 - Update linear position
	glm::vec3 movement = mySpeed * duration;
	sceneComponent->setWorldPosition(sceneComponent->getWorldPosition() + movement);
}

ColliderComponent::ColliderComponent(glm::vec3 position, ShapeType shapeType, ShapeData shapeData) :
	SceneComponent(position),
	myCenterOfMass(0.0f),
	myShapeType(shapeType),
	myShapeData(shapeData)
{
}

ColliderComponent::~ColliderComponent()
{
}

void ColliderComponent::calculateCenterOfMassFromMesh(const MeshComponent& meshComponent)
{
	myCenterOfMass = glm::vec3(0.0f, 0.0f, 0.0f);
}

AABB ColliderComponent::getAABB() const
{
	glm::vec3 worldCenter = getWorldPosition() + myCenterOfMass;
	AABB aabb;
	switch (myShapeType)
	{
		case ShapeType::Sphere:
		{
			float radius = getShapeData<SphereShape>().radius;
			glm::vec3 halfExtent = glm::vec3(radius);
			aabb.min = worldCenter - halfExtent;
			aabb.max = worldCenter + halfExtent;
			break;
		}
		case ShapeType::Box:
		{
			glm::vec3 halfExtent = getShapeData<BoxShape>().halfExtents;
			aabb.min = worldCenter - halfExtent;
			aabb.max = worldCenter + halfExtent;
			break;
		}
		case ShapeType::Plane:
		{
			const float large = 1e6f;
			aabb.min = glm::vec3(-large);
			aabb.max = glm::vec3(large);
			break;
		}
		default:
		{
			aabb.min = worldCenter;
			aabb.max = worldCenter;
			break;
		}
	}
	return aabb;

}

//TODO: Review this method to be sure about it
CollisionManyFold ColliderComponent::checkCollision(const ColliderComponent& other)
{
	CollisionManyFold manifold;
	manifold.bIsColliding = false;

	// Skip AABB broadphase if neither shape is a plane
	if (myShapeType != ShapeType::Plane && other.getShapeType() != ShapeType::Plane)
	{
		if (!getAABB().overlaps(other.getAABB()))
			return manifold;
	}

	// --- Sphere vs Sphere ---
	if (myShapeType == ShapeType::Sphere && other.getShapeType() == ShapeType::Sphere)
	{
		glm::vec3 centerA = getWorldPosition() + myCenterOfMass;
		glm::vec3 centerB = other.getWorldPosition() + other.myCenterOfMass;
		float radiusA = getShapeData<SphereShape>().radius;
		float radiusB = other.getShapeData<SphereShape>().radius;

		glm::vec3 delta = centerA - centerB;
		float distance = glm::length(delta);
		float sumRadii = radiusA + radiusB;

		if (distance > 0.0f && distance < sumRadii)
		{
			manifold.bIsColliding = true;
			manifold.normal = glm::normalize(delta);
			manifold.penetrationDepth = sumRadii - distance;
			manifold.contactPoints.push_back(centerB + manifold.normal * radiusB);
		}
	}
	// --- Box vs Box ---
	else if (myShapeType == ShapeType::Box && other.getShapeType() == ShapeType::Box)
	{
		glm::vec3 centerA = getWorldPosition() + myCenterOfMass;
		glm::vec3 centerB = other.getWorldPosition() + other.myCenterOfMass;
		glm::vec3 halfA = getShapeData<BoxShape>().halfExtents;
		glm::vec3 halfB = other.getShapeData<BoxShape>().halfExtents;

		glm::vec3 delta = centerA - centerB;

		glm::vec3 overlap(
			halfA.x + halfB.x - glm::abs(delta.x),
			halfA.y + halfB.y - glm::abs(delta.y),
			halfA.z + halfB.z - glm::abs(delta.z)
		);

		// No collision if any axis has no overlap
		if (overlap.x <= 0.0f || overlap.y <= 0.0f || overlap.z <= 0.0f)
			return manifold;

		int minAxis = 0;
		if (overlap[1] < overlap[minAxis]) minAxis = 1;
		if (overlap[2] < overlap[minAxis]) minAxis = 2;

		glm::vec3 collisionNormal(0.0f);
		collisionNormal[minAxis] = (delta[minAxis] >= 0.0f) ? 1.0f : -1.0f;

		glm::vec3 overlapMin = glm::max(centerA - halfA, centerB - halfB);
		glm::vec3 overlapMax = glm::min(centerA + halfA, centerB + halfB);

		manifold.bIsColliding = true;
		manifold.normal = collisionNormal;
		manifold.penetrationDepth = overlap[minAxis];
		manifold.contactPoints.push_back((overlapMin + overlapMax) * 0.5f);
	}
	// --- Sphere vs Plane ---
	else if ((myShapeType == ShapeType::Sphere && other.getShapeType() == ShapeType::Plane) ||
			 (myShapeType == ShapeType::Plane && other.getShapeType() == ShapeType::Sphere))
	{
		const ColliderComponent& sphere = (myShapeType == ShapeType::Sphere) ? *this : other;
		const ColliderComponent& plane  = (myShapeType == ShapeType::Plane)  ? *this : other;

		glm::vec3 planeNormal = plane.getShapeData<PlaneShape>().normal;
		float planeOffset     = plane.getShapeData<PlaneShape>().offset;
		glm::vec3 sphereCenter = sphere.getWorldPosition() + sphere.myCenterOfMass;
		float sphereRadius     = sphere.getShapeData<SphereShape>().radius;

		float dist = glm::dot(sphereCenter, planeNormal) - planeOffset;

		if (dist < sphereRadius)
		{
			manifold.bIsColliding = true;
			manifold.normal = (myShapeType == ShapeType::Sphere) ? planeNormal : -planeNormal;
			manifold.penetrationDepth = sphereRadius - dist;
			manifold.contactPoints.push_back(sphereCenter - planeNormal * dist);
		}
	}
	// --- Box vs Plane ---
	else if ((myShapeType == ShapeType::Box && other.getShapeType() == ShapeType::Plane) ||
			 (myShapeType == ShapeType::Plane && other.getShapeType() == ShapeType::Box))
	{
		const ColliderComponent& box   = (myShapeType == ShapeType::Box)   ? *this : other;
		const ColliderComponent& plane  = (myShapeType == ShapeType::Plane) ? *this : other;

		glm::vec3 planeNormal = plane.getShapeData<PlaneShape>().normal;
		float planeOffset     = plane.getShapeData<PlaneShape>().offset;
		glm::vec3 boxCenter   = box.getWorldPosition() + box.myCenterOfMass;
		glm::vec3 halfExtents = box.getShapeData<BoxShape>().halfExtents;

		// Project half-extents onto plane normal to find effective radius
		float projectedRadius = halfExtents.x * glm::abs(planeNormal.x)
								+ halfExtents.y * glm::abs(planeNormal.y)
								+ halfExtents.z * glm::abs(planeNormal.z);

		float dist = glm::dot(boxCenter, planeNormal) - planeOffset;

		if (dist < projectedRadius)
		{
			manifold.bIsColliding = true;
			manifold.normal = (myShapeType == ShapeType::Box) ? planeNormal : -planeNormal;
			manifold.penetrationDepth = projectedRadius - dist;
			manifold.contactPoints.push_back(boxCenter - planeNormal * dist);
		}
	}

	return manifold;
}