#include "MaterialControlWidget.h"
#include <QLayout>
#include "ElaText.h"
#include "Define.h"

MaterialControlWidget::MaterialControlWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0, 3, 0, 3);
    vlayout->setSpacing(3);
    
    ElaText *select_material_text = new ElaText(" Select Material Type ", 13, this);
    select_material_combo = new ElaComboBox(this);
    select_material_combo->setMinimumWidth(200);
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->addWidget(select_material_text);
    hlayout->addWidget(select_material_combo);
    hlayout->addStretch();
    vlayout->addLayout(hlayout);

    QStringList material_types(static_cast<int>(lcf::MaterialType::SIZE) - 1);
    material_types[static_cast<int>(lcf::MaterialType::None)] = "None";
    material_types[static_cast<int>(lcf::MaterialType::PBR)] = "PBR";
    material_types[static_cast<int>(lcf::MaterialType::Phong)] = "Phong";
    material_types[static_cast<int>(lcf::MaterialType::UserCustom)] = "User Custom";
    select_material_combo->addItems(material_types);
}

void MaterialControlWidget::setControlledObject(lcf::RenderableObject3D *object)
{
    disconnect(select_material_combo, &QComboBox::currentIndexChanged, nullptr, nullptr);
    m_controlled_object = object;
    if (not m_controlled_object) { return; }
    select_material_combo->setCurrentIndex(static_cast<int>(m_controlled_object->materialType()));
    connect(select_material_combo, &QComboBox::currentIndexChanged, [this] (int index) {
        m_controlled_object->setMaterialType(static_cast<lcf::MaterialType>(index));
    });
}
