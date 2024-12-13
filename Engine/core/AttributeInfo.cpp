#include "AttributeInfo.h"

int lcf::AttributeInfo::itemSize() const
{
    return m_item_size;
}

int lcf::AttributeInfo::strideBytes() const
{
    return m_item_size * m_type_size;
}

size_t lcf::AttributeInfo::typeSize() const
{
    return m_type_size;
}

int lcf::AttributeInfo::GLType() const
{
    return m_gl_type;
}

unsigned int lcf::AttributeInfo::location() const
{
    return m_location;
}

const std::vector<lcf::AttributeInfo> &lcf::AttributeInfos::get() const
{
    return m_infos;
}

int lcf::AttributeInfos::itemSize(size_t index) const
{
    return m_infos[index].itemSize();
}

int lcf::AttributeInfos::strideBytes() const
{
    return m_stride_bytes;
}

size_t lcf::AttributeInfos::offset(size_t index) const
{
    return m_offsets.at(index);
}

int lcf::AttributeInfos::GLType() const
{
    return m_infos.front().GLType();
}

size_t lcf::AttributeInfos::typeSize() const
{
    return m_infos.front().typeSize();
}
