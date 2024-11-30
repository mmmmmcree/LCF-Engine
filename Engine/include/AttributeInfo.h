#pragma once

#include <string>
#include <vector>
#include <QDebug>
#include "Define.h"

namespace lcf {
    class AttributeInfo
    {
    public:
        template <typename T>
        AttributeInfo(int item_size, T type_check);
        int itemSize() const;
        int strideBytes() const;
        size_t typeSize() const;
        int GLType() const;
    private:
        int m_item_size;
        int m_type_size;
        int m_gl_type;
    };

    class AttributeInfos
    {
    public:
        template <typename T>
        AttributeInfos(const std::initializer_list<int> &item_sizes, T type_check);
        const std::vector<AttributeInfo> &get() const;
        int itemSize(size_t index) const;
        int strideBytes() const;
        size_t offset(size_t index) const;
        int GLType() const;
        size_t typeSize() const;
    private:
        std::vector<AttributeInfo> m_infos;
        std::vector<size_t> m_offsets;
        int m_stride_bytes;
    };
}

template <typename T>
inline lcf::AttributeInfo::AttributeInfo(int item_size, T type_check)
{
    m_item_size = item_size;
    m_type_size = sizeof(T);
    m_gl_type = GLTypeMapper<T>::type;
}

template <typename T>
inline lcf::AttributeInfos::AttributeInfos(const std::initializer_list<int> &item_sizes, T type_check)
{
    if (item_sizes.size() == 0) {
        qDebug() << "AttributeInfos: item_sizes is empty";
        return;
    }
    m_stride_bytes = 0;
    for (const auto &item_size : item_sizes) {
        auto &info = m_infos.emplace_back(item_size, type_check);
        m_offsets.emplace_back(m_stride_bytes);
        m_stride_bytes += info.strideBytes();
    }
}