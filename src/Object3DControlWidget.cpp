#include "Object3DControlWidget.h"
#include <QLayout>
#include "ElaText.h"
#include "ElaPushButton.h"


Object3DControlWidget::Object3DControlWidget(QWidget *parent) :
    QWidget(parent),
    m_translation_group(3, nullptr),
    m_rotation_group(4, nullptr),
    m_scale_group(3, nullptr)
{
    m_object_name_edit = new ElaLineEdit(this);
    m_object_name_edit->setFixedHeight(40);
    m_translation_text = new ElaText("", 13, this);
    m_rotation_text = new ElaText("", 13, this);
    m_rotation_text->setMaximumWidth(800);
    m_rotation_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_world_pos_text = new ElaText("", 13, this);
    m_world_pos_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_scale_text = new ElaText("", 13, this);
    ElaText *object_name_text = new ElaText(" Object Name ", 13, this);
    ElaPushButton *set_name_button = new ElaPushButton("rename", this);
    set_name_button->setDarkDefaultColor({20, 143, 119});
    set_name_button->setDarkHoverColor({26, 188, 156});
    set_name_button->setDarkPressColor({20, 143, 119});

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0, 3, 14, 3);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->addWidget(object_name_text);
    hlayout->addWidget(m_object_name_edit);
    hlayout->addWidget(set_name_button);
    hlayout->addStretch();
    vlayout->addLayout(hlayout);

    QStringList labels = {"X = ", "Y = ", "Z = "};
    for (int i = 0; i < m_translation_group.size(); ++i) {
        auto &spinbox = m_translation_group[i];
        spinbox = new ElaDoubleSpinBox(this);
        spinbox->setSingleStep(0.1);
        spinbox->setRange(-1000.0, 1000.0);
        spinbox->setPrefix(labels[i]);
    }
    for (int i = 0; i < m_rotation_group.size(); ++i) {
        auto &spinbox = m_rotation_group[i];
        spinbox = new ElaDoubleSpinBox(this);
        spinbox->setSingleStep(1.0);
    }
    for (int i = 0; i < labels.size(); ++i) {
        auto &spinbox = m_rotation_group[i];
        spinbox->setPrefix(labels[i]);
        spinbox->setRange(-1000.0, 1000.0);
    }
    m_rotation_group.back()->setSuffix("°");
    m_rotation_group.back()->setRange(-360.0, 360.0);
    m_rotation_group[1]->setValue(1.0);
    for (int i = 0; i < m_scale_group.size(); ++i) {
        auto &spinbox = m_scale_group[i];
        spinbox = new ElaDoubleSpinBox(this);
        spinbox->setSingleStep(0.05);
        spinbox->setRange(0.001, 1000.0);
        spinbox->setPrefix(labels[i]);
    }
    std::vector<SpinBoxGroup *> spinboxes = {&m_translation_group, &m_scale_group};
    std::vector<ElaText *> texts = {m_translation_text, m_scale_text};
    for (int i = 0; i < texts.size(); ++i) {
        QHBoxLayout *hlayout = new QHBoxLayout(this);
        hlayout->addWidget(texts[i]);
        vlayout->addLayout(hlayout);
        for (auto spinbox : *spinboxes[i]) {
            hlayout->addWidget(spinbox);
        }
    }
    hlayout = new QHBoxLayout(this);
    hlayout->addWidget(m_rotation_text);
    hlayout->addStretch();
    hlayout->addWidget(m_rotation_group[3]);
    vlayout->addLayout(hlayout);
    hlayout = new QHBoxLayout(this);
    hlayout->addStretch();
    for (int i = 0; i < 3; ++i) {
        hlayout->addWidget(m_rotation_group[i]);
    }
    vlayout->addLayout(hlayout);
    hlayout = new QHBoxLayout(this);
    hlayout->addWidget(m_world_pos_text);
    vlayout->addLayout(hlayout);

    int spinbox_height = m_translation_group.front()->height();
    this->setFixedHeight(9 * spinbox_height + 20);
    this->updateControlArea();
    auto set_translation = [this](double value) {

        auto object_3d = m_controlled_object;
        if (not object_3d) { return; }
        lcf::Vector3D translation;
        for (int i = 0; i < 3; ++i) {
            translation[i] = m_translation_group[i]->value();
        }
        object_3d->setTranslation(translation);
    };
    for (auto &spinbox : m_translation_group) {
        connect(spinbox, &QDoubleSpinBox::valueChanged, set_translation);
    }
    auto set_rotation = [this](double value) {
        if (not m_controlled_object) { return; }
        float x = m_rotation_group[0]->value();
        float y = m_rotation_group[1]->value();
        float z = m_rotation_group[2]->value();
        float degrees = m_rotation_group[3]->value();
        m_controlled_object->setRotation(degrees, x, y, z);
    };
    for (auto &spinbox : m_rotation_group) {
        connect(spinbox, &QDoubleSpinBox::valueChanged, set_rotation);
    }
    auto set_scale = [this](double value) {
        if (not m_controlled_object) { return; }
        float scale_x = m_scale_group[0]->value();
        float scale_y = m_scale_group[1]->value();
        float scale_z = m_scale_group[2]->value();
        m_controlled_object->setScale(scale_x, scale_y, scale_z);
    };
    for (auto &spinbox : m_scale_group) {
        connect(spinbox, &QDoubleSpinBox::valueChanged, set_scale);
    }
    connect(set_name_button, &QPushButton::clicked, [this] {
        if (not m_controlled_object) { return; }
        m_controlled_object->setName(m_object_name_edit->text().toStdString());
        emit currentObjectNameChanged(m_controlled_object->getName());
    });
}


void Object3DControlWidget::setControlledObject(lcf::Object3D *object_3d)
{
    // if (m_controlled_object) {
    //     disconnect(m_controlled_object->signalSender(), &lcf::SignalSender::transformUpdated, this, &Object3DControlWidget::updateTexts);
    // }
    m_controlled_object = object_3d;
    this->updateControlArea();
    if (not object_3d) { return; }
    // connect(m_controlled_object->signalSender(), &lcf::SignalSender::transformUpdated, this, &Object3DControlWidget::updateTexts);
}

void Object3DControlWidget::updateControlArea()
{
    lcf::Vector3D translation;
    lcf::Vector4D rotation{0.0f, 1.0f, 0.0f, 0.0f};
    lcf::Vector3D scale{1.0f, 1.0f, 1.0f};
    if (m_controlled_object) {
        translation = m_controlled_object->translation();
        m_controlled_object->rotation().getAxisAndAngle(&rotation[0], &rotation[1], &rotation[2], &rotation[3]);
        scale = m_controlled_object->scale();
    }
    for (int i = 0; i < 3; ++i) {
        m_translation_group[i]->blockSignals(true);
        m_translation_group[i]->setValue(translation[i]);
        m_translation_group[i]->blockSignals(false);
    }
    for (int i = 0; i < 4; ++i) {
        m_rotation_group[i]->blockSignals(true);
        m_rotation_group[i]->setValue(rotation[i]);
        m_rotation_group[i]->blockSignals(false);
    }
    for (int i = 0; i < 3; ++i) {
        m_scale_group[i]->blockSignals(true);
        m_scale_group[i]->setValue(scale[i]);
        m_scale_group[i]->blockSignals(false);
    }
    this->updateTexts();
}

void Object3DControlWidget::updateTexts()
{
    QString object_name("None");
    QString translation_text = " Translation (%1, %2, %3) ";
    QString rotation_text = " Rotation (%1, %2, %3, %4) ";
    QString world_pos_text = " World Position (%1, %2, %3) ";
    QString scale_text = " Scale (%1, %2, %3) ";
    if (not m_controlled_object) {
        translation_text = translation_text.arg("0.00").arg("0.00").arg("0.00");
        rotation_text = rotation_text.arg("0.00").arg("0.00").arg("0.00").arg("0.00");
        world_pos_text = world_pos_text.arg("0.00").arg("0.00").arg("0.00");
        scale_text = scale_text.arg("1.00").arg("1.00").arg("1.00");
    } else {
        object_name = m_controlled_object->getName().c_str();
        const auto &world_pos = m_controlled_object->getWorldPosition();
        for (int i = 0; i < 3; ++i) {
            world_pos_text = world_pos_text.arg(QString::number(world_pos[i], 'f', 2));
        }
        const auto &translation = m_controlled_object->translation();
        for (int i = 0; i < 3; ++i) {
            translation_text = translation_text.arg(QString::number(translation[i], 'f', 2));
        }
        float rotation[4];
        m_controlled_object->rotation().getAxisAndAngle(rotation + 1, rotation + 2, rotation + 3, rotation);
        for (int i = 0; i < 4; ++i) {
            rotation_text = rotation_text.arg(QString::number(rotation[i], 'f', 2));
        }
        const auto &scale = m_controlled_object->scale();
        for (int i = 0; i < 3; ++i) {
            scale_text = scale_text.arg(QString::number(scale[i], 'f', 2));
        }
    }
    m_object_name_edit->setText(object_name);
    m_translation_text->setText(translation_text);
    m_rotation_text->setText(rotation_text);
    m_world_pos_text->setText(world_pos_text);
    m_scale_text->setText(scale_text);
}