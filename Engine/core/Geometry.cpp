#include "Geometry.h"
#include "GLFunctions.h"
#include "Constants.h"
#include "ShapeFactory.h"

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
    }
    for (int i = 0; i < m_vbo_list.size(); ++i) {
        auto &vbo = m_vbo_list[i];
        auto &data = m_buffer_data_list[i];
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
    for (int i = 0; i < m_vbo_list.size(); ++i) {
        m_vbo_list[i].bind();
        const auto &attr_infos = m_interleaved_attribute_info_list[i];
        size_t attr_idx = 0;
        for (auto &attr_info : attr_infos.get()) {
            int location = attr_info.location();
            int item_size = attr_info.itemSize();
            int gl_type = attr_info.GLType();
            int stride_bytes = attr_infos.strideBytes();
            int offset_bytes = attr_infos.offset(attr_idx);
            gl->glEnableVertexAttribArray(location);
            gl->glVertexAttribPointer(location, item_size, gl_type, GL_FALSE, stride_bytes, (void *)(static_cast<size_t>(offset_bytes)));
            if (location == static_cast<int>(AttributeLocation::Position)) {
                auto &buffer_data = m_buffer_data_list[i];
                this->readPositions(buffer_data.data(), buffer_data.size(), stride_bytes, offset_bytes);
            }
            ++attr_idx;
        }
    }
    m_ebo.bind();
    m_vao->release();
    for (auto &buffer_data : m_buffer_data_list) {
        buffer_data.clear();
    }
    m_bounding_shape_map.emplace(ShapeType::Box, ShapeFactory::createUnique<Box>(m_positions));
    m_bounding_shape_map.emplace(ShapeType::Sphere, ShapeFactory::createUnique<Sphere>(m_positions));
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

int lcf::Geometry::getIndicesSize() const
{
    return static_cast<int>(m_indices.size());
}

void lcf::Geometry::draw()
{
    if (not m_created) { return; }
    m_vao->bind();
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->glDrawElements(m_mode, this->getIndicesSize(), GL_UNSIGNED_INT, nullptr);
    m_vao->release();
}

void lcf::Geometry::drawInstanced(int instance_count)
{
    if (not m_created) { return; }
    m_vao->bind();
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->glDrawElementsInstanced(m_mode, this->getIndicesSize(), GL_UNSIGNED_INT, nullptr, instance_count);
    m_vao->release();
}

const lcf::Geometry::SharedPtr &lcf::Geometry::quad()
{
    static SharedPtr s_quad = nullptr;
    if (not s_quad) {
        s_quad = std::make_shared<Geometry>();
        s_quad->addInterleavedAttributes(data::quad, std::size(data::quad), {{0, 3}, {1, 3}, {2, 2}});
        s_quad->create();
    }
    return s_quad;
}

const lcf::Geometry::SharedPtr &lcf::Geometry::cube()
{
    static SharedPtr s_cube = nullptr;
    if (not s_cube) {
        s_cube = std::make_shared<Geometry>();
        s_cube->addInterleavedAttributes(data::cube, std::size(data::cube), {{0, 3}, {1, 3}, {2, 2}});
        s_cube->create();
    }
    return s_cube;
}

const lcf::Geometry::SharedPtr &lcf::Geometry::sphere()
{
    static SharedPtr s_sphere = nullptr;
    if (not s_sphere) {
        s_sphere = std::make_unique<Geometry>(generateSphere());
    }
    return s_sphere;
}

const lcf::Geometry::PositionList &lcf::Geometry::getPositions() const
{
    return m_positions;
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

void lcf::Geometry::readPositions(void *data, size_t total_bytes, int stride_bytes, int offset_bytes)
{
    m_positions.reserve(total_bytes / stride_bytes);
    Vector3D position;
    for (int i = 0; i < total_bytes; i += stride_bytes) {
        memcpy(&position, static_cast<unsigned char *>(data) + i + offset_bytes, sizeof(position));
        m_positions.emplace_back(position);
    }
}
