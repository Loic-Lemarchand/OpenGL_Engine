#include "ui.h"
#include "actor.h"
#include "GLFW/glfw3.h"

UI::UI(World& world, GLFWwindow* window) :
	myWorld(world)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();
}

UI::~UI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UI::DrawSceneComponentTree(const std::shared_ptr<SceneComponent>& comp, int depth = 0) {
	if (!comp) return;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
	if (comp->getChildrenComps().empty())
		flags |= ImGuiTreeNodeFlags_Leaf;

	// Affiche le type via typeid (nom démanglé simplifié)
	const char* typeName = typeid(*comp).name();
	glm::vec3 pos = comp->getPosition();

	bool open = ImGui::TreeNodeEx(
		(void*)(intptr_t)depth,
		flags,
		"%s  (%.1f, %.1f, %.1f)",
		typeName, pos.x, pos.y, pos.z
	);

	if (open)
	{
		for (const auto& child : comp->getChildrenComps())
			DrawSceneComponentTree(child, depth + 1);
		ImGui::TreePop();
	}
}

void UI::DrawSceneGraphWindow(World& world)
{
	ImGui::Begin("Scene Graph");
	auto& actors = world.getActors();
	for (size_t i = 0; i < actors.size(); ++i)
	{
		auto& actor = actors[i];
		const char* actorType = typeid(*actor).name();

		if (ImGui::TreeNodeEx((void*)(intptr_t)i, ImGuiTreeNodeFlags_DefaultOpen, "%s", actorType))
		{
			if (actor->getRootComponent())
				DrawSceneComponentTree(actor->getRootComponent());
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void UI::update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	DrawSceneGraphWindow(myWorld);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
