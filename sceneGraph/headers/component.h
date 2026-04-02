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
	SceneComponent() : myPosition(0.0f), myScale(1.0f), myRotation(0.0f), myRotationAxis(0.0f) { bCanRecurseTick = true; }
	SceneComponent(glm::vec3 position) : myPosition(position), myScale(1.0f), myRotation(0.0f), myRotationAxis(0.0f) { bCanRecurseTick = true; }

	glm::vec3 getPosition() const { return myPosition; }
	float getRotation() const { return myRotation; }
	glm::vec3 getScale() const { return myScale; }
	glm::vec3 getRotationAxis() const { return myRotationAxis; }

	std::shared_ptr<SceneComponent> getParent() const { return myParent; }
	
	void setPosition(glm::vec3 position) { myPosition = position; }
	void setRotation(float rotation) { myRotation = rotation; }
	void SetScale(glm::vec3 scale) { myScale = scale; }
	void SetRotationAxis(glm::vec3 rotationAxis) { myRotationAxis = rotationAxis; }

	void setParent(std::shared_ptr<SceneComponent> parent) { myParent = parent; }

	glm::mat4 getWorldTransform() const;

	void AddChild(std::shared_ptr<SceneComponent> child);
	std::vector<std::shared_ptr<SceneComponent>> getChildrenComps() const { return myChildren; }

	virtual void Tick() override;

	virtual bool getRenderProxy(RenderProxy& proxy) const { return false; }

protected:
	glm::vec3 myPosition;
	glm::vec3 myScale;
	float myRotation;
	glm::vec3 myRotationAxis;

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
