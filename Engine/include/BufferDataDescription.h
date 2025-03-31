#pragma once

#include <vector>
#include <span>

namespace lcf {
    class BufferDataDescription
    {
    public:
        using SizeList = std::vector<int>;
        BufferDataDescription() = default;
        BufferDataDescription(std::span<const int> sizes);
        BufferDataDescription(const std::initializer_list<int> &sizes);
        operator bool() const;
        int getTotalSize() const;
        int getOffset(int index) const;
        int getSize(int index) const;
    private:
        SizeList m_sizes;
        SizeList m_offsets;
        int m_total_size = 0;
    };
}