#pragma once
#include <vector>
#include <memory>

namespace lcf {
    class UniformBufferObject
    {
    public:
        using SizeList = std::vector<long long>;
        UniformBufferObject() = default;
        ~UniformBufferObject();
        void setBindingPoint(unsigned int binding_point);
        void setDataSizes(const SizeList& sizes);
        operator bool() const;
        void create();
        bool isCreated() const;
        void bind();
        void release();
        void updateData(int index, const void* data);
    private:
        unsigned int m_binding_point = 0;
        unsigned int m_ubo = 0;
        SizeList m_sizes;
        SizeList m_offsets;
        long long m_total_size = 0;
    };
}