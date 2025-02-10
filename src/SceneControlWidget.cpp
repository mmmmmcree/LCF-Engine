#include "SceneControlWidget.h"
#include "ElaComboBox.h"
#include "ElaText.h"
#include <QBoxLayout>
#include "ElaPushButton.h"
#include "ElaLineEdit.h"
#include "ElaMessageBar.h"
#include <QFileDialog>
#include "Constants.h"
#include "TextureManager.h"
#include "ControlManager.h"

SceneControlWidget::SceneControlWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vlayout = nullptr;
    QHBoxLayout *hlayout = nullptr;
    QVBoxLayout *g_vlayout = new QVBoxLayout(this);
    g_vlayout->setContentsMargins(0, 3, 0, 3);
    g_vlayout->setSpacing(3);

    ElaText *scene_select_text = new ElaText(" Scene Select ", 13, this);
    ElaComboBox *scene_select_box = new ElaComboBox(this);
    scene_select_box->setMinimumWidth(180);
    hlayout = new QHBoxLayout(this);
    hlayout->addWidget(scene_select_text);
    hlayout->addStretch();
    hlayout->addWidget(scene_select_box);
    g_vlayout->addLayout(hlayout);

    ElaText *set_skybox_text = new ElaText(" Set Skybox ", 13, this);
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
        QString select_filter("Image(*.jpg *.png *.hdr)");
        QString path = QFileDialog::getOpenFileName(this, "Select Image Path", lcf::path::res_prefix, select_filter);
        path_edit->setText(path);
    });
    connect(load_button, &ElaPushButton::clicked, this, [=] {
        QString path = path_edit->text();
        auto texture = lcf::TextureManager::instance()->loadTexture2D(path, true);
        auto current_scene = lcf::ControlManager::instance()->currentScene();
        current_scene->environment()->setSkyboxTexture(texture);
    });
    hlayout = new QHBoxLayout(this);
    hlayout->addWidget(set_skybox_text);
    hlayout->addWidget(path_select_button);
    hlayout->addWidget(path_edit);
    hlayout->addWidget(load_button);
    g_vlayout->addLayout(hlayout);

    int height = 10;
    height += scene_select_box->height();
    height += set_skybox_text->height();
    this->setFixedHeight(height);
}