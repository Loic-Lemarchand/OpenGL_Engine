#include "component.h"
#include "model.h"
#include "shader.h"

//Component

Component::Component() : 
	bIsActive(false),
	bIsValid(true),
	myOwner(nullptr),
	bCanRecurseTick(false)
{
}

Component::~Component()
{
}

void Component::Tick()
{
}

//SceneComponent

void SceneComponent::Tick()
{
	Component::Tick();
	for (auto& child : myChildren)
	{
		if (child && child->isActive())
			child->Tick();
	}
}

glm::mat4 SceneComponent::getWorldTransform() const
{
	glm::mat4 local = glm::mat4(1.0f);
	local = glm::translate(local, myLocalPosition);
	if (myLocalRotation != 0.0f && glm::length(myLocalRotationAxis) > 0.0f)
	{
		local = glm::rotate(local, glm::radians(myLocalRotation), myLocalRotationAxis);
	}
	local = glm::scale(local, myLocalScale);

	if (myParent)
	{
		return myParent->getWorldTransform() * local;
	}
	return local;
}

glm::vec3 SceneComponent::getWorldPosition() const
{
	return glm::vec3(getWorldTransform()[3]);
}

glm::vec3 SceneComponent::getWorldScale() const
{
	glm::mat4 world = getWorldTransform();
	float scalex = glm::length(glm::vec3(world[0]));
	float scaley = glm::length(glm::vec3(world[1]));
	float scalez = glm::length(glm::vec3(world[2]));
	return glm::vec3(scalex, scaley, scalez);
}

void SceneComponent::setWorldPosition(glm::vec3 WorldPos)
{
	if (myParent)
	{
		glm::mat4 parentWorldInv = glm::inverse(myParent->getWorldTransform());
		myLocalPosition = glm::vec3(parentWorldInv * glm::vec4(WorldPos, 1.0f));
	}
	else
	{
		myLocalPosition = WorldPos;
	}
}

void SceneComponent::AddChild(std::shared_ptr<SceneComponent> child)
{
	child->setOwner(myOwner);
	child->setParent(shared_from_this());
	myChildren.push_back(child);
}

//MeshComponent

MeshComponent::MeshComponent()
{
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::loadModelFromFile(const std::string& modelPath, std::shared_ptr<Shader> shader)
{
	myModel = std::make_unique<Model>();
	myModel->Load(modelPath, shader);
	myModel->position = myLocalPosition;
	myModel->scale = myLocalScale;
	myModel->rotation = myLocalRotation;
	myModel->rotationAxis = myLocalRotationAxis;
	bIsActive = true;
}

void MeshComponent::setModel(Model* model)
{
	myModel.reset(model);
}

bool MeshComponent::getRenderProxy(RenderProxy& proxy) const
{
	if (!myModel || !bIsActive)
		return false;
	proxy.modelMat = getWorldTransform();
	proxy.model = myModel;
	return true;
}