#pragma once

#include "Uniform.h"
#include "Matrix.h"
#include "UniformManager.h"

namespace lcf {
    template <typename T>
    class SingleUniform : public Uniform
    {
        static_assert(
            std::is_same_v<T, bool> or
            std::is_same_v<T, int> or
            std::is_same_v<T, float> or
            std::is_same_v<T, Vector3D> or
            std::is_same_v<T, Vector4D> or
            std::is_same_v<T, Vector2D> or
            std::is_same_v<T, Matrix3x3> or
            std::is_same_v<T, Matrix4x4>,
            "Invalid type for SingleUniform"
        );
    public:
        SingleUniform() = default;
        SingleUniform(const T &value);
        SingleUniform(const SingleUniform &other);
        operator T() const;
        SingleUniform &operator=(const SingleUniform &other);
        SingleUniform &operator=(const T &value);
        void bind(GLShaderProgram *shader) const override;
        void setValue(const T &value);
        const T &value() const;
    private:
        T m_value;
    };
    
}

template <typename T>
inline lcf::SingleUniform<T>::SingleUniform(const T &value) :
    Uniform(),
    m_value(value)
{
}

template <typename T>
inline lcf::SingleUniform<T>::SingleUniform(const SingleUniform &other) :
    Uniform(other),
    m_value(other.m_value)
{
}

template <typename T>
inline lcf::SingleUniform<T>::operator T() const
{
    return m_value;
}

template <typename T>
inline lcf::SingleUniform<T> &lcf::SingleUniform<T>::operator=(const SingleUniform &other)
{
    this->setValue(other.m_value);
    return *this;
}

template <typename T>
inline lcf::SingleUniform<T> &lcf::SingleUniform<T>::operator=(const T &value)
{
    this->setValue(value);
    return *this;
}

template <typename T>
inline void lcf::SingleUniform<T>::bind(GLShaderProgram *shader) const
{
    if (not shader) { return; }
    shader->setUniformValue(this->name().c_str(), m_value);
}

template<typename T>
inline void lcf::SingleUniform<T>::setValue(const T & value)
{
    m_value = value;
    UniformManager::instance()->emitUniformUpdatedSignal(this);
}

template <typename T>
inline const T &lcf::SingleUniform<T>::value() const
{
    return m_value;
}
