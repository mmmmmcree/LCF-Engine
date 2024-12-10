#pragma once

#include "Light.h"

namespace lcf {
    class DirectionalLight : public Light
    {
    public: 
        using SharedPtr = std::shared_ptr<DirectionalLight>;
        DirectionalLight() = default;
        UniformList asUniformList() const override;
        void setDirection(const Vector3D &direction);
    private:
        Vector3D m_direction = {0.0f, -1.0f, 0.0f};
    };
}