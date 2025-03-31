#pragma once
#include "BufferDataDescription.h"

namespace lcf {
    class ShaderStorageBufferObject
    {
    public:
        using SizeList = std::initializer_list<int>;
        ShaderStorageBufferObject() = default;
        ~ShaderStorageBufferObject();
        void setBindingPoint(unsigned int binding_point);
        void setDataSizes(const SizeList& sizes);
        operator bool() const;
        bool create();
        bool isCreated() const;
        void bind();
        void release();
        void updateData(int index, int data_index, const void* data);
        void resize(int size);
        void fitToSize(int size);
        unsigned int getId() const;
    private:
        unsigned int m_binding_point = 0;
        unsigned int m_ssbo = 0;
        int m_ssbo_size = 3;
        BufferDataDescription m_data_description;
    };
}