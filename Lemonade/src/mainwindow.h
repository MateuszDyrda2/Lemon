#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <lemon/engine.h>
#include <lemon/events/dispatcher.h>

namespace Ui{class MainWindow;}

using namespace lemon;
class editor_window : public window_base
{
public:
    editor_window(size_type width, size_type height);
    ~editor_window();
    bool end_frame() override;
    void* get_handle() override;
};

class editor_engine : public engine
{
public:
    editor_engine(const std::string& workingDirectory, int argc, char** argv);
    virtual ~editor_engine();
    void initialize();
    bool update();
    void drop();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:

private slots:

private:
    Ui::MainWindow* ui;
    editor_engine* _engine;
    dispatcher _dispatcher;
};

#endif // MAINWINDOW_H
