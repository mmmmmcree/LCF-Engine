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

const std::vector<lcf::AttributeInfo> &lcf::InterleavedAttributeInfo::get() const
{
    return m_infos;
}

int lcf::InterleavedAttributeInfo::itemSize(size_t index) const
{
    return m_infos[index].itemSize();
}

int lcf::InterleavedAttributeInfo::strideBytes() const
{
    return m_stride_bytes;
}

int lcf::InterleavedAttributeInfo::offset(size_t index) const
{
    return m_offsets.at(index);
}

int lcf::InterleavedAttributeInfo::GLType() const
{
    return m_infos.front().GLType();
}

size_t lcf::InterleavedAttributeInfo::typeSize() const
{
    return m_infos.front().typeSize();
}
