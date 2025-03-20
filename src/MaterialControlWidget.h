#pragma once

#include <QWidget>
#include "ElaComboBox.h"
#include "Object3D.h"
#include "RenderableObject3D.h"

class MaterialControlWidget : public QWidget
{
public:
    MaterialControlWidget(QWidget* parent = nullptr);
    void setControlledObject(lcf::RenderableObject3D *object);
private:
    lcf::RenderableObject3D *m_controlled_object = nullptr;
    ElaComboBox *select_material_combo = nullptr;
};