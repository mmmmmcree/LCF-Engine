#pragma once

#include <concepts>

namespace lcf {
    template <typename T>
    concept EnumTypeConcept = std::is_enum_v<T>;

    template <EnumTypeConcept EnumType>
    struct TypeConsistencyChecker
    {
        virtual EnumType getTypeValue() const = 0;
        virtual bool isTypeConsistent() const = 0;
    };

    template <EnumTypeConcept EnumType, EnumType EnumValue, typename BaseClass>
    struct TypeConsistencyCheckerMixin : public BaseClass
    {
        using BaseConstructor = TypeConsistencyCheckerMixin<EnumType, EnumValue, BaseClass>;
        using Base = BaseClass;
        static constexpr EnumType getStaticTypeValue() { return EnumValue; }
        EnumType getTypeValue() const override { return EnumValue; }
        bool isTypeConsistent() const override { return getStaticTypeValue() == this->getTypeValue(); }
    };
}