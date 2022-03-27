#include <lemon/engine/systems/debug_system.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <lemon/core/game.h>
#include <lemon/platform/window.h>

#include <lemon/scene/components/physics_components.h>
#include <lemon/scene/components/transform_components.h>

#include <GLFW/glfw3.h>

namespace lemon {

debug_system::debug_system(ptr<scene> s):
    enabled(false), showColliders(false),
    showFPS(false)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)game::get_main_window()->get_handle(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    window::onKeyPressed.register_observer(
        [this](event_args* a) {
            auto s = (KeyPressed*)a;
            if(s->keycode == key::keycode::f2
               && s->action == key::action::down)
            {
                this->enabled ^= 1;
            }
        },
        string_id("debug_system::onKeyPressed"));
}
debug_system::~debug_system()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void debug_system::update(entity_registry& registry)
{
    if(enabled)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Debug");
        {
            ImGui::Checkbox("show colliders", &showColliders);
            ImGui::Checkbox("show fps", &showFPS);
        }
        ImGui::End();
        if(showColliders)
        {
            auto windowSize = ImGui::GetWindowSize();
            auto back       = ImGui::GetBackgroundDrawList();
            ImU32 color     = 0xFF0000FF;
            f32 thickness   = 1.f;
            ImGui::Begin("Wind");
            for(auto&& [ent, coll, tr] : registry.view<collider, transform>().each())
            {
                vec2 center      = tr.position + coll.offset;
                vec2 debugCenter = { windowSize.x * 0.5 + center.x,
                                     windowSize.y * 0.5 - center.y };
                ImVec2 ld(debugCenter.x - coll.box.hSize.x, debugCenter.y - coll.box.hSize.y);
                ImVec2 rd(debugCenter.x + coll.box.hSize.x, debugCenter.y - coll.box.hSize.y);
                ImVec2 ru(debugCenter.x + coll.box.hSize.x, debugCenter.y + coll.box.hSize.y);
                ImVec2 lu(debugCenter.x - coll.box.hSize.x, debugCenter.y + coll.box.hSize.y);
                back->AddQuad(ld, rd, ru, lu, color, thickness);
            }
            ImGui::End();
        }
        if(showFPS)
        {
            ImGui::Begin("FPS");
            {
                ImGui::Text("%.0f", ImGui::GetIO().Framerate);
            }
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
} // namespace lemon
