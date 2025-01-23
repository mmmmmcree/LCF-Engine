#include "ControlWidget.h"
#include "ElaNavigationBar.h"
#include <QLayout>
#include <QPlainTextEdit>
#include <QMessageBox>
#include "ElaMenuBar.h"
#include "CameraControlPanel.h"
#include "ModelControlPanel.h"
#include "RenderSettingsPanel.h"

ControlWidget::ControlWidget(QWidget *parent) : ElaWindow(parent)
{
    this->setWindowButtonFlags(ElaAppBarType::StayTopButtonHint | ElaAppBarType::CloseButtonHint | ElaAppBarType::ThemeChangeButtonHint);
    this->setNavigationBarDisplayMode(ElaNavigationType::Compact);
    this->setUserInfoCardVisible(false);

    ModelControlPanel *model_loader_panel = new ModelControlPanel(this);
    this->addPageNode("Model Control Panel", model_loader_panel, ElaIconType::Dragon);
    RenderSettingsPanel *render_settings_panel = new RenderSettingsPanel(this);
    this->addPageNode("Render Settings Panel", render_settings_panel, ElaIconType::PresentationScreen);
    CameraControlPanel *camera_control_panel = new CameraControlPanel(this);
    this->addPageNode("Camera Control Panel", camera_control_panel, ElaIconType::CameraMovie);

    this->setMinimumWidth(720);
    this->resize(720, 480);
    this->setMinimumHeight(20);
    ElaMenuBar* menu_bar = new ElaMenuBar(this);
    connect(menu_bar->addElaIconAction(ElaIconType::WindowRestore, ""), &QAction::triggered, this, [this] {
        static bool is_minimized = false;
        is_minimized = !is_minimized;
        this->resize(this->width(), is_minimized? 20 : 480);
    });
    QWidget* custom_widget = new QWidget(this);
    custom_widget->setFixedWidth(100);
    QHBoxLayout* customLayout = new QHBoxLayout(custom_widget);
    customLayout->setContentsMargins(0, 0, 0, 18);
    customLayout->addWidget(menu_bar);
    customLayout->addStretch();
    this->setCustomWidget(ElaAppBarType::LeftArea, custom_widget);
}
