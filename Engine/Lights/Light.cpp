#include "Light.h"
#include "ShaderManager.h"
#include "Geometry.h"

lcf::Object3DType lcf::Light::type() const
{
    return Object3DType::Light;    
}

void lcf::Light::draw()
{
    static auto shader = ShaderManager::instance()->get(ShaderManager::GeometryDebug);
    shader->bind();
    shader->setUniformValue("model", this->worldMatrix());
    shader->setUniformValue("normal_matrix", this->normalMatrix());
    Geometry::quad()->draw();
    shader->release();
}

lcf::UniformList lcf::Light::asUniformList()
{
    UniformList uniform_list;
    uniform_list.emplace_back(SingleUniform(uniformName("color"), [this] { return m_color; }));
    uniform_list.emplace_back(SingleUniform(uniformName("intensity"), [this] { return m_intensity; }));
    uniform_list.emplace_back(SingleUniform(uniformName("cast_shadow"), [this] { return this->castShadow(); }));
    return uniform_list;
}

void lcf::Light::setColor(const Vector3D &color)
{
    m_color = color;
}

void lcf::Light::setIntensity(float intensity)
{
    m_intensity = intensity;
}

lcf::Vector3D lcf::Light::direction()
{
    return -this->worldMatrix().column(2).toVector3D().normalized();
}

void lcf::Light::setShadowMapUnit(int unit)
{
    m_shadow_map_unit = unit;
}

std::string lcf::Light::uniformName(const std::string &name) const
{
    return this->name() + "." + name;
}