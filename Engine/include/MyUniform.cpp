#include "MyUniform.h"
#include "GLShaderProgram.h"
#include "UniformManager.h"

lcf::MyUniform::~MyUniform()
{
    if (UniformManager::isInstanceExists()) {
        UniformManager::instance()->removeUniform(this);
    }
}

lcf::MyUniform::MyUniform(const MyUniform &other) :
    m_name(other.m_name)
{
}

void lcf::MyUniform::setName(std::string_view name)
{
    m_name = name;
    UniformManager::instance()->addUniform(this);
    UniformManager::instance()->emitUniformUpdatedSignal(this);
}

const std::string &lcf::MyUniform::name() const
{
    return m_name;
}

lcf::MySingleUniform::MySingleUniform(const SingleUniformValue &value) :
    m_value(value)
{
}

lcf::MySingleUniform::MySingleUniform(const MySingleUniform &other) :
    MyUniform(other),
    m_value(other.m_value)
{
}

void lcf::MySingleUniform::bind(GLShaderProgram *shader) const
{
    if (not shader) { return; }
    std::visit([&](auto &&value) {
        shader->setUniformValue(m_name.c_str(), value);
    }, m_value);
}

void lcf::MySingleUniform::setValue(const SingleUniformValue &value)
{
    m_value = value;
    UniformManager::instance()->emitUniformUpdatedSignal(this);
}

const lcf::SingleUniformValue &lcf::MySingleUniform::value() const
{
    return m_value;
}

bool lcf::MySingleUniform::asBool() const
{
    return std::get<bool>(m_value);
}

int lcf::MySingleUniform::asInt() const
{
    return std::get<int>(m_value);
}

float lcf::MySingleUniform::asFloat() const
{
    return std::get<float>(m_value);
}

const lcf::Vector2D &lcf::MySingleUniform::asVector2D() const
{
    return std::get<Vector2D>(m_value);
}

const lcf::Vector3D &lcf::MySingleUniform::asVector3D() const
{
    return std::get<Vector3D>(m_value);
}

const lcf::Vector4D &lcf::MySingleUniform::asVector4D() const
{
    return std::get<Vector4D>(m_value);
}

const lcf::Matrix2x2 &lcf::MySingleUniform::asMatrix2x2() const
{
    return std::get<Matrix2x2>(m_value);
}

const lcf::Matrix3x3 &lcf::MySingleUniform::asMatrix3x3() const
{
    return std::get<Matrix3x3>(m_value);
}

const lcf::Matrix4x4 &lcf::MySingleUniform::asMatrix4x4() const
{
    return std::get<Matrix4x4>(m_value);
}