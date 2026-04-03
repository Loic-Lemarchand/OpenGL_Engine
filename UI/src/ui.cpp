#include "ui.h"
#include "actor.h"

UI::UI()
{

}

UI::~UI()
{

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