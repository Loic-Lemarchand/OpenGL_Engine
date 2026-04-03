#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>
#include <string>
#include <vector>

class Actor;
class Model;
class Shader;

struct RenderProxy
{
	glm::mat4 modelMat;
	std::shared_ptr<Model> model;
};

class Component
{
public:
	Component();
	~Component();

	Actor* getOwner() { return myOwner; }
	void setOwner(Actor* owner) { myOwner = owner; }

	bool isActive() { return bIsActive; }
	bool isValid() { return bIsValid;  }
	bool canRecurseTick() { return bCanRecurseTick; }
	

	void setActive(bool activation) { bIsActive = activation; }

	virtual void Tick();
protected:
	
	Actor* myOwner;

	bool bIsActive;
	bool bIsValid;


	bool bCanRecurseTick;
};

class ActorComponent : public Component
{

};

class SceneComponent : public ActorComponent, public std::enable_shared_from_this<SceneComponent>
{
public:
	SceneComponent() : myLocalPosition(0.0f), myLocalScale(1.0f), myLocalRotation(0.0f), myLocalRotationAxis(0.0f) { bCanRecurseTick = true; }
	SceneComponent(glm::vec3 position) : myLocalPosition(position), myLocalScale(1.0f), myLocalRotation(0.0f), myLocalRotationAxis(0.0f) { bCanRecurseTick = true; }

	glm::vec3 getLocalPosition() const { return myLocalPosition; }
	float getLocalRotation() const { return myLocalRotation; }
	glm::vec3 getLocalScale() const { return myLocalScale; }
	glm::vec3 getLocalRotationAxis() const { return myLocalRotationAxis; }

	std::shared_ptr<SceneComponent> getParent() const { return myParent; }
	
	void setLocalPosition(glm::vec3 position) { myLocalPosition = position; }
	void setLocalRotation(float rotation) { myLocalRotation = rotation; }
	void SetLocalScale(glm::vec3 scale) { myLocalScale = scale; }
	void SetLocalRotationAxis(glm::vec3 rotationAxis) { myLocalRotationAxis = rotationAxis; }

	glm::mat4 getWorldTransform() const;
	glm::vec3 getWorldPosition() const;
	glm::vec3 getWorldScale() const;

	void setWorldPosition(glm::vec3 WorldPos);

	void setParent(std::shared_ptr<SceneComponent> parent) { myParent = parent; }
	

	void AddChild(std::shared_ptr<SceneComponent> child);
	std::vector<std::shared_ptr<SceneComponent>> getChildrenComps() const { return myChildren; }

	virtual void Tick() override;

	virtual bool getRenderProxy(RenderProxy& proxy) const { return false; }

protected:
	glm::vec3 myLocalPosition;
	glm::vec3 myLocalScale;
	float myLocalRotation;
	glm::vec3 myLocalRotationAxis;

	std::shared_ptr<SceneComponent> myParent;

	std::vector<std::shared_ptr<SceneComponent>> myChildren;
};

class MeshComponent : public SceneComponent
{
public:
	MeshComponent();
	~MeshComponent();

	void loadModelFromFile(const std::string& modelPath, std::shared_ptr<Shader> shader);
	void setModel(Model* model);

	std::shared_ptr<Model> getModel() const { return myModel; }

	virtual bool getRenderProxy(RenderProxy& proxy) const override;

private:
	
	std::shared_ptr<Model> myModel;
};
