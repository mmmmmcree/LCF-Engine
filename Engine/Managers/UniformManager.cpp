#include "UniformManager.h"

lcf::UniformManager *lcf::UniformManager::instance()
{
    static UniformManager s_instance;
    s_instance_exists = true;
    return &s_instance;
}

bool lcf::UniformManager::isInstanceExists()
{
    return s_instance_exists;
}

lcf::UniformManager::~UniformManager()
{
    s_instance_exists = false;
}

void lcf::UniformManager::emitUniformUpdatedSignal(const Uniform *uniform)
{
    if (m_uniforms.contains(uniform)) {
        emit uniformUpdated(uniform);
    }
}

void lcf::UniformManager::addUniform(const Uniform *uniform)
{
    if (not uniform) { return; }
    m_uniforms.insert(uniform);
}

void lcf::UniformManager::removeUniform(const Uniform *uniform)
{
    m_uniforms.remove(uniform);
}

bool lcf::UniformManager::existUniform(const Uniform *uniform) const
{
    return m_uniforms.contains(uniform);
}

lcf::UniformManager::UniformPtrList lcf::UniformManager::existedUniforms() const
{
    return m_uniforms.values();
}
