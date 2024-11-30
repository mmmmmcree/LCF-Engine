#include "Window.h"
#include "GLWidget.h"
#include "ControlWidget.h"

Window::Window(QWidget* parent)
    : ElaWindow(parent)
{
    this->setNavigationBarDisplayMode(ElaNavigationType::Compact);
    this->setWindowButtonFlag(ElaAppBarType::ThemeChangeButtonHint, false);
    this->setIsDefaultClosed(false);
    GLWidget *gl_widget = new GLWidget(this);
    this->addPageNode("GLWidget", gl_widget, ElaIconType::PaintbrushPencil);
    ControlWidget *control_widget = new ControlWidget(nullptr);
    control_widget->hide();
    QString control_key("");
    this->addFooterNode("ControlWidget", nullptr, control_key, 0, ElaIconType::GearComplex);
    connect(this, &ElaWindow::navigationNodeClicked, this, [=](ElaNavigationType::NavigationNodeType node_type, QString node_key) {
        if (node_key == control_key) {
            control_widget->move(0, 100);
            control_widget->showNormal();
        }
    });
    connect(this, &ElaWindow::closeButtonClicked, this, [=] {
        control_widget->setParent(this);
        this->closeWindow();
    });
}

Window::~Window()
{
}