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
    m_inverted_need_update = true;
    return *this;
}

const lcf::Matrix4x4 &lcf::HierarchicalTransformer::getHierarchialMatrix()
{
    if (not m_need_update) { return m_hierarchial_matrix; }
    return m_hierarchial_matrix =
        m_parent ? m_parent->getHierarchialMatrix() * this->getMatrix() : this->getMatrix();
}

const lcf::Matrix4x4 &lcf::HierarchicalTransformer::getInvertedHierarchialMatrix()
{
    if (not m_inverted_need_update) { return m_inverted_hierarchial_matrix; }
    m_inverted_need_update = false;
    return m_inverted_hierarchial_matrix = this->getHierarchialMatrix().inverted();
}

lcf::Matrix3x3 lcf::HierarchicalTransformer::getNormalMatrix()
{
    return this->getInvertedHierarchialMatrix().transposed().toGenericMatrix<3, 3>();
}

lcf::Vector3D lcf::HierarchicalTransformer::getHierarchialPosition()
{
    return this->getHierarchialMatrix().column(3).toVector3D();
}

void lcf::HierarchicalTransformer::attachTo(HierarchicalTransformer *parent)
{
    m_need_update = true;
    m_inverted_need_update = true;
    this->_setParent(parent);
}

void lcf::HierarchicalTransformer::requireUpdate()
{
    /*
    - 不处理矩阵更新，只向子结点传递更新需求，并标记自己需要更新
    - 如果自己已经被标记，说明已经向下传递过更新需求，并且该需求没有被处理，不需要再向下传递
    */
    if (m_need_update) { return; }
    m_need_update = true;
    m_inverted_need_update = true;
    for (auto child : m_children) {
        child->requireUpdate();
    }
}