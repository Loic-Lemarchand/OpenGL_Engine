#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "world.h"


class UI
{
public:
	UI();
	~UI();

	
	void DrawSceneGraphWindow(World& world);

private:
	void DrawSceneComponentTree(const std::shared_ptr<SceneComponent>& comp, int depth);

};


