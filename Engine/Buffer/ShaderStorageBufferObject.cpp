#include "ShaderStorageBufferObject.h"
#include "GLFunctions.h"

lcf::ShaderStorageBufferObject::~ShaderStorageBufferObject()
{
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    if (gl) {
        gl->glDeleteBuffers(1, &m_ssbo);
    }
}

void lcf::ShaderStorageBufferObject::setBindingPoint(unsigned int binding_point)
{
    m_binding_point = binding_point;
}

void lcf::ShaderStorageBufferObject::setDataSizes(const SizeList &sizes)
{
    m_data_description = BufferDataDescription(sizes);
    this->resize(m_ssbo_size);
}

lcf::ShaderStorageBufferObject::operator bool() const
{
    return m_ssbo;
}

bool lcf::ShaderStorageBufferObject::create()
{
    if (m_ssbo) { return true; }
    if (not m_data_description) { return false; }
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->glGenBuffers(1, &m_ssbo);
    this->resize(m_ssbo_size);
    return true;
}

bool lcf::ShaderStorageBufferObject::isCreated() const
{
    return m_ssbo;
}

void lcf::ShaderStorageBufferObject::bind()
{
    if (not this->isCreated()) { return; }
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_binding_point, m_ssbo);
}

void lcf::ShaderStorageBufferObject::release()
{
    if (not this->isCreated()) { return; }
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void lcf::ShaderStorageBufferObject::updateData(int index, int data_index, const void *data)
{
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    int offset = index * m_data_description.getTotalSize() + m_data_description.getOffset(data_index);
    int data_size = m_data_description.getSize(data_index);
    gl->glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, data_size, data);
}

void lcf::ShaderStorageBufferObject::resize(int size)
{
    m_ssbo_size = size;
    if (not this->isCreated()) { return; }
    this->bind();
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->glBufferData(GL_SHADER_STORAGE_BUFFER, m_ssbo_size * m_data_description.getTotalSize(), nullptr, GL_DYNAMIC_DRAW);
    this->release();
}

void lcf::ShaderStorageBufferObject::fitToSize(int size)
{
    if (size <= m_ssbo_size) { return; }
    this->resize(size + 5);
}

unsigned int lcf::ShaderStorageBufferObject::getId() const
{
    return m_ssbo;
}
