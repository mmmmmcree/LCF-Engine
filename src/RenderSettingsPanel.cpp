#include "RenderSettingsPanel.h"
#include "ElaScrollPageArea.h"
#include "ElaCheckBox.h"
#include <QLayout>
#include "Renderer.h"

RenderSettingsPanel::RenderSettingsPanel(QWidget *parent) :
    ElaScrollPage(parent)
{
    ElaScrollPageArea *render_area = new ElaScrollPageArea(this);

    QVBoxLayout *vlayout = new QVBoxLayout(render_area);
    ElaCheckBox *gamma_correction_checkbox = new ElaCheckBox("Gamma Correction", this);
    ElaCheckBox *hdr_checkbox = new ElaCheckBox("HDR", this);
    ElaCheckBox *bloom_checkbox = new ElaCheckBox("Bloom", this);
    ElaCheckBox *msaa_checkbox = new ElaCheckBox("MSAA", this);
    vlayout->addWidget(gamma_correction_checkbox);
    vlayout->addWidget(hdr_checkbox);
    vlayout->addWidget(bloom_checkbox);
    vlayout->addWidget(msaa_checkbox);
    vlayout->addStretch();
    int checkbox_height = gamma_correction_checkbox->height();
    render_area->setFixedHeight(checkbox_height * 4 + 10);

    QWidget *central_widget = new QWidget(this);
    central_widget->setWindowTitle("Render Settings Panel");
    vlayout = new QVBoxLayout(central_widget);
    vlayout->addWidget(render_area);
    vlayout->addStretch();
    vlayout->setContentsMargins(0, 3, 14, 3);
    vlayout->setSpacing(10);
    this->addCentralWidget(central_widget, true, true, 0);

    connect(lcf::Renderer::instance(), &lcf::Renderer::GammaCorrectionEnabledChanged, [gamma_correction_checkbox] (bool enabled) {
        gamma_correction_checkbox->setChecked(enabled);
    });

    connect(lcf::Renderer::instance(), &lcf::Renderer::HDREnabledChanged, [hdr_checkbox] (bool enabled) {
        hdr_checkbox->setChecked(enabled);
    });

    connect(lcf::Renderer::instance(), &lcf::Renderer::BloomEnabledChanged, [bloom_checkbox] (bool enabled) {
        bloom_checkbox->setChecked(enabled);
    });

    connect(lcf::Renderer::instance(), &lcf::Renderer::MSAAEnabledChanged, [msaa_checkbox] (bool enabled) {
        msaa_checkbox->setChecked(enabled);
    });

    connect(gamma_correction_checkbox, &QCheckBox::stateChanged, [] (int state) {
        lcf::Renderer::instance()->enableGammaCorrection(state == Qt::Checked);
    });
    connect(hdr_checkbox, &QCheckBox::stateChanged, [bloom_checkbox] (int state) {
        lcf::Renderer::instance()->enableHDR(state == Qt::Checked);
    });
    connect(bloom_checkbox, &QCheckBox::stateChanged, [] (int state) {  
        lcf::Renderer::instance()->enableBloom(state == Qt::Checked);
    });
    connect(msaa_checkbox, &QCheckBox::stateChanged, [] (int state) {
        lcf::Renderer::instance()->enableMSAA(state == Qt::Checked);
    });
}