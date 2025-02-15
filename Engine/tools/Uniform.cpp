#include "Uniform.h"
#include "GLShaderProgram.h"
#include "UniformManager.h"

lcf::SingleUniform::SingleUniform(std::string_view name, const Value &value) :
    m_name(name), m_value(value)
{
}

lcf::SingleUniform::SingleUniform(std::string_view name, const ValueGenerator &generator) :
    m_name(name), m_generator(generator)
{
}

void lcf::SingleUniform::bind(GLShaderProgram *shader)
{
    const auto &value = m_generator.has_value() ? m_generator.value()() : m_value;
    std::visit([&](auto &&value) {
        shader->setUniformValue(m_name.c_str(), value);
    }, value);
}

const std::string &lcf::SingleUniform::name() const
{
    return m_name;
}

void lcf::SingleUniform::setLocation(int location)
{
    m_location = location;
}

void lcf::SingleUniform::setUniformValue(const Value &value)
{
    m_value = value;
    // UniformManager::instance()->emitUniformUpdatedSignal(*this);
}

lcf::ArrayUniform::ArrayUniform(std::string_view name, const Values &values) :
    m_name(name), m_values(values)
{
}

lcf::ArrayUniform::ArrayUniform(std::string_view name, const ValuesGenerator &generator) :
    m_name(name), m_generator(generator)
{
}

void lcf::ArrayUniform::bind(GLShaderProgram *shader)
{
    const auto &values = m_generator.has_value() ? m_generator.value()() : m_values;
    std::visit([&](auto &&values) {
        shader->setUniformValueArray(m_location, values.data(), static_cast<int>(values.size()));
    }, values);
}

const std::string &lcf::ArrayUniform::name() const
{
    return m_name;
}

void lcf::ArrayUniform::setLocation(unsigned int location)
{
    m_location = location;
}