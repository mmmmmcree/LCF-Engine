#pragma once

#include "Light.h"
#include "my_math.h"

namespace lcf {
    class SpotLight : public Light
    {
    public:
        using SharedPtr = std::shared_ptr<SpotLight>;
        SpotLight() = default;
        LightType lightType() const override;
        UniformList asUniformList() override;
        void setInnerAngle(float angle_deg);
        void setOuterAngle(float angle_deg);
    private:  
        float m_cos_inner = qCos(qDegreesToRadians(15.0f));
        float m_cos_outer = qCos(qDegreesToRadians(20.0f));
    };
}