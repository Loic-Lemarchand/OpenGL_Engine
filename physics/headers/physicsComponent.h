#pragma once

#include "component.h" 

#include <variant>

class ColliderComponent;

enum bodyType
{
	Static,
	Dynamic,
	Sleeping
};

struct AABB
{
	glm::vec3 min;
	glm::vec3 max;

	bool overlaps(const AABB& other)
	{
		return (
			((max.x >= other.min.x) && (min.x <= other.max.x))
			&&
			((max.y >= other.min.y) && (min.y <= other.max.y))
			&&
			((max.z >= other.min.z) && (min.z <= other.max.z))
			);
	}
};

struct CollisionManyFold
{
	bool bIsColliding;

	glm::vec3 normal;
	float penetrationDepth;

	std::vector<glm::vec3> contactPoints;
};

class RigidBodyComponent : public ActorComponent
{
public:
	RigidBodyComponent(bodyType bodyType, double mass);
	~RigidBodyComponent();

	bodyType getBodyType() { return myBodyType; }
	glm::vec3 getAccumulatedForces() { return myResultingForce; }
	float getMass() { return myMass; }
	float getMassInverse() { return myBodyType != bodyType::Dynamic ? 0.0f : myMassInverse; }
	glm::vec3 getSpeed() { return mySpeed; }
	float getLinearDamping() { return myLinearDamping; }
	std::shared_ptr<ColliderComponent> getColliderComponent() { return myColliderComponent; }


	void setBodyType(const bodyType& bodyType) { myBodyType = bodyType; }

	void AddForce(const glm::vec3& force) { myResultingForce += force; }
	void AddForces(const std::vector<glm::vec3>& forces);
	void setMass(const double& mass) { myMass = mass; }
	void setMassInverse(const double& massInverse) { myMassInverse = massInverse; }
	void setSpeed(const glm::vec3& speed) { mySpeed = speed; }
	void setLinearDamping(const float& linearDamping) { myLinearDamping = linearDamping; }
	void setColliderComponent(std::shared_ptr<ColliderComponent> colliderComponent) { myColliderComponent = colliderComponent; }

	void ClearForces() { myResultingForce = glm::vec3(0.0f); };
	void CalculateAcceleration();
	void Integrate(float duration, std::shared_ptr<SceneComponent> sceneComponent);

	RigidBodyComponent& operator+=(const glm::vec3& force) {
		myResultingForce += force;
		return *this;
	}

private:
	std::shared_ptr<ColliderComponent> myColliderComponent;

	float myMass;
	float myMassInverse;

	glm::vec3 mySpeed;

	glm::vec3 myResultingForce;

	bodyType myBodyType;

	glm::vec3 myAcceleration;
	glm::vec3 myLastFrameAcceleration;

	float myLinearDamping;
};

enum ShapeType
{
	Box,
	Sphere,
	Plane
};

struct BoxShape
{
	glm::vec3 halfExtents;
};

struct SphereShape
{
	float radius;
};

struct PlaneShape
{
	glm::vec3 normal;
	float offset;
};

class ColliderComponent : public SceneComponent
{
public:
	using ShapeData = std::variant<BoxShape, SphereShape, PlaneShape>;

	ColliderComponent(glm::vec3 position, ShapeType shapeType, ShapeData shapeData);
	~ColliderComponent();

	template<typename T>
	const T& getShapeData() const { return std::get<T>(myShapeData); }

	glm::vec3 getCenterOfMass() const { return myCenterOfMass; }
	ShapeType getShapeType() const { return myShapeType; }

	void setCenterOfMass(const glm::vec3& centerOfMass) { myCenterOfMass = centerOfMass; }
	void setShapeType(ShapeType shapeType) { myShapeType = shapeType; }

	void setShape(BoxShape box) { myShapeData = box; myShapeType = ShapeType::Box; }
	void setShape(SphereShape sphere) { myShapeData = sphere; myShapeType = ShapeType::Sphere; }
	void setShape(PlaneShape plane) { myShapeData = plane; myShapeType = ShapeType::Plane; }

	//Not the real cente of mass yet
	void calculateCenterOfMassFromMesh(const MeshComponent& meshComponent);

	AABB getAABB() const;
	CollisionManyFold  checkCollision(const ColliderComponent& other);

private:
	glm::vec3 myCenterOfMass;

	ShapeType myShapeType;
	ShapeData myShapeData;

};