#pragma once

#include "TypeConsistencyChecker.h"
#include <memory>

namespace lcf {
    enum class ComponentType
    {
        AnimationPlayer,
        RigidBody,
    };

    class Component : public TypeConsistencyChecker<ComponentType>
    {
    public:
        using UniquePtr = std::unique_ptr<Component>;
        virtual ~Component() { }
        virtual void update(float dt) { }
        virtual void update() { }
    };

    template <typename T>
    concept ComponentConcept = std::is_base_of_v<Component, T>;

    template <ComponentType EnumValue>
    using ComponentMixin = TypeConsistencyCheckerMixin<ComponentType, EnumValue, Component>;

    struct ComponentFactory
    {
        template <ComponentConcept T>
        static std::unique_ptr<T> createUnique() { return std::make_unique<T>(); }
    };
}