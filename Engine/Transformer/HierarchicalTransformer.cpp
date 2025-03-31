#include "HierarchicalTransformer.h"

lcf::HierarchicalTransformer::HierarchicalTransformer(const HierarchicalTransformer &other) :
    Transformer(other),
    Hierarchical(other)
{
}

lcf::HierarchicalTransformer &lcf::HierarchicalTransformer::operator=(const HierarchicalTransformer &other)
{
    Transformer::operator=(other);
    Hierarchical::operator=(other);
    return *this;
}

const lcf::Matrix4x4 &lcf::HierarchicalTransformer::getWorldMatrix() const
{
    if (not m_need_update) { return m_hierarchial_matrix; }
    return m_hierarchial_matrix =
        m_parent ? m_parent->getWorldMatrix() * this->getLocalMatrix() : this->getLocalMatrix();
}

lcf::Vector3D lcf::HierarchicalTransformer::getWorldPosition() const
{
    return this->getWorldMatrix().column(3).toVector3D();
}

lcf::Vector3D lcf::HierarchicalTransformer::getWorldScale() const
{
    float x = this->getWorldMatrix().column(0).length();
    float y = this->getWorldMatrix().column(1).length();
    float z = this->getWorldMatrix().column(2).length();
    return { x, y, z };
}

void lcf::HierarchicalTransformer::attachTo(HierarchicalTransformer *parent)
{
    if (parent == this) { return; }
    this->requireUpdate();
    this->_setParent(parent);
}

const lcf::HierarchicalTransformer::Children &lcf::HierarchicalTransformer::getChildrenTransformer() const
{
    return m_children;
}

lcf::HierarchicalTransformer::Parent lcf::HierarchicalTransformer::getRootTransformer() const
{
    return this->_getRoot();
}

void lcf::HierarchicalTransformer::requireUpdate()
{
    /*
    - 不处理矩阵更新，只向子结点传递更新需求，并标记自己需要更新
    - 如果自己已经被标记，说明已经向下传递过更新需求，并且该需求没有被处理，不需要再向下传递
    */
   if (m_need_update) { return; }
   m_need_update = true;
   for (auto child : m_children) {
       child->requireUpdate();
   }
}