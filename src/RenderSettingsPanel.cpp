#include "RenderSettingsPanel.h"
#include "ElaScrollPageArea.h"
#include "ElaCheckBox.h"
#include <QLayout>
#include "Renderer.h"
#include "ElaDoubleSpinBox.h"
#include "ElaText.h"

RenderSettingsPanel::RenderSettingsPanel(QWidget *parent) :
    ElaScrollPage(parent)
{
    QVBoxLayout *vlayout = nullptr;
    QHBoxLayout *hlayout = nullptr;


    ElaScrollPageArea *gamma_area = new ElaScrollPageArea(this);
    vlayout = new QVBoxLayout(gamma_area);
    ElaText *gamma_text = new ElaText(" Gamma Correction ", 13, this);
    ElaCheckBox *gamma_correction_checkbox = new ElaCheckBox(this);
    hlayout = new QHBoxLayout();
    hlayout->addWidget(gamma_text);
    hlayout->addWidget(gamma_correction_checkbox);
    hlayout->addStretch();
    vlayout->addLayout(hlayout);
    connect(lcf::Renderer::instance(), &lcf::Renderer::GammaCorrectionEnabledChanged, [gamma_correction_checkbox] (bool enabled) {
        gamma_correction_checkbox->setChecked(enabled);
    });
    connect(gamma_correction_checkbox, &QCheckBox::stateChanged, [] (int state) {
        lcf::Renderer::instance()->enableGammaCorrection(state == Qt::Checked);
    });

    ElaScrollPageArea *bloom_area = new ElaScrollPageArea(this);
    vlayout = new QVBoxLayout(bloom_area);
    ElaText *bloom_text = new ElaText(" Bloom ", 13, this);
    ElaCheckBox *bloom_checkbox = new ElaCheckBox(this);
    hlayout = new QHBoxLayout();
    hlayout->addWidget(bloom_text);
    hlayout->addWidget(bloom_checkbox);
    hlayout->addStretch();
    vlayout->addLayout(hlayout);
    connect(lcf::Renderer::instance(), &lcf::Renderer::BloomEnabledChanged, [bloom_checkbox] (bool enabled) {
        bloom_checkbox->setChecked(enabled);
    });
    connect(bloom_checkbox, &QCheckBox::stateChanged, [] (int state) {  
        lcf::Renderer::instance()->enableBloom(state == Qt::Checked);
    });

    ElaScrollPageArea *hdr_area = new ElaScrollPageArea(this);
    vlayout = new QVBoxLayout(hdr_area);
    ElaText *hdr_text = new ElaText(" HDR ", 13, this);
    ElaCheckBox *hdr_checkbox = new ElaCheckBox(this);
    hlayout = new QHBoxLayout();
    hlayout->addWidget(hdr_text);
    hlayout->addWidget(hdr_checkbox);
    hlayout->addStretch();
    vlayout->addLayout(hlayout);
    hlayout = new QHBoxLayout();
    ElaText *hdr_exposure_text = new ElaText(" Exposure ", 13, this);
    ElaDoubleSpinBox *exposure_spinbox = new ElaDoubleSpinBox(this);
    exposure_spinbox->setRange(0.02, 10.0);
    exposure_spinbox->setSingleStep(0.02);
    exposure_spinbox->setValue(1.0);
    hlayout->addWidget(hdr_exposure_text);
    hlayout->addWidget(exposure_spinbox);
    vlayout->addLayout(hlayout);
    connect(lcf::Renderer::instance(), &lcf::Renderer::HDREnabledChanged, [hdr_checkbox] (bool enabled) {
        hdr_checkbox->setChecked(enabled);
    });
    connect(hdr_checkbox, &QCheckBox::stateChanged, [bloom_checkbox] (int state) {
        lcf::Renderer::instance()->enableHDR(state == Qt::Checked);
    });
    connect(exposure_spinbox, &QDoubleSpinBox::valueChanged, [] (double value) {
        lcf::Renderer::instance()->setHDRExposure(static_cast<float>(value));
    });

    ElaScrollPageArea *msaa_area = new ElaScrollPageArea(this);
    vlayout = new QVBoxLayout(msaa_area);
    ElaText *msaa_text = new ElaText(" MSAA ", 13, this);
    ElaCheckBox *msaa_checkbox = new ElaCheckBox(this);
    hlayout = new QHBoxLayout();
    hlayout->addWidget(msaa_text);
    hlayout->addWidget(msaa_checkbox);
    hlayout->addStretch();
    vlayout->addLayout(hlayout);
    connect(lcf::Renderer::instance(), &lcf::Renderer::MSAAEnabledChanged, [msaa_checkbox] (bool enabled) {
        msaa_checkbox->setChecked(enabled);
    });
    connect(msaa_checkbox, &QCheckBox::stateChanged, [] (int state) {
        lcf::Renderer::instance()->enableMSAA(state == Qt::Checked);
    });

    ElaScrollPageArea *fxaa_area = new ElaScrollPageArea(this);
    vlayout = new QVBoxLayout(fxaa_area);
    ElaText *fxaa_text = new ElaText(" FXAA ", 13, this);
    ElaCheckBox *fxaa_checkbox = new ElaCheckBox(this);
    hlayout = new QHBoxLayout();
    hlayout->addWidget(fxaa_text);
    hlayout->addWidget(fxaa_checkbox);
    hlayout->addStretch();
    vlayout->addLayout(hlayout);
    connect(lcf::Renderer::instance(), &lcf::Renderer::FXAAEnabledChanged, [fxaa_checkbox] (bool enabled) {
        fxaa_checkbox->setChecked(enabled);
    });
    connect(fxaa_checkbox, &QCheckBox::stateChanged, [] (int state) {
        lcf::Renderer::instance()->enableFXAA(state == Qt::Checked);
    });

    ElaScrollPageArea *color_grading_area = new ElaScrollPageArea(this);
    vlayout = new QVBoxLayout(color_grading_area);
    ElaText *color_grading_text = new ElaText(" Color Grading ", 13, this);
    ElaCheckBox *color_grading_checkbox = new ElaCheckBox(this);
    hlayout = new QHBoxLayout();
    hlayout->addWidget(color_grading_text);
    hlayout->addWidget(color_grading_checkbox);
    hlayout->addStretch();
    vlayout->addLayout(hlayout);
    connect(lcf::Renderer::instance(), &lcf::Renderer::ColorGradingEnabledChanged, [color_grading_checkbox] (bool enabled) {
        color_grading_checkbox->setChecked(enabled);
    });
    connect(color_grading_checkbox, &QCheckBox::stateChanged, [] (int state) {
        lcf::Renderer::instance()->enableColorGrading(state == Qt::Checked);
    });

    QWidget *central_widget = new QWidget(this);
    central_widget->setWindowTitle("Render Settings Panel");
    vlayout = new QVBoxLayout(central_widget);
    vlayout->addWidget(gamma_area);
    vlayout->addWidget(hdr_area);
    vlayout->addWidget(bloom_area);
    vlayout->addWidget(msaa_area);
    vlayout->addWidget(fxaa_area);
    vlayout->addWidget(color_grading_area);
    vlayout->addStretch();
    vlayout->setContentsMargins(0, 3, 14, 3);
    vlayout->setSpacing(10);
    this->addCentralWidget(central_widget, true, true, 0);
}