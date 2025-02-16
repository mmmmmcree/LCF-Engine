#pragma once

#include "Uniform.h"
#include <vector>
#include "GLShaderProgram.h"
#include "Matrix.h"

namespace lcf {
    template <typename T>
    class ArrayUniform : public Uniform
    {
        static_assert(
            std::is_same_v<T, int> or
            std::is_same_v<T, float> or
            std::is_same_v<T, Vector2D> or
            std::is_same_v<T, Vector3D> or
            std::is_same_v<T, Vector4D> or
            std::is_same_v<T, Matrix2x2> or
            std::is_same_v<T, Matrix3x3> or
            std::is_same_v<T, Matrix4x4>,
            "Invalid type for ArrayUniform"
        );
    public:
        using UniformList = std::vector<T>;
        ArrayUniform() = default;
        void bind(GLShaderProgram *shader) const override;
    private:
        UniformList m_values;
    };

    template <>
    class ArrayUniform<float> : public Uniform
    {
    public:
        using UniformList = std::vector<float>;
        ArrayUniform(int tuple_size = 1);
        void bind(GLShaderProgram *shader) const override;
    private: 
        UniformList m_values; 
        int m_tuple_size;
    };
}

template <typename T>
inline void lcf::ArrayUniform<T>::bind(GLShaderProgram *shader) const
{
    if (not shader) { return; } 
    shader->setUniformValue(this->name().c_str(), m_values.data(), static_cast<int>(m_values.size()));
}

inline lcf::ArrayUniform<float>::ArrayUniform(int tuple_size) :
    Uniform(),
    m_tuple_size(tuple_size)
{
}

inline void lcf::ArrayUniform<float>::bind(GLShaderProgram *shader) const
{
    if (not shader) { return; }
    shader->setUniformValueArray(this->name().c_str(), m_values.data(), static_cast<int>(m_values.size()), m_tuple_size);
}