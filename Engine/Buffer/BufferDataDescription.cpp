#include "BufferDataDescription.h"

lcf::BufferDataDescription::BufferDataDescription(std::span<const int> sizes) :
    m_sizes(sizes.begin(), sizes.end()), m_offsets(m_sizes.size()), m_total_size(0)
{
    for (int i = 0; i < sizes.size(); ++i) {
        m_offsets[i] = m_total_size;
        m_total_size += sizes[i];
    }
}

lcf::BufferDataDescription::BufferDataDescription(const std::initializer_list<int> &sizes) :
    BufferDataDescription(std::span(sizes))
{
}

lcf::BufferDataDescription::operator bool() const
{
    return m_total_size;
}

int lcf::BufferDataDescription::getTotalSize() const
{
    return m_total_size;
}

int lcf::BufferDataDescription::getOffset(int index) const
{
    return m_offsets.at(index);
}

int lcf::BufferDataDescription::getSize(int index) const
{
    return m_sizes.at(index);
}
