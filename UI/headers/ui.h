#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "world.h"


class UI
{
public:
	UI(World& world, GLFWwindow* window);
	~UI();

	
	void DrawSceneGraphWindow(World& world);
	void update();

private:
	void DrawSceneComponentTree(const std::shared_ptr<SceneComponent>& comp, int depth);

	World& myWorld;

};


