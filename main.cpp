#include "window.h"
#include "utilities.h"
#include "renderer.h"
#include <memory>
#include "inputManager.h"
#include "camera.h"
#include "world.h"
#include "physicsComponent.h"


class ATown : public Actor
{
public:
	ATown(std::shared_ptr<Shader> shader)
	{
		auto meshComponent = addSceneComponent<MeshComponent>();
		meshComponent->loadModelFromFile(PROJECT_ROOT_DIR"/Assets/k3/source/mistitown.fbx", shader);
		setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		SetScale(glm::vec3(0.001f));

		auto rigidBody = addActorComponent<RigidBodyComponent>(bodyType::Static, 0.0);
		auto collider = std::make_shared<ColliderComponent>(glm::vec3(0.0f), ShapeType::Plane, PlaneShape{ glm::vec3(0.0f, 1.0f, 0.0f), 0.0f });
		rigidBody->setColliderComponent(collider);

		World::getWorld().getPhysicsWorld().registerRigidBody(rigidBody, getRootComponent());
	};
	~ATown(){}

private:
};

class ACrate : public Actor
{
public:
	ACrate(std::shared_ptr<Shader> shader)
	{
		auto meshComponent = addSceneComponent<MeshComponent>();
		meshComponent->loadModelFromFile(PROJECT_ROOT_DIR"/Assets/woodenCrate/Wooden_Crate.fbx", shader);
		setPosition(glm::vec3(0.0f, 10.0f, 0.0f));
		SetScale(glm::vec3(0.001f));

		auto rigidBody = addActorComponent<RigidBodyComponent>(bodyType::Dynamic, 1.0);
		auto collider = std::make_shared<ColliderComponent>(glm::vec3(0.0f, 10.0f, 0.0f), ShapeType::Box, BoxShape{ glm::vec3(0.5f) });
		rigidBody->setColliderComponent(collider);

		World::getWorld().getPhysicsWorld().registerRigidBody(rigidBody, getRootComponent());
	};
	~ACrate(){}

private:
};

int main(int argc, char* argv[])
{

	
	std::shared_ptr<EventDispatcher::EventBus> eventBus = std::make_shared<EventDispatcher::EventBus>();
	World::setEventBus(eventBus);
	std::unique_ptr<InputManager> inputManager = std::make_unique<InputManager>(*eventBus);
	std::unique_ptr<Window> window = std::make_unique<Window>(1920, 1080, "My Window", *inputManager);
	World& world = World::getWorld();

	std::shared_ptr<ACamera> cameraActor = world.spawnActor<ACamera>();
	

	if (!window->bIsValid)
	{
		utilities::log("Window is not valid");
		return -1;
	}

	auto renderer = std::make_unique<Renderer>(window->getFrameBufferWidth(), window->getFrameBufferHeight(), cameraActor->getCameraComponent());

	if (!renderer->bIsValid)
	{
		utilities::log("Renderer is not valid");
		return -1;
	}

	world.spawnActor<ATown>(renderer->getShader());
	world.spawnActor<ACrate>(renderer->getShader());


	while (!window->shoudClose() && window->bIsValid)
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		window->update();

		//EVENT BUS POLLING
		eventBus->dispatchEvents();

		world.Tick();

		//RENDERER WORK HERE
		renderer->update();
		//renderer->update(glm::vec3(0.0f, 0.0f, -2.5f), 45, glm::vec3(0.0f, window->triOffset, 0.0f), glm::vec3(0.4f, 0.4f, 1.0f));

		//PHYSICS COMPUTATION HERE
		
	}

	return 0;

}