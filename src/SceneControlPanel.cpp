#include "SceneControlPanel.h"
#include "ElaScrollPage.h"
#include <QLayout>
#include "ElaScrollPageArea.h"
#include "ElaPushButton.h"
#include "ElaLineEdit.h"
#include "ElaText.h"
#include "ControlManager.h"
#include "Object3DControlWidget.h"
#include "SceneControlWidget.h"
#include "ModelControlWidget.h"
#include "ElaPivot.h"

SceneControlPanel::SceneControlPanel(QWidget *parent) : QWidget(parent)
{
    ElaScrollPage *page = new ElaScrollPage(this);
    page->setTitleVisible(false);

    ElaScrollPageArea *scene_control_area = new ElaScrollPageArea(page);
    SceneControlWidget *scene_control_widget = new SceneControlWidget(page);
    scene_control_area->setFixedHeight(scene_control_widget->height() + 10);
    QVBoxLayout *vlayout = new QVBoxLayout(scene_control_area);
    vlayout->addWidget(scene_control_widget);
    vlayout->setContentsMargins(0, 3, 0, 3);


    ElaScrollPageArea *model_control_area = new ElaScrollPageArea(page);
    ModelControlWidget *model_control_widget = new ModelControlWidget(page);
    model_control_area->setFixedHeight(model_control_widget->height() + 10);
    vlayout = new QVBoxLayout(model_control_area);
    vlayout->addWidget(model_control_widget);
    vlayout->setContentsMargins(0, 3, 0, 3);

    QList<QWidget *> central_widgets;

    QWidget *central_widget = new QWidget(page);
    vlayout = new QVBoxLayout(central_widget);
    vlayout->addWidget(scene_control_area);
    vlayout->addStretch();
    vlayout->setContentsMargins(0, 3, 14, 3);
    vlayout->setSpacing(10);
    central_widgets.push_back(central_widget);

    central_widget = new QWidget(page);
    vlayout = new QVBoxLayout(central_widget);
    vlayout->addWidget(model_control_area);
    vlayout->addStretch();
    vlayout->setContentsMargins(0, 3, 14, 3);
    vlayout->setSpacing(10);
    central_widgets.push_back(central_widget);
    

    for (auto central_widget : central_widgets) {
        page->addCentralWidget(central_widget, true, true, 0);
    }

    ElaPivot *pivot = new ElaPivot(this);
    pivot->appendPivot("Scene Control");
    pivot->appendPivot("Model Control");
    pivot->setCurrentIndex(0);
    connect(pivot, &ElaPivot::pivotClicked, page, [page] (int index) {
        page->navigation(index);
    });
    
    vlayout = new QVBoxLayout(this);
    vlayout->addWidget(pivot);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->addWidget(page);
}