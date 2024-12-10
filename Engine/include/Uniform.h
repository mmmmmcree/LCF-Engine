#pragma once

#include <variant>
#include <string>
#include <vector>
#include <functional>
#include <optional>
#include <string_view>
#include "Matrix.h"
#include "Color.h"
#include "GLShaderProgram.h"

namespace lcf {
    
    class SingleUniform
    {
    public:
        using Value = std::variant<int, float, Vector2D, Vector3D, Vector4D, Color, Matrix2x2, Matrix3x3, Matrix4x4>;
        using ValueGenerator = std::function<Value()>;
        SingleUniform(std::string_view name, const Value &value);
        SingleUniform(std::string_view name, const ValueGenerator &generator);
        void bind(GLShaderProgram *shader);
        const std::string &name() const;
        void setLocation(int location);
    private:
        std::string m_name;
        int m_location = -1;
        Value m_value;
        std::optional<ValueGenerator> m_generator;
    };

    class ArrayUniform
    {
    public:
        using Values = std::variant<std::vector<int>, std::vector<Vector4D>, std::vector<Matrix4x4>>;
        using ValuesGenerator = std::function<Values()>;
        ArrayUniform(std::string_view name, const Values &values);
        ArrayUniform(std::string_view name, const ValuesGenerator &generator);
        void bind(GLShaderProgram *shader);
        const std::string &name() const;
        void setLocation(unsigned int location);
    private:
        std::string m_name;
        unsigned int m_location = -1;
        Values m_values;
        std::optional<ValuesGenerator> m_generator;
    };

    using Uniform = std::variant<SingleUniform, ArrayUniform>;
    using UniformList = std::vector<Uniform>;
}