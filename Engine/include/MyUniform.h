#pragma once

#include <string>
#include <variant>
#include "Matrix.h"
#include <functional>

namespace lcf {
    class GLShaderProgram;


    class  MyUniform
    {
    public:
        MyUniform() = default;
        virtual ~MyUniform();
        MyUniform(const MyUniform &other);
        virtual void bind(GLShaderProgram *shader) const = 0;
        void setName(std::string_view name);
        const std::string &name() const;
    protected:
        std::string m_name;
    };

    using SingleUniformValue = std::variant<bool, int, float, Vector2D, Vector3D, Vector4D, Matrix2x2, Matrix3x3, Matrix4x4>;

    class MySingleUniform : public MyUniform
    {
    public:
        MySingleUniform() = default;
        MySingleUniform(const SingleUniformValue &value);
        MySingleUniform(const MySingleUniform &other);
        void bind(GLShaderProgram *shader) const override;
        void setValue(const SingleUniformValue &value);
        const SingleUniformValue &value() const;
        bool asBool() const;
        int asInt() const;
        float asFloat() const;
        const Vector2D &asVector2D() const;
        const Vector3D &asVector3D() const;
        const Vector4D &asVector4D() const;
        const Matrix2x2 &asMatrix2x2() const;
        const Matrix3x3 &asMatrix3x3() const;
        const Matrix4x4 &asMatrix4x4() const;
    private:
        SingleUniformValue m_value;
    };
}