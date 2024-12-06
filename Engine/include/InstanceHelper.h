#pragma once

#include "Matrix.h"
#include <QList>

namespace lcf {
    class InstanceHelper
    {
    public:   
        using Matrices = QList<Matrix4x4>;
        using Data = QList<float>;
        InstanceHelper() = default;
        void bind();
        void addInstanceOffset(const Matrix4x4& matrix);
        void addInstanceData(const Matrix4x4* data, size_t size);
        void setInstanceOffset(size_t index, const Matrix4x4& matrix);
        int instanceCount() const;
    private:
        bool m_updated = false;
        size_t m_allocated_size = 0;
        unsigned int m_ssbo = 0;
        Data m_instance_offsets_data;
    };
}