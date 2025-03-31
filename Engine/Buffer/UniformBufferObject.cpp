#include "UniformBufferObject.h"
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>


lcf::UniformBufferObject::~UniformBufferObject()
{
    auto context = QOpenGLContext::currentContext();
    if (context and context->isValid()) {
        auto gl = context->functions();
        gl->glDeleteBuffers(1, &m_ubo);
    }
}

void lcf::UniformBufferObject::setBindingPoint(unsigned int binding_point)
{
    m_binding_point = binding_point;
}

void lcf::UniformBufferObject::setDataSizes(const SizeList &sizes)
{
    m_sizes = sizes;
    m_offsets.clear();
    m_total_size = 0;
    for (auto size : sizes) {
        m_offsets.push_back(m_total_size);
        m_total_size += size;
    }
}

lcf::UniformBufferObject::operator bool() const
{
    return this->isCreated();
}

void lcf::UniformBufferObject::create()
{
    if (this->isCreated() or m_sizes.empty()) { return; }
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glGenBuffers(1, &m_ubo);
    gl->glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    gl->glBufferData(GL_UNIFORM_BUFFER, m_total_size, nullptr, GL_DYNAMIC_DRAW);
}

bool lcf::UniformBufferObject::isCreated() const
{
    return m_ubo;
}

void lcf::UniformBufferObject::bind()
{
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    gl->glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    gl->glBindBufferRange(GL_UNIFORM_BUFFER, m_binding_point, m_ubo, 0, m_total_size);
}

void lcf::UniformBufferObject::release()
{
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void lcf::UniformBufferObject::updateData(int index, const void *data)
{
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glBufferSubData(GL_UNIFORM_BUFFER, m_offsets[index], m_sizes[index], data);
}
