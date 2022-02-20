#include <RiverEditor/application.h>

#include <RiverEditor/editor_system.h>

#include <lemon/game.h>
#include <lemon/game/basic_components.h>
#include <lemon/game/system.h>
#include <lemon/input/input.h>
#include <lemon/threads/scheduler.h>

#include <thread>

editor_engine::editor_engine(int argc, char** argv):
    engine(argc, argv)
{
    _scheduler = create_owned<scheduler>(std::thread::hardware_concurrency() - 1);
    _events    = create_owned<event_handler>();
    _resources = create_owned<asset_storage>("path");
    _clock     = create_owned<lemon::clock>();
    _window    = create_owned<editor_window>(1920, 1080);
    rendering_context::create();
    _input        = create_owned<input>(_window.get());
    _sceneManager = create_owned<scene_manager>();
}

editor_engine::~editor_engine()
{
    rendering_context::drop();
}
void editor_engine::initialize()
{
    engine::initialize();
    currentScene = _sceneManager->push_scene(string_id("EditorScene"))
                       ->add_system<editor_system>()
                       ->add_system<rendering_system>();

    auto ent = currentScene->add_entity(string_id("boxEnt"));
    ent.add_component<sprite_renderer>(asset<texture>(string_id("box")));
}
application::application():
    eng(std::make_unique<editor_engine>(0, nullptr)),
    oldViewport(0.f, 0.f)
{
    _window     = (editor_window*)game::get_main_window();
    frameBuffer = std::make_unique<framebuffer>(_window->get_size());
    editedScene = eng->currentScene;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->Fonts->AddFontFromFileTTF((LEMONADE_RESOURCE_PATH + std::string("fonts/Poppins-Regular.ttf")).c_str(), 20.f);
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)_window->get_handle(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();
    set_imgui_style();
}
application::~application()
{
}
void application::loop()
{
    do
    {
        rendering_context::clear_screen({ 1.f, 1.f, 1.f, 1.f });
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("Open project", "CTRL+O")) { }
                if(ImGui::MenuItem("Save", "CTRL+S")) { }
                if(ImGui::MenuItem("Save As", "CTRL+SHIFT+S")) { }
                if(ImGui::MenuItem("Exit", "CTRL+Q")) { }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Edit"))
            {
                if(ImGui::MenuItem("Undo", "CTRL+Z")) { }
                if(ImGui::MenuItem("Redo", "CTRL+Y", false, false)) { }
                ImGui::Separator();
                if(ImGui::MenuItem("Cut", "CTRL+X")) { }
                if(ImGui::MenuItem("Copy", "CTRL+C")) { }
                if(ImGui::MenuItem("Paste", "CTRL+V")) { }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        const ImGuiViewport* vp = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(vp->WorkPos);
        ImGui::SetNextWindowSize(vp->WorkSize);
        ImGui::SetNextWindowViewport(vp->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking
                                       | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
                                       | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                                       | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground
                                       | ImGuiWindowFlags_NoNavFocus;
        ImGui::Begin("MainDockspace", nullptr, windowFlags);
        {
            ImGui::PopStyleVar(3);

            ImGuiID dockspaceID = ImGui::GetID("Dockspace");
            if(ImGui::DockBuilderGetNode(dockspaceID) == NULL)
            {
                ImGui::DockBuilderRemoveNode(dockspaceID);
                ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->Size);

                ImGuiID dock_main_id  = dockspaceID;
                ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(
                    dock_main_id, ImGuiDir_Right, 0.175f, NULL, &dock_main_id);
                ImGuiID dock_right_down_id = ImGui::DockBuilderSplitNode(
                    dock_right_id, ImGuiDir_Down, 0.4f, NULL, &dock_right_id);
                ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(
                    dock_main_id, ImGuiDir_Down, 0.20f, NULL, &dock_main_id);

                ImGui::DockBuilderDockWindow("Hierarchy", dock_right_id);
                ImGui::DockBuilderDockWindow("Properties", dock_right_down_id);
                ImGui::DockBuilderDockWindow("Resources", dock_down_id);
                ImGui::DockBuilderDockWindow("Viewport", dock_main_id);
                ImGui::DockBuilderFinish(dockspaceID);
            }
            ImGui::DockSpace(dockspaceID, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
            ImGui::Begin("Hierarchy");
            draw_hierarchy();
            ImGui::End();
            ImGui::Begin("Properties");
            draw_properties();
            ImGui::End();
            ImGui::Begin("Resources");
            ImGui::End();
            ImGuiWindowFlags viewport_flags =
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground
                | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize;
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Viewport", nullptr, viewport_flags);
            update_viewport();
            ImGui::End();
            ImGui::PopStyleVar();
        }
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    while(!eng->_window->end_frame());
}

void application::update_viewport()
{
    ImGui::BeginChild("vpChild");
    ImVec2 _pos  = ImGui::GetWindowPos();
    ImVec2 _size = ImGui::GetContentRegionAvail();

    bool windowHovered = ImGui::IsWindowHovered();
    bool windowFocused = ImGui::IsWindowFocused();

    if(_size.x != oldViewport.x || _size.y != oldViewport.y)
    {
        disp.send(string_id("FramebufferSize"), (int)_size.x, (int)_size.y);
        frameBuffer->resize({ (int)_size.x, (int)_size.y });
        oldViewport = _size;
    }
    if(windowHovered)
    {
        auto& t = editedScene->get_main_camera()
                      .get_component<transform>();
        if(io->MouseDown[2])
        {
            t.position.x -= io->MouseDelta.x;
            t.position.y -= io->MouseDelta.y;
        }
        if(float wheelDelta = io->MouseWheel; wheelDelta != 0.0f)
        {
            wheelDelta *= 0.1f;
            t.scale.x += wheelDelta;
            t.scale.y += wheelDelta;
        }
    }
    eng->_clock->update();
    frameBuffer->bind();
    eng->_sceneManager->update();
    frameBuffer->unbind();
    rendering_context::set_viewport({ 0.f, 0.f,
                                      _window->get_width(), _window->get_height() });

    ImGui::Image((ImTextureID)frameBuffer->get_texture(), _size,
                 { 0, 0 }, { -1, -1 });
    ImGui::EndChild();
}
void application::draw_hierarchy()
{
    auto& registry = editedScene->get_registry();
    registry.each([&, this](entity_handle ent) {
        auto name = registry.get<tag>(ent);
        ImGuiTreeNodeFlags flags =
            ((this->selection.get_handle() == ent)
                 ? ImGuiTreeNodeFlags_Selected
                 : 0)
            | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;
        bool isOpened = ImGui::TreeNodeEx(
            (void*)(u64(u32(ent))),
            flags, name.id.get_string());
        if(ImGui::IsItemClicked())
            this->selection = entity(&registry, ent);
        if(isOpened)
        {
            ImGui::TreePop();
        }
    });
}
void application::draw_properties()
{
    if(selection.get_handle() != entt::null)
    {
        if(selection.has_component<transform>())
        {
            auto& t = selection.get_component<transform>();
            ImGui::Separator();
            if(ImGui::TreeNodeEx("transform"))
            {
                ImGui::TreePop();
            }
        }
    }
}
void application::set_imgui_style()
{
    ImGuiStyle& style = ImGui::GetStyle();

    ImVec4 lemonYellow{ 0.988f, 1.f, 0.0f, 1.f };
    ImVec4 lemonYellowL(0.992f, 1.f, 0.38f, 1.f);
    ImVec4 lemonGreen{ 0.655f, 1.f, 0.f, 1.f };
    ImVec4 lemonGreenL{ 0.722f, 1.f, 0.192f, 1.f };
    ImVec4 lemonGreenD{ 0.62f, 0.949f, 0.f, 1.f };
    ImVec4 lemonPink{ 1.f, 0.f, 0.545f, 1.f };
    ImVec4 lemonPinkL{ 0.996f, 0.38f, 0.71f, 1.f };
    ImVec4 lemonPinkD{ 0.863f, 0.f, 0.471f, 1.f };
    ImVec4 lemonPurple{ 0.678f, 0.051f, 1.f, 1.f };
    ImVec4 lemonPurpleL{ 0.796f, 0.408f, 0.992f, 1.f };
    ImVec4 lemonPurpleD{ 0.502f, 0.f, 0.761f, 1.f };
    ImVec4 lemonGray{ 0.21f, 0.25f, 0.27f, 1.f };
    ImVec4 lemonGray2{ 0.29f, 0.37f, 0.43f, 1.f };
    ImVec4 lemonBlack{ 0.14f, 0.13f, 0.12f, 1.f };
    ImVec4 lemonBlack2{ 0.1f, 0.09f, 0.09f, 1.f };
    ImVec4 lemonBlack3{ 0.3f, 0.28f, 0.25f, 1.f };
    ImVec4 lemonBlack4{ 0.22f, 0.21f, 0.19f, 1.f };

    style.WindowRounding = 0.0f;
    style.FramePadding   = ImVec2(0.f, 0.f);

    style.Colors[ImGuiCol_Text]                  = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = lemonBlack;
    style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
    style.Colors[ImGuiCol_Border]                = lemonGray;
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = lemonBlack2;
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
    style.Colors[ImGuiCol_TitleBg]               = lemonBlack2;
    style.Colors[ImGuiCol_TitleBgCollapsed]      = lemonBlack2;
    style.Colors[ImGuiCol_TitleBgActive]         = lemonBlack4;
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_Header]                = lemonGreen;
    style.Colors[ImGuiCol_HeaderHovered]         = lemonGreenL;
    style.Colors[ImGuiCol_HeaderActive]          = lemonGreenD;
    style.Colors[ImGuiCol_Tab]                   = lemonYellow;
    style.Colors[ImGuiCol_TabHovered]            = lemonBlack;
    style.Colors[ImGuiCol_TabActive]             = lemonBlack4;
    style.Colors[ImGuiCol_TabUnfocused]          = lemonBlack2;
    style.Colors[ImGuiCol_TabUnfocusedActive]    = lemonBlack2;
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
    style.Colors[ImGuiCol_NavHighlight]          = lemonYellow;
    style.Colors[ImGuiCol_NavWindowingHighlight] = lemonYellowL;
    style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}