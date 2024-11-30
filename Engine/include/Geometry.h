#pragma once

#include "AttributeInfo.h"
#include "GLBuffer.h"
#include "GLVAO.h"
#include <unordered_map>
#include <QDebug>

namespace lcf {
    class Geometry
    {
    // Attribute的添加顺序就是VAO的绑定顺序
    public:
        Geometry();
        template <typename T>
        void addAttribute(T *data, size_t data_size, int item_size);
        template <typename T>
        void addInterleavedAttributes(T *data, size_t data_size, const std::initializer_list<int> &item_sizes);
        void setIndices(unsigned int *indices, size_t indices_size);
        void setBeginMode(GLBeginMode mode);
        void create();
        bool isCreated() const;
        unsigned int id() const;
        int indicesSize() const;
        void draw();
        static void drawQuad();
        static void drawCube();
        static void drawSphere();
        static Geometry *quad();
        static Geometry *cube();
        static Geometry *sphere();
    private:
        static Geometry generateSphere(int x_segments = 128, int y_segments = 128);
        inline static std::unique_ptr<Geometry> s_quad = nullptr;
        inline static std::unique_ptr<Geometry> s_cube = nullptr;
        inline static std::unique_ptr<Geometry> s_sphere = nullptr;
    private:
        bool m_created;
        size_t m_items_cnt;
        std::unique_ptr<GLVAO> m_vao;
        std::vector<GLBuffer> m_buffers;
        std::vector<std::vector<unsigned char>> m_buffer_data;
        std::vector<AttributeInfos> m_attribute_infos_list;
        GLBuffer m_ebo;
        std::vector<unsigned char> m_indices;
        int m_indices_size;
        GLBeginMode m_mode;
        //todo Bounding Box
        //todo Bounding Sphere
    };
}

template <typename T>
inline void lcf::Geometry::addAttribute(T *data, size_t data_size, int item_size)
{
    if (not data or data_size == 0) { return; }
    if (m_items_cnt == 0) {
        m_items_cnt = data_size / item_size;
    } else if (m_items_cnt != data_size / item_size) {
        qDebug() << "Data does not match the number of items.";
    }
    auto &attr_infos = m_attribute_infos_list.emplace_back(std::initializer_list<int>{item_size}, T{});
    auto &vbo = m_buffers.emplace_back(GLBuffer::VertexBuffer);
    auto &buffer_data = m_buffer_data.emplace_back(std::vector<unsigned char>(data_size * sizeof(T)));

    if constexpr (std::is_same_v<T, int>) {
        qDebug() << attr_infos.GLType() << attr_infos.typeSize();
    }
    memcpy(buffer_data.data(), data, data_size * sizeof(T));
}

template <typename T>
inline void lcf::Geometry::addInterleavedAttributes(T *data, size_t data_size, const std::initializer_list<int> &item_sizes)
{
    if (not data or data_size == 0) { return; }
    int total_size = 0;
    for (auto item_size : item_sizes) { total_size += item_size; }
    if (m_items_cnt == 0) {
        m_items_cnt = data_size / total_size;
    } else if (m_items_cnt != data_size / total_size) {
         qDebug() << "Data does not match the number of items.";
    }
    auto &attr_infos = m_attribute_infos_list.emplace_back(item_sizes, T{});
    auto &vbo = m_buffers.emplace_back(GLBuffer::VertexBuffer);
    auto &buffer_data = m_buffer_data.emplace_back(std::vector<unsigned char>(data_size * sizeof(T)));
    memcpy(buffer_data.data(), data, data_size * sizeof(T));
}
