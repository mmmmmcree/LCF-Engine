#pragma once

#include "Light.h"
#include "my_math.h"
#include "MyUniform.h"

namespace lcf {
    class SpotLight : public Light
    {
    public:
        using SharedPtr = std::shared_ptr<SpotLight>;
        static SharedPtr createShared();
        SpotLight() = default;
        LightType lightType() const override;
        void setName(std::string_view name) override;
        void setInnerAngle(float angle_deg);
        void setOuterAngle(float angle_deg);
    private:  
        MySingleUniform m_cos_inner = MySingleUniform(qCos(qDegreesToRadians(15.0f)));
        MySingleUniform m_cos_outer = MySingleUniform(qCos(qDegreesToRadians(20.0f)));
    };
}