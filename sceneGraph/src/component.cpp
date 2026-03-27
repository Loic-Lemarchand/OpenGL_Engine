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
	local = glm::translate(local, myPosition);
	if (myRotation != 0.0f && glm::length(myRotationAxis) > 0.0f)
	{
		local = glm::rotate(local, glm::radians(myRotation), myRotationAxis);
	}
	local = glm::scale(local, myScale);

	if (myParent)
	{
		return myParent->getWorldTransform() * local;
	}
	return local;
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
	myModel->position = myPosition;
	myModel->scale = myScale;
	myModel->rotation = myRotation;
	myModel->rotationAxis = myRotationAxis;
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