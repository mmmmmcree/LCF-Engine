#include "Light.h"
#include "ShaderManager.h"
#include "Geometry.h"

lcf::Light::Light()
{

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
    uniform_list.emplace_back(SingleUniform(uniformName("diffuse_intensity"), [this] { return m_diffuse_intensity; }));
    uniform_list.emplace_back(SingleUniform(uniformName("specular_intensity"), [this] { return m_specular_intensity; }));
    uniform_list.emplace_back(SingleUniform(uniformName("ambient_intensity"), [this] { return m_ambient_intensity; }));
    return uniform_list;
}

void lcf::Light::setColor(const Vector3D &color)
{
    m_color = color;
}

void lcf::Light::setDiffuseIntensity(float intensity)
{
    m_diffuse_intensity = intensity;
}

void lcf::Light::setSpecularIntensity(float intensity)
{
    m_specular_intensity = intensity;
}

void lcf::Light::setAmbientIntensity(float intensity)
{
    m_ambient_intensity = intensity;
}

lcf::Vector3D lcf::Light::direction()
{
    return -this->worldMatrix().column(2).toVector3D().normalized();
}

std::string lcf::Light::uniformName(const std::string &name) const
{
    return this->name() + "." + name;
}