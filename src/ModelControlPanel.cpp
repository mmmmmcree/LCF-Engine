#include "ModelControlPanel.h"
#include <QLayout>
#include "ElaScrollPageArea.h"
#include "ElaPushButton.h"
#include "ElaLineEdit.h"
#include "ElaText.h"
#include "ControlManager.h"
#include "Object3DControlWidget.h"
#include "ModelControlWidget.h"

ModelControlPanel::ModelControlPanel(QWidget *parent) :
    ElaScrollPage(parent)
{
    ElaScrollPageArea *model_control_area = new ElaScrollPageArea(this);
    ModelControlWidget *model_control_widget = new ModelControlWidget(this);
    model_control_area->setFixedHeight(model_control_widget->height() + 10);
    QVBoxLayout *vlayout = new QVBoxLayout(model_control_area);
    vlayout->addWidget(model_control_widget);
    vlayout->setContentsMargins(0, 3, 0, 3);

    QWidget *central_widget = new QWidget(this);
    central_widget->setWindowTitle("Model Loader Panel");
    vlayout = new QVBoxLayout(central_widget);
    vlayout->addWidget(model_control_area);
    vlayout->addStretch();
    vlayout->setContentsMargins(0, 3, 14, 3);
    vlayout->setSpacing(10);
    this->addCentralWidget(central_widget, true, true, 0);
}