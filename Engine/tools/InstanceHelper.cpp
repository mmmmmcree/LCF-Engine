#include "InstanceHelper.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

void lcf::InstanceHelper::bind()
{
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    if (not m_ssbo) { gl->glGenBuffers(1, &m_ssbo); }
    float *data = m_instance_offsets_data.data();
    size_t size = m_instance_offsets_data.size() * sizeof(float);
    gl->glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_ssbo);
    if (m_updated) {
        if (size > m_allocated_size) {
            m_allocated_size = size * 2;
            gl->glBufferData(GL_SHADER_STORAGE_BUFFER, m_allocated_size, nullptr, GL_DYNAMIC_DRAW);
        }
        gl->glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
        m_updated = false;
    }
    gl->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void lcf::InstanceHelper::addInstanceOffset(const Matrix4x4 &matrix)
{
    auto begin = Data::ConstIterator(matrix.data());
    m_instance_offsets_data.append(begin, begin + 16);
    m_updated = true;
}

void lcf::InstanceHelper::addInstanceData(const Matrix4x4 *data, size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        auto begin = Data::ConstIterator(data[i].data());
        m_instance_offsets_data.append(begin, begin + 16);
    }
    m_updated = true;
}

void lcf::InstanceHelper::setInstanceOffset(size_t index, const Matrix4x4 &matrix)
{
    if (index >= this->instanceCount()) { return; }
    memcpy(m_instance_offsets_data.data() + index * 16, matrix.data(), 16 * sizeof(float));
} 

int lcf::InstanceHelper::instanceCount() const
{
    return static_cast<int>(m_instance_offsets_data.size() / 16);
}
