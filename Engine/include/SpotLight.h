#pragma once

#include "Light.h"
#include "my_math.h"

namespace lcf {
    class SpotLight : public Light
    {
    public:
        using SharedPtr = std::shared_ptr<SpotLight>;
        SpotLight() = default;
        UniformList asUniformList() const override;
        void setDirection(const Vector3D& direction);
        void setInnerAngle(float angle_deg);
        void setOuterAngle(float angle_deg);
    private:  
        Vector3D m_direction = {0.0f, 0.0f, -1.0f};
        float m_cos_inner = qCos(qDegreesToRadians(15.0f));
        float m_cos_outer = qCos(qDegreesToRadians(20.0f));
    };
}