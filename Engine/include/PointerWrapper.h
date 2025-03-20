#pragma once

#include <variant>
#include <memory>
#include <concepts>

namespace lcf {
    template <typename T>
    class PointerWrapper
    {
    public:
        using Impl = std::variant<std::unique_ptr<T>, std::shared_ptr<T>, T *>;
        using ValueType = T;
        PointerWrapper() = default;
        template <typename U> requires std::derived_from<U, T>
        PointerWrapper(std::unique_ptr<U> &&ptr) : m_impl(std::unique_ptr<T>(std::move(ptr))) { }
        PointerWrapper(const std::unique_ptr<T> &ptr) : m_impl(std::make_unique<T>(*ptr)) { }
        PointerWrapper(const std::shared_ptr<T> &ptr) : m_impl(ptr) { }
        PointerWrapper(T *ptr) : m_impl(ptr) { }
        PointerWrapper(const PointerWrapper &other);
        PointerWrapper(PointerWrapper &&other) = default;
        PointerWrapper &operator=(const PointerWrapper &other);
        operator bool() const;
        T *get() const;
        T *operator->() const;
        T &operator*() const;
    private:
        Impl m_impl = nullptr;
    };
}

template <typename T>
lcf::PointerWrapper<T>::PointerWrapper(const PointerWrapper &other)
{
    std::visit([&](auto &&ptr) {
        if constexpr (std::is_copy_constructible_v<std::decay_t<decltype(ptr)>>) {
            m_impl = ptr;
        } else {
            m_impl = std::make_unique<T>(*ptr);
        }
    }, other.m_impl);
}

template <typename T>
T *lcf::PointerWrapper<T>::operator->() const
{
    return this->get();
}

template <typename T>
T &lcf::PointerWrapper<T>::operator*() const
{
    return *(this->operator->());
}

template <typename T>
lcf::PointerWrapper<T> &lcf::PointerWrapper<T>::operator=(const PointerWrapper &other)
{
    std::visit([&](auto &&ptr) {
        if constexpr (std::is_copy_assignable_v<std::decay_t<decltype(ptr)>>) {
            m_impl = ptr;
        } else {
            m_impl = std::make_unique<T>(*ptr);
        }
    }, other.m_impl);
    return *this;
}

template <typename T>
inline lcf::PointerWrapper<T>::operator bool() const
{
    return m_impl;
}

template <typename T>
inline T *lcf::PointerWrapper<T>::get() const
{
    T *result = nullptr;
    std::visit([&](auto &&ptr) {
        if constexpr (std::is_pointer_v<std::decay_t<decltype(ptr)>>) {
            result = ptr;
        } else {
            result = ptr.get();
        }
    }, m_impl);
    return result;
}
