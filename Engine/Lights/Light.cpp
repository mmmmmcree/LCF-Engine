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
    shader->bindWithTextures();
    shader->setUniformValue("model", this->worldMatrix());
    shader->setUniformValue("normal_matrix", this->normalMatrix());
    Geometry::quad()->draw();
    shader->release();
}

void lcf::Light::bind()
{
    m_direction.setValue(this->orientation());
    m_position.setValue(this->worldPosition());
}

void lcf::Light::setColor(const Vector3D &color)
{
    m_color.setValue(color);
}

void lcf::Light::setIntensity(float intensity)
{
    m_intensity.setValue(intensity);
}

lcf::Vector3D lcf::Light::orientation()
{
    return -this->worldMatrix().column(2).toVector3D().normalized();
}

void lcf::Light::setName(std::string_view name)
{
    Object3D::setName(name);
    m_color.setName(this->uniformName("color"));
    m_intensity.setName(this->uniformName("intensity"));
    m_cast_shadow.setName(this->uniformName("cast_shadow"));
    m_direction.setName(this->uniformName("direction"));
    m_position.setName(this->uniformName("position"));
}

void lcf::Light::setCastShadow(bool cast_shadow)
{
    m_cast_shadow.setValue(cast_shadow);
}

bool lcf::Light::isCastShadow() const
{
    return m_cast_shadow.value();
}

std::string lcf::Light::uniformName(const std::string &name) const
{
    return this->name() + "." + name;
}