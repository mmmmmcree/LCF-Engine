#include "CameraControlPanel.h"
#include "ElaScrollPageArea.h"
#include <QLayout>
#include "ElaComboBox.h"
#include "ElaToggleButton.h"
#include "ElaText.h"
#include "ElaCheckBox.h"
#include "GlobalCamera.h"

CameraControlPanel::CameraControlPanel(QWidget *parent) : ElaScrollPage(parent)
{
    ElaScrollPageArea *button_area = new ElaScrollPageArea(this);
    ElaText *text = new ElaText("Configuration Mode", this);
    text->setWordWrap(false);
    text->setTextPixelSize(13);
    ElaToggleButton *toggle_btn = new ElaToggleButton(this);
    toggle_btn->setFixedWidth(200);
    QHBoxLayout *hlayout = new QHBoxLayout(button_area);
    hlayout->addWidget(text);
    hlayout->addStretch();
    hlayout->addWidget(toggle_btn);
    ElaScrollPageArea *projection_area = new ElaScrollPageArea(this);
    text = new ElaText("Projection Mode", this);
    text->setWordWrap(false);
    text->setTextPixelSize(13);
    ElaComboBox *projection_selector = new ElaComboBox(this);
    projection_selector->addItems({"Perspective", "Orthograhic"});
    hlayout = new QHBoxLayout(projection_area);
    hlayout->addWidget(text);
    hlayout->addStretch();
    hlayout->addWidget(projection_selector);
    ElaScrollPageArea *control_mode_area = new ElaScrollPageArea(this);
    text = new ElaText("Control Mode", this);
    text->setWordWrap(false);
    text->setTextPixelSize(13);
    ElaComboBox *control_mode_selector = new ElaComboBox(this);
    control_mode_selector->addItems({"Trackball", "Game"});
    hlayout = new QHBoxLayout(control_mode_area);
    hlayout->addWidget(text);
    hlayout->addStretch();
    hlayout->addWidget(control_mode_selector);
    ElaScrollPageArea *camera_select_area = new ElaScrollPageArea(this);
    text = new ElaText("Configured Camera", this);
    text->setWordWrap(false);
    text->setTextPixelSize(13);
    ElaComboBox *camera_selector = new ElaComboBox(this);
    camera_selector->addItems({
        "Trackball Perspective Camera",
        "First-Person Perspective Camera"
    });
    hlayout = new QHBoxLayout(camera_select_area);
    hlayout->addWidget(text);
    hlayout->addStretch();
    hlayout->addWidget(camera_selector);

    QWidget *central_widget = new QWidget(this);
    central_widget->setWindowTitle("Camera Control Panel");
    QVBoxLayout *vlayout = new QVBoxLayout(central_widget);
    vlayout->addWidget(button_area);
    vlayout->addWidget(projection_area);
    vlayout->addWidget(control_mode_area);
    vlayout->addWidget(camera_select_area);
    vlayout->addStretch();
    vlayout->setContentsMargins(0, 3, 14, 3);
    vlayout->setSpacing(10);
    this->addCentralWidget(central_widget, true, true, 0);

    connect(projection_selector, &QComboBox::currentIndexChanged, this, [](int index) {
        lcf::GlobalCamera::instance()->setProjectionType(static_cast<lcf::Camera::ProjectionType>(index));
    });
    connect(control_mode_selector, &QComboBox::currentIndexChanged, this, [](int index) {
        lcf::GlobalCamera::instance()->setControllerType(static_cast<lcf::GlobalCamera::ControllerType>(index));
    });
    connect(camera_selector, &QComboBox::currentIndexChanged, this, [](int index) {
        lcf::GlobalCamera::instance()->setCameraType(static_cast<lcf::GlobalCamera::Type>(index));
    });
    connect(toggle_btn, &ElaToggleButton::toggled, this, [=](bool checked) {
        if (checked) {
            toggle_btn->setText("Preset Configuration");
        } else {
            toggle_btn->setText("Custom Configuration");
        }
        projection_area->setEnabled(!checked);
        control_mode_area->setEnabled(!checked);
        camera_select_area->setEnabled(checked);
    });
    toggle_btn->toggled(false);
}