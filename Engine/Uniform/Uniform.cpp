#include "Uniform.h"
#include "GLShaderProgram.h"
#include "UniformManager.h"

lcf::Uniform::~Uniform()
{
    if (UniformManager::isInstanceExists()) {
        UniformManager::instance()->removeUniform(this);
    }
}

lcf::Uniform::Uniform(const Uniform &other) :
    m_name(other.m_name)
{
}

void lcf::Uniform::setName(std::string_view name)
{
    m_name = name;
    UniformManager::instance()->addUniform(this);
    UniformManager::instance()->emitUniformUpdatedSignal(this);
}

const std::string &lcf::Uniform::name() const
{
    return m_name;
}

