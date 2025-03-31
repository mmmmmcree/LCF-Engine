#include "ModelControlWidget.h"
#include "Object3DControlWidget.h"
#include "MaterialControlWidget.h"
#include "ElaCheckBox.h"
#include "ElaComboBox.h"
#include "ElaLineEdit.h"
#include "ElaPushButton.h"
#include "ElaMessageBar.h"
#include "ElaText.h"
#include <QFileDialog>
#include "ControlManager.h"
#include "ShaderManager.h"
#include "ModelManager.h"
#include "Constants.h"
#include <QLayout>


ModelControlWidget::ModelControlWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0, 3, 0, 3);
    vlayout->setSpacing(3);

    ElaText *load_model_text = new ElaText(" Load Model ", 13, this);
    ElaPushButton *path_select_button = new ElaPushButton("select path", this);
    path_select_button->setDarkDefaultColor(QColor(125, 60, 152));
    path_select_button->setDarkHoverColor(QColor(165, 105, 189));
    path_select_button->setDarkPressColor(QColor(125, 60, 152));
    ElaLineEdit *path_edit = new ElaLineEdit(this);
    ElaPushButton *load_button = new ElaPushButton("load", this);
    load_button->setDarkDefaultColor(QColor(46, 134, 193));
    load_button->setDarkHoverColor(QColor(93, 173, 226));
    load_button->setDarkPressColor(QColor(46, 134, 193));
    connect(path_select_button, &ElaPushButton::clicked, this, [=] {
        QString select_filter("Model(*.glb *.gltf *.obj *.dae *.fbx)");
        QString path = QFileDialog::getOpenFileName(this, "Select Model Path", lcf::path::models_prefix, select_filter);
        path_edit->setText(path);
    });
    connect(load_button, &ElaPushButton::clicked, this, [=] {
        QString path = path_edit->text();
        auto model = lcf::ModelManager::instance()->load(path);
        if (not model) {
            ElaMessageBar::error(ElaMessageBarType::TopRight, "Tip", "Loading Failed", 2000, this);
            return;
        }
        lcf::ControlManager::instance()->currentScene()->addObject3D(model);
        ElaMessageBar::success(ElaMessageBarType::TopRight, "Tip", "Loading Success", 2000, this);
    });
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->addWidget(load_model_text);
    hlayout->addWidget(path_select_button);
    hlayout->addWidget(path_edit);
    hlayout->addWidget(load_button);
    vlayout->addLayout(hlayout);


    ElaComboBox *model_select_box = new ElaComboBox(this);
    model_select_box->setMinimumWidth(200);
    model_select_box->addItem("None");
    ElaText *model_select_text = new ElaText(" Select Model ", 13, this);
    connect(model_select_box, &QComboBox::currentIndexChanged, [] (int index) {
        auto scene = lcf::ControlManager::instance()->currentScene();
        lcf::ControlManager::instance()->setCurrentModel(scene->takeModel(index - 1));
    });
    hlayout = new QHBoxLayout(this);
    hlayout->addWidget(model_select_text);
    hlayout->addWidget(model_select_box);
    hlayout->addStretch();
    vlayout->addLayout(hlayout);

    ElaText *cast_shadow_text = new ElaText(" Cast Shadow ", 13, this);
    ElaCheckBox *cast_shadow_checkbox = new ElaCheckBox(this);
    connect(cast_shadow_checkbox, &QCheckBox::stateChanged, [=] (int state) {
        auto model = lcf::ControlManager::instance()->currentModel();
        model->setCastShadow(state == Qt::Checked);
    });
    hlayout = new QHBoxLayout(this);
    hlayout->addWidget(cast_shadow_text);
    hlayout->addWidget(cast_shadow_checkbox);
    hlayout->addStretch();
    vlayout->addLayout(hlayout);

    hlayout = new QHBoxLayout(this);
    ElaText *animation_text = new ElaText(" Animation Playlist ", 13, this);
    ElaComboBox *animation_select_box = new ElaComboBox(this);
    animation_select_box->addItem("None");
    animation_select_box->setMinimumWidth(200);
    connect(animation_select_box, &QComboBox::currentIndexChanged, [] (int index) {
        auto model = lcf::ControlManager::instance()->currentModel();
        model->playAnimation(index - 1);
    });
    hlayout->addWidget(animation_text);
    hlayout->addWidget(animation_select_box);
    hlayout->addStretch();
    vlayout->addLayout(hlayout);

    MaterialControlWidget *material_control_widget = new MaterialControlWidget(this);
    vlayout->addWidget(material_control_widget);

    Object3DControlWidget *object3d_control_widget = new Object3DControlWidget(this);
    connect(object3d_control_widget, &Object3DControlWidget::currentObjectNameChanged, [model_select_box](const std::string &name) {
        model_select_box->setItemText(model_select_box->currentIndex(), name.c_str());
    });
    vlayout->addWidget(object3d_control_widget);
    vlayout->addStretch();

    int height = 0;
    height += path_select_button->height();
    height += model_select_box->height();
    height += animation_select_box->height();
    height += cast_shadow_checkbox->height();
    height += material_control_widget->height();
    height += object3d_control_widget->height();
    this->setFixedHeight(height + 20);

    connect(lcf::ControlManager::instance(), &lcf::ControlManager::currentSceneChanged, [=] (lcf::Scene *scene) {
        model_select_box->clear();
        model_select_box->addItem("None");
        for (auto model : scene->models()) {
            model_select_box->addItem(model->getName().c_str());
        }
        model_select_box->setCurrentIndex(0);
    });

    connect(lcf::SignalSender::instance(), &lcf::SignalSender::modelAdded, [model_select_box] (lcf::Model *model) {
        model_select_box->addItem(model->getName().c_str());
    });

    connect(lcf::ControlManager::instance(), &lcf::ControlManager::currentModelChanged, [=](lcf::Model *model) {
        object3d_control_widget->setControlledObject(model);
        cast_shadow_checkbox->setCheckState(model->isCastShadow() ? Qt::Checked : Qt::Unchecked);
        material_control_widget->setControlledObject(model);
        animation_select_box->blockSignals(true);
        animation_select_box->clear();
        animation_select_box->addItem("None");
        for (const auto &animation : model->animations()) {
            animation_select_box->addItem(animation->name().c_str());
        }
        animation_select_box->setCurrentIndex(model->currentAnimationIndex() + 1);
        animation_select_box->blockSignals(false);
    });
}