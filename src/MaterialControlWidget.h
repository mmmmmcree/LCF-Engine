#pragma once

#include <QWidget>
#include "ElaComboBox.h"
#include "Object3D.h"

class MaterialControlWidget : public QWidget
{
public:
    MaterialControlWidget(QWidget* parent = nullptr);
    void setControlledObject(lcf::Object3D *object);
private:
    lcf::Object3D *m_controlled_object = nullptr;
    ElaComboBox *select_material_combo = nullptr;
};