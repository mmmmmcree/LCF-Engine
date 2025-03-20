#pragma once

#include "AttributeInfo.h"
#include "GLBuffer.h"
#include "GLVAO.h"
#include <unordered_map>
#include <QDebug>
#include "Vector.h"
#include <cstddef>
#include "Shapes.h"

namespace lcf {
    class Geometry
    {
    public:
        using SharedPtr = std::shared_ptr<Geometry>;
        using VBOList = std::vector<GLBuffer>;
        using InterleavedAttributeInfoList = std::vector<InterleavedAttributeInfo>;
        using IndexList = std::vector<unsigned int>;
        using BufferData = std::vector<unsigned char>;
        using BufferDataList = std::vector<BufferData>;
        using PositionList = std::vector<Vector3D>;
        // using BoundingShapeList = std::vector<Shape::UniquePtr>;
        using BoundingShapeMap = std::unordered_map<Shape::EnumType, Shape::UniquePtr>;
        Geometry();
        template <typename T>
        void addAttribute(T *data, size_t data_size, unsigned int location, int item_size);
        template <typename T>
        void addInterleavedAttributes(T *data, size_t data_size, const LocItemsizePairList &loc_itemsize_list);
        void setIndices(unsigned int *indices, size_t indices_size);
        void setBeginMode(GLBeginMode mode);
        void create();
        bool isCreated() const;
        unsigned int id() const;
        int getIndicesSize() const;
        void draw();
        void drawInstanced(int instance_count);
        static const SharedPtr &quad();
        static const SharedPtr &cube();
        static const SharedPtr &sphere();
        const PositionList &getPositions() const;
        template <ShapeConcept Shape>
        const Shape *getBoundingShape() const;
    private:
        static Geometry generateSphere(int x_segments = 128, int y_segments = 128);
        void readPositions(void *data, size_t total_bytes, int stride_bytes, int offset_bytes);
    private:
        bool m_created;
        size_t m_items_cnt;
        std::unique_ptr<GLVAO> m_vao;
        VBOList m_vbo_list;
        BufferDataList m_buffer_data_list;
        InterleavedAttributeInfoList m_interleaved_attribute_info_list;
        GLBuffer m_ebo;
        IndexList m_indices;
        GLBeginMode m_mode;
        PositionList m_positions;
        BoundingShapeMap m_bounding_shape_map;
    };
}

template <typename T>
inline void lcf::Geometry::addAttribute(T *data, size_t data_size, unsigned int location, int item_size)
{
    if (not data or data_size == 0) { return; }
    if (m_items_cnt == 0) {
        m_items_cnt = data_size / item_size;
    } else if (m_items_cnt != data_size / item_size) {
        qDebug() << "Data does not match the number of items.";
    }
    m_interleaved_attribute_info_list.emplace_back(LocItemsizePair(location, item_size), T{});
    m_vbo_list.emplace_back(GLBuffer::VertexBuffer);
    auto &buffer_data = m_buffer_data_list.emplace_back(std::vector<unsigned char>(data_size * sizeof(T)));
    memcpy(buffer_data.data(), data, data_size * sizeof(T));
}

template <typename T>
inline void lcf::Geometry::addInterleavedAttributes(T *data, size_t data_size, const LocItemsizePairList &loc_itemsize_list)
{
    if (not data or data_size == 0) { return; }
    int total_size = 0;
    for (const auto &[location, item_size] : loc_itemsize_list) { total_size += item_size; }
    if (m_items_cnt == 0) {
        m_items_cnt = data_size / total_size;
    } else if (m_items_cnt != data_size / total_size) {
         qDebug() << "Data does not match the number of items.";
    }
    m_interleaved_attribute_info_list.emplace_back(loc_itemsize_list, T{});
    m_vbo_list.emplace_back(GLBuffer::VertexBuffer);
    auto &buffer_data = m_buffer_data_list.emplace_back(std::vector<unsigned char>(data_size * sizeof(T)));
    memcpy(buffer_data.data(), data, data_size * sizeof(T));
}

template <lcf::ShapeConcept Shape>
inline const Shape *lcf::Geometry::getBoundingShape() const
{
    auto it = m_bounding_shape_map.find(Shape::getStaticTypeValue());
    if (it == m_bounding_shape_map.end()) { return nullptr; }
    return static_cast<const Shape *>(it->second.get());
}
