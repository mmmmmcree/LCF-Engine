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
        AttributeInfo(unsigned int location, int item_size, T type_check);
        int itemSize() const;
        int strideBytes() const;
        size_t typeSize() const;
        int GLType() const;
        unsigned int location() const;
    private:
        unsigned int m_location;
        int m_item_size;
        int m_type_size;
        int m_gl_type;
    };

    class AttributeInfos
    {
    public:
        using LocItemsizePair = std::pair<unsigned int, int>;
        using LocItemsizePairList = std::initializer_list<LocItemsizePair>;
        using AttributeInfoList = std::vector<AttributeInfo>;
        using OffsetList = std::vector<size_t>;
        template <typename T>
        AttributeInfos(const LocItemsizePairList &loc_itemsize_list, T type_check);
        const AttributeInfoList &get() const;
        int itemSize(size_t index) const;
        int strideBytes() const;
        size_t offset(size_t index) const;
        int GLType() const;
        size_t typeSize() const;
    private:
        AttributeInfoList m_infos;
        OffsetList m_offsets;
        int m_stride_bytes;
    };
}

template <typename T>
inline lcf::AttributeInfo::AttributeInfo(unsigned int location, int item_size, T type_check)
{
    m_location = location;
    m_item_size = item_size;
    m_type_size = sizeof(T);
    m_gl_type = GLTypeMapper<T>::type;
}

template <typename T>
inline lcf::AttributeInfos::AttributeInfos(const LocItemsizePairList &loc_itemsize_list, T type_check)
{
    m_stride_bytes = 0;
    for (const auto &[location, item_size] : loc_itemsize_list) {
        auto &info = m_infos.emplace_back(location, item_size, type_check);
        m_offsets.emplace_back(m_stride_bytes);
        m_stride_bytes += info.strideBytes();
    }
}