#include "Object3DHierarchicalTransformer.h"

lcf::Object3DHierarchicalTransformer::Object3DHierarchicalTransformer(const HierarchicalTransformer &other) :
    HierarchicalTransformer(other),
    m_inverted_need_update(true)
{
}

lcf::Object3DHierarchicalTransformer &lcf::Object3DHierarchicalTransformer::operator=(const HierarchicalTransformer &other)
{
    HierarchicalTransformer::operator=(other);
    m_inverted_need_update = true;
    return *this;
}

const lcf::Matrix4x4 &lcf::Object3DHierarchicalTransformer::getInvertedHierarchialMatrix() const
{
    if (not m_inverted_need_update) { return m_inverted_hierarchial_matrix; }
    m_inverted_need_update = false;
    return m_inverted_hierarchial_matrix = this->getWorldMatrix().inverted();
}

lcf::Matrix3x3 lcf::Object3DHierarchicalTransformer::getNormalMatrix() const
{
    // return this->getInvertedHierarchialMatrix().transposed().toGenericMatrix<3, 3>();
    return this->getInvertedHierarchialMatrix().transposed().toMatrix3x3();
}

void lcf::Object3DHierarchicalTransformer::requireUpdate()
{
    HierarchicalTransformer::requireUpdate();
    m_inverted_need_update = true;
}