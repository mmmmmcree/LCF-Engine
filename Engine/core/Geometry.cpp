#include "Geometry.h"
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include "Constants.h"

lcf::Geometry::Geometry() :
    m_created(false), m_items_cnt(0),
    m_vao(nullptr), m_ebo(GLBuffer::IndexBuffer),
    m_mode(TRIANGLES)
{
}

void lcf::Geometry::setIndices(unsigned int *indices, size_t indices_size)
{
    m_indices.resize(indices_size * sizeof(unsigned int));
    memcpy(m_indices.data(), indices, indices_size * sizeof(unsigned int));
    m_indices_size = static_cast<int>(indices_size);
}

void lcf::Geometry::setBeginMode(GLBeginMode mode)
{
    m_mode = mode;
}

void lcf::Geometry::create()
{
    if (m_created) { return; }
    m_created = true;
    if (m_indices.empty()) {
        m_indices.resize(m_items_cnt * sizeof(unsigned int));
        for (unsigned int i = 0; i < m_items_cnt; ++i) {
            memcpy(m_indices.data() + i * sizeof(unsigned int), &i, sizeof(unsigned int));
        }
    }
    for (int i = 0; i < m_buffers.size(); ++i) {
        auto &vbo = m_buffers[i];
        auto &data = m_buffer_data[i];
        vbo.create();
        vbo.bind();
        vbo.setUsagePattern(GLBuffer::StaticDraw);
        vbo.allocate(data.data(), static_cast<int>(data.size()));
        vbo.release();
        data.clear();
    }
    m_ebo.create();
    m_ebo.bind();
    m_ebo.setUsagePattern(GLBuffer::StaticDraw);
    m_ebo.allocate(m_indices.data(), static_cast<int>(m_indices.size()));
    m_ebo.release();
    m_indices.clear();
    auto gl = QOpenGLContext::currentContext()->functions();
    m_vao = std::make_unique<GLVAO>();
    m_vao->create();
    m_vao->bind();
    unsigned int loc = 0;
    for (int i = 0; i < m_buffers.size(); ++i) {
        m_buffers[i].bind();
        const auto &attr_infos = m_attribute_infos_list[i];
        size_t attr_idx = 0;
        for (auto &attr_info : attr_infos.get()) {
            gl->glEnableVertexAttribArray(loc);
            gl->glVertexAttribPointer(loc, attr_info.itemSize(), attr_info.GLType(), GL_FALSE, attr_infos.strideBytes(), (void *)(attr_infos.offset(attr_idx)));
            ++loc;
            ++attr_idx;
        }
    }
    m_ebo.bind();
    m_vao->release();
}

bool lcf::Geometry::isCreated() const
{
    return m_created;
}

unsigned int lcf::Geometry::id() const
{
    if (not m_vao) { return 0; }
    return m_vao->objectId();
}

int lcf::Geometry::indicesSize() const
{
    return m_indices_size;
}

void lcf::Geometry::draw()
{
    if (not m_created) { return; }
    m_vao->bind();
    QOpenGLContext::currentContext()->functions()->glDrawElements(m_mode, m_indices_size, GL_UNSIGNED_INT, nullptr);
    m_vao->release();
}

void lcf::Geometry::drawQuad()
{
    if (not s_quad) {
        s_quad = std::make_unique<Geometry>();
        s_quad->addInterleavedAttributes(data::quad, std::size(data::quad), {3, 3, 2});
        s_quad->create();
    }
    s_quad->draw();
}

void lcf::Geometry::drawCube()
{
    if (not s_cube) {
        s_cube = std::make_unique<Geometry>();
        s_cube->addInterleavedAttributes(data::cube, std::size(data::cube), {3, 3, 2});
        s_cube->create();
    }
    s_cube->draw();
}

void lcf::Geometry::drawSphere()
{
    if (not s_sphere) {
        s_sphere = std::make_unique<Geometry>(generateSphere());
    }
    s_sphere->draw();
}

lcf::Geometry *lcf::Geometry::quad()
{
    if (not s_quad) {
        s_quad = std::make_unique<Geometry>();
        s_quad->addInterleavedAttributes(data::quad, std::size(data::quad), {3, 3, 2});
        s_quad->create();
    }
    return s_quad.get();
}

lcf::Geometry *lcf::Geometry::cube()
{
    if (not s_cube) {
        s_cube = std::make_unique<Geometry>();
        s_cube->addInterleavedAttributes(data::cube, std::size(data::cube), {3, 3, 2});
        s_cube->create();
    }
    return s_cube.get();
}

lcf::Geometry *lcf::Geometry::sphere()
{
    if (not s_sphere) {
        s_sphere = std::make_unique<Geometry>(generateSphere());
    }
    return s_sphere.get();
}

lcf::Geometry lcf::Geometry::generateSphere(int x_segments, int y_segments)
{
    constexpr float PI = 3.14159265359f;
    std::vector<float> vertices;
    for (int i = 0; i <= x_segments; ++i) {
        for (int j = 0; j <= y_segments; ++j) {
            float x_segment = static_cast<float>(i) / x_segments;
            float y_segment = static_cast<float>(j) / y_segments;
            float theta = x_segment * 2.0f * PI, phi = y_segment * PI;
            float x = std::cos(theta) * std::sin(phi);
            float y = std::cos(phi);
            float z = std::sin(theta) * std::sin(phi);
            vertices.insert(vertices.end(), {x, y, z, x, y, z, x_segment, y_segment});
        }
    }
    std::vector<unsigned int> indices;
    for (int y = 0, odd_row = false; y < y_segments; ++y, odd_row = !odd_row) {
        if (not odd_row) {
            for (int x = 0; x <= x_segments; ++x) {
                indices.emplace_back(y * (x_segments + 1) + x);
                indices.emplace_back((y + 1) * (x_segments + 1) + x);
            }
        } else {
            for (int x = x_segments; x >= 0; --x) {
                indices.emplace_back((y + 1) * (x_segments + 1) + x);
                indices.emplace_back(y * (x_segments + 1) + x);
            }
        }
    }
    lcf::Geometry sphere;
    sphere.addInterleavedAttributes(vertices.data(), vertices.size(), {3, 3, 2});
    sphere.setIndices(indices.data(), indices.size());
    sphere.setBeginMode(TRIANGLE_STRIP);
    sphere.create();
    return sphere;
}
