#include "Window.h"
#include "GLWidget.h"
#include "ControlWidget.h"
#include "ElaMenuBar.h"
#include <QVBoxLayout>

Window::Window(QWidget* parent)
    : ElaWindow(parent)
{
    QWidget* custom_widget = new QWidget(this);
    QVBoxLayout* custom_layout = new QVBoxLayout(custom_widget);
    ElaMenuBar* menu_bar = new ElaMenuBar(this);
    menu_bar->setFixedHeight(27);
    custom_layout->setContentsMargins(1, 1, 1, 1);
    custom_layout->addWidget(menu_bar);
    custom_layout->addStretch();
    this->setCustomWidget(ElaAppBarType::LeftArea, custom_widget);

    this->setIsNavigationBarEnable(false);
    this->setWindowButtonFlag(ElaAppBarType::ThemeChangeButtonHint, false);
    this->setIsDefaultClosed(false);

    GLWidget *gl_widget = new GLWidget(this);
    this->addPageNode("GLWidget", gl_widget, ElaIconType::PaintbrushPencil);
    
    ControlWidget *control_widget = new ControlWidget(nullptr);
    control_widget->hide();

    auto action = menu_bar->addElaIconAction(ElaIconType::GearComplex, "");
    connect(action, &QAction::triggered, this, [=] {
        control_widget->move(0, 100);
        control_widget->showNormal();
    });
    connect(this, &ElaWindow::closeButtonClicked, this, [=] {
        control_widget->setParent(this);
        this->closeWindow();
    });
}

Window::~Window()
{
}