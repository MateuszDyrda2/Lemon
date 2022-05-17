#include <lemon/engine/systems/debug_system.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <lemon/core/game.h>
#include <lemon/platform/window.h>

#include <lemon/core/instrumentor.h>
#include <lemon/physics/components/physics_components.h>
#include <lemon/scene/components/transform_components.h>

#include <GLFW/glfw3.h>

namespace lemon {

debug_system::debug_system(service_registry& globalRegistry):
    system(globalRegistry),
    enabled(false),
    showColliders(false),
    showFPS(false), ebus(globalRegistry.get_service<event_bus>())
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)game::get_main_window()->get_handle(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ebus.sink(string_id("KeyPressed")) += this;
}
debug_system::~debug_system()
{
    ebus.sink(string_id("KeyPressed")) -= this;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void debug_system::on_event(event* e)
{
    auto s = static_cast<KeyPressed*>(e);
    if(s->keycode == key::keycode::f2
       && s->action == key::action::down)
    {
        this->enabled ^= 1;
    }
}
void debug_system::on_update(entity_registry& registry)
{
    LEMON_PROFILE_FUNCTION();
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
            auto mainCamera          = registry.view<main_camera_t>().front();
            auto&& [cCamera, cModel] = registry.get<camera, model>(mainCamera);

            auto windowSize        = game::get_main_window()->get_size();
            auto back              = ImGui::GetBackgroundDrawList();
            ImU32 color            = 0xFF00FF00;
            f32 thickness          = 1.5f;
            const f32 inResolution = windowSize.y / f32(windowSize.x);
            //            vec2 scaler          = { windowSize.x / (resolution * cCamera.size), windowSize.y / cCamera.size };
            vec2 scaler = { inResolution * windowSize.x / cCamera.size,
                            windowSize.y / cCamera.size };

            registry.group<transform, collider>().each([&, this](auto, const auto& tr, const auto& coll) {
                vec2 center      = (tr.position + coll.offset) * scaler;
                vec2 debugCenter = { windowSize.x * 0.5 + center.x,
                                     windowSize.y * 0.5 - center.y };
                vec2 boxSize     = coll.box.hSize * scaler;
                ImGui::Text("%fx%f", scaler.x, scaler.y);
                ImVec2 ld(debugCenter.x - boxSize.x, debugCenter.y - boxSize.y);
                ImVec2 rd(debugCenter.x + boxSize.x, debugCenter.y - boxSize.y);
                ImVec2 ru(debugCenter.x + boxSize.x, debugCenter.y + boxSize.y);
                ImVec2 lu(debugCenter.x - boxSize.x, debugCenter.y + boxSize.y);
                back->AddQuad(ld, rd, ru, lu, color, thickness);
                // back->AddRect(ld, ru, color, thickness);
            });
        }
        if(showFPS)
        {
            ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
            ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
            {
                ImGui::SetWindowFontScale(2.f);
                ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "%.2f", ImGui::GetIO().Framerate);
            }
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
} // namespace lemon
