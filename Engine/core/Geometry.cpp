#include "Geometry.h"
#include "GLFunctions.h"
#include "Constants.h"

lcf::Geometry::Geometry() :
    m_created(false), m_items_cnt(0),
    m_vao(nullptr), m_ebo(GLBuffer::IndexBuffer),
    m_mode(TRIANGLES)
{
}

void lcf::Geometry::setIndices(unsigned int *indices, size_t indices_size)
{
    m_indices.resize(indices_size);
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
        m_indices.resize(m_items_cnt);
        std::iota(m_indices.begin(), m_indices.end(), 0);
        m_indices_size = static_cast<int>(m_items_cnt);
    }
    for (int i = 0; i < m_buffers.size(); ++i) {
        auto &vbo = m_buffers[i];
        auto &data = m_buffer_data[i];
        vbo.create();
        vbo.bind();
        vbo.setUsagePattern(GLBuffer::StaticDraw);
        vbo.allocate(data.data(), static_cast<int>(data.size()));
        vbo.release();
    }
    m_ebo.create();
    m_ebo.bind();
    m_ebo.setUsagePattern(GLBuffer::StaticDraw);
    m_ebo.allocate(m_indices.data(), static_cast<int>(m_indices.size() * sizeof(unsigned int)));
    m_ebo.release();
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    m_vao = std::make_unique<GLVAO>();
    m_vao->create();
    m_vao->bind();
    for (int i = 0; i < m_buffers.size(); ++i) {
        m_buffers[i].bind();
        const auto &attr_infos = m_attribute_infos_list[i];
        size_t attr_idx = 0;
        for (auto &attr_info : attr_infos.get()) {
            gl->glEnableVertexAttribArray(attr_info.location());
            gl->glVertexAttribPointer(attr_info.location(), attr_info.itemSize(), attr_info.GLType(), GL_FALSE, attr_infos.strideBytes(), (void *)(attr_infos.offset(attr_idx)));
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
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->glDrawElements(m_mode, m_indices_size, GL_UNSIGNED_INT, nullptr);
    m_vao->release();
}

void lcf::Geometry::drawInstanced(int instance_count)
{
    if (not m_created) { return; }
    m_vao->bind();
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->glDrawElementsInstanced(m_mode, m_indices_size, GL_UNSIGNED_INT, nullptr, instance_count);
    m_vao->release();
}

const lcf::Geometry::Ptr &lcf::Geometry::quad()
{
    static Ptr s_quad = nullptr;
    if (not s_quad) {
        s_quad = std::make_shared<Geometry>();
        s_quad->addInterleavedAttributes(data::quad, std::size(data::quad), {{0, 3}, {1, 3}, {2, 2}});
        s_quad->create();
    }
    return s_quad;
}

const lcf::Geometry::Ptr &lcf::Geometry::cube()
{
    static Ptr s_cube = nullptr;
    if (not s_cube) {
        s_cube = std::make_shared<Geometry>();
        s_cube->addInterleavedAttributes(data::cube, std::size(data::cube), {{0, 3}, {1, 3}, {2, 2}});
        s_cube->create();
    }
    return s_cube;
}

const lcf::Geometry::Ptr &lcf::Geometry::sphere()
{
    static Ptr s_sphere = nullptr;
    if (not s_sphere) {
        s_sphere = std::make_unique<Geometry>(generateSphere());
    }
    return s_sphere;
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
    sphere.addInterleavedAttributes(vertices.data(), vertices.size(), {{0, 3}, {1, 3}, {2, 2}});
    sphere.setIndices(indices.data(), indices.size());
    sphere.setBeginMode(TRIANGLE_STRIP);
    sphere.create();
    return sphere;
}
