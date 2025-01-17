#pragma once

#include "ElaDoubleSpinBox.h"
#include "ElaText.h"
#include "ElaLineEdit.h"
#include "Object3D.h"
#include <array>
#include <vector>
#include <functional>

class Object3DControlWidget : public QWidget
{
    Q_OBJECT
public:
    using SpinBoxGroup = std::vector<ElaDoubleSpinBox *>;
    using Object3DGetter = std::function<lcf::Object3D *()>;
    Object3DControlWidget(QWidget* parent = nullptr);
    void setObject3DGetter(Object3DGetter getter);
    void controlledObjectChanged(lcf::Object3D *object_3d);
private:
    void updateControlArea();
    void updateTexts();
signals:
    void currentObjectNameChanged(const std::string &name);
private:
    Object3DGetter m_object_getter = [] { return nullptr; };
    SpinBoxGroup m_translation_group;
    SpinBoxGroup m_rotation_group;
    SpinBoxGroup m_scale_group;
    ElaLineEdit *m_object_name_edit = nullptr;
    ElaText *m_world_pos_text = nullptr;
    ElaText *m_translation_text = nullptr;
    ElaText *m_rotation_text = nullptr;
    ElaText *m_scale_text = nullptr;
    lcf::Object3D *m_controlled_object = nullptr;
};