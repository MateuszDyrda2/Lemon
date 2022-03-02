#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <lemon/events/dispatcher.h>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

  public:
    explicit OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();

  protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void resizeEvent(QResizeEvent* event) override;
private:
    lemon::dispatcher _dispatcher;
};

#endif // OPENGLWIDGET_H
