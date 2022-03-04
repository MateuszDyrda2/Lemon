#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <lemon/renderer/rendering_context.h>

editor_window::editor_window(size_type width, size_type height) :
    window_base(width, height)
{

}
editor_window::~editor_window()
{
}
bool editor_window::end_frame()
{
    return false;
}
void* editor_window::get_handle()
{
    return nullptr;
}

editor_engine::editor_engine(const std::string& workingDirectory, int argc, char** argv) :
    engine(workingDirectory, argc, argv)
{
    _events = create_owned<event_handler>();
    _clock = create_owned<lemon::clock>();
    _window = create_owned<editor_window>(1920, 1080);
    rendering_context::create();
    _resources = create_owned<asset_storage>();
    _sceneManager = create_owned<scene_manager>();
    this->initialize();
}
editor_engine::~editor_engine(){

}
void editor_engine::initialize(){
   engine::initialize();
  }
bool editor_engine::update(){
    _sceneManager->update();
    return true;
}
void editor_engine::drop(){
    engine::drop();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent},
      ui(new Ui::MainWindow),
      _engine(new editor_engine("/home/matzix/shared/Lemon/example/lemon.json", 0, nullptr))
{
    ui->setupUi(this);
}
MainWindow::~MainWindow(){
    delete ui;
    if(_engine) delete _engine;
}
