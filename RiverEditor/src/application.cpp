#include <RiverEditor/application.h>

#include <river/input/input.h>

editor_engine::editor_engine(int argc, char** argv):
    engine(argc, argv)
{
    services::provide(create_owned<event_handler>());
    services::provide(create_owned<resource_manager>());
    services::provide<window_base>(create_owned<editor_window>(1920, 1080));
    services::provide(create_owned<input>());
    services::provide(create_owned<rendering_context>());
    services::provide(create_owned<scene_manager>());
    services::provide(create_owned<river::clock>());
    this->initialize();
}
editor_engine::~editor_engine()
{
}
void editor_engine::initialize()
{
    engine::initialize();
    _sceneManager->push_scene(string_id("EditorScene"));
}
application::application():
    eng(std::make_unique<editor_engine>(0, nullptr)),
    oldViewport(0.f, 0.f)
{
    _window     = (editor_window*)services::get<window_base>();
    frameBuffer = std::make_unique<framebuffer>(_window->get_size());
    handler     = services::get<event_handler>();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO* io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)_window->get_handle(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();
}
application::~application()
{
}
void application::loop()
{
    do
    {
        eng->_context->clear_screen({ 1.f, 1.f, 1.f, 1.f });

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
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
                                       | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
                                       | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                                       | ImGuiWindowFlags_NoBringToFrontOnFocus
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

                ImGuiID dock_main_id = dockspaceID;
                ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(
                    dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);
                ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(
                    dock_main_id, ImGuiDir_Right, 0.25f, NULL, &dock_main_id);
                ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(
                    dock_main_id, ImGuiDir_Down, 0.25f, NULL, &dock_main_id);

                ImGui::DockBuilderDockWindow("SceneView", dock_left_id);
                ImGui::DockBuilderDockWindow("Properties", dock_right_id);
                ImGui::DockBuilderDockWindow("Resources", dock_down_id);
                ImGui::DockBuilderDockWindow("Viewport", dock_main_id);
                ImGui::DockBuilderFinish(dockspaceID);
            }
            ImGui::DockSpace(dockspaceID, ImVec2(0, 0));
            ImGui::Begin("SceneView");
            ImGui::End();
            ImGui::Begin("Properties");
            ImGui::End();
            ImGui::Begin("Resources");
            ImGui::End();
            ImGuiWindowFlags viewport_flags =
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground
                | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize;

            ImGui::Begin("Viewport", nullptr, viewport_flags);
            {
                ImGui::BeginChild("vpChild");
                ImVec2 _pos  = ImGui::GetWindowPos();
                ImVec2 _size = ImGui::GetContentRegionAvail();
                if(_size.x != oldViewport.x || _size.y != oldViewport.y)
                {
                    handler->dispatch<int, int>(
                        string_id("FramebufferSize"), (int)_size.x, (int)_size.y);
                    frameBuffer->resize({ (int)_size.x, (int)_size.y });
                    oldViewport = _size;
                }
                eng->_clock->update();
                frameBuffer->bind();
                eng->_sceneManager->update(eng->_clock->read_delta_time());
                frameBuffer->unbind();
                eng->_context->set_viewport({ 0.f, 0.f,
                                              _window->get_width(), _window->get_height() });

                ImGui::Image((ImTextureID)frameBuffer->get_texture(), _size);
                ImGui::EndChild();
            }
            ImGui::End();
        }
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    while(!eng->_window->end_frame());
}
