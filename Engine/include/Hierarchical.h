#pragma once

#include <vector>
#include <ranges>

namespace lcf {
    template<typename T>
    concept Pointer = std::is_pointer_v<T>;

    template <Pointer T>
    class Hierarchical
    {
    public:
        using Parent = T ;
        using Child = T;
        using Children = std::vector<Child>;
    protected:
        Hierarchical() = default;
        virtual ~Hierarchical();
        Hierarchical(const Hierarchical &);
        Hierarchical &operator=(const Hierarchical &);
        void _setParent(Parent parent);
        void _addChild(Child child);
        void _removeChild(Child child);
        void _unregisterFromParent();
        Parent _getRoot() const;
    protected:
        Parent m_parent = nullptr;
        Children m_children;
    };
}

template <lcf::Pointer T>
inline lcf::Hierarchical<T>::~Hierarchical()
{
    this->_unregisterFromParent();
    for (auto child : m_children) {
        child->_setParent(nullptr);
    }
    m_children.clear();
}

template <lcf::Pointer T>
inline lcf::Hierarchical<T>::Hierarchical(const Hierarchical &) :
    m_parent(nullptr),
    m_children()
{
}

template <lcf::Pointer T>
inline lcf::Hierarchical<T> &lcf::Hierarchical<T>::operator=(const Hierarchical &)
{
    this->_unregisterFromParent();
    return *this;
}

template <lcf::Pointer T>
inline void lcf::Hierarchical<T>::_setParent(Parent parent)
{
    this->_unregisterFromParent();
    m_parent = parent;
    if (m_parent) {
        m_parent->_addChild(static_cast<Child>(this));
    }
}

template <lcf::Pointer T>
inline void lcf::Hierarchical<T>::_addChild(Child child)
{
    if (not child or child == this) { return; }
    auto it = std::ranges::find(m_children, child);
    if (it != m_children.end()) { return; }
    m_children.push_back(child);
}

template <lcf::Pointer T>
inline void lcf::Hierarchical<T>::_removeChild(Child child)
{
    if (not child or child == this) { return; }
    auto it = std::ranges::find(m_children, child);
    if (it == m_children.end()) { return; }
    m_children.erase(it);
}

template <lcf::Pointer T>
inline void lcf::Hierarchical<T>::_unregisterFromParent()
{
    if (not m_parent) { return; }
    m_parent->_removeChild(static_cast<Child>(this));
    m_parent = nullptr;
}

template <lcf::Pointer T>
inline lcf::Hierarchical<T>::Parent lcf::Hierarchical<T>::_getRoot() const
{
    if (not m_parent) { return static_cast<Parent>(const_cast<lcf::Hierarchical<T>*>(this)); }
    return m_parent->_getRoot();
}