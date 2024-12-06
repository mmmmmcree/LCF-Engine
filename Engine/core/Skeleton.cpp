#include "Skeleton.h"


lcf::Skeleton::Skeleton(BonePtrs &&bones, const MatricesPtr &offset_matrices) :
    m_bones(std::move(bones)),
    m_offset_matrices(offset_matrices)
{
    if (m_bones.size()!= m_offset_matrices->size()) {
        qDebug() << "Error: Skeleton::Skeleton() - Bone and offset matrix sizes do not match.";
    }
}

lcf::Skeleton::Skeleton(const Skeleton &other) :
    m_bones(other.m_bones.size()),
    m_offset_matrices(other.m_offset_matrices)
{
    for (int i = 0; i < m_bones.size(); ++i) {
        if (not other.m_bones[i]) { continue; }
        m_bones[i] = std::make_unique<Bone>(*other.m_bones[i]);
    }
}

const lcf::Skeleton::BonePtrs &lcf::Skeleton::bones() const
{
    return m_bones;
}

const lcf::Skeleton::MatricesPtr &lcf::Skeleton::offsetMatrices() const
{
    return m_offset_matrices;
}

lcf::Skeleton::Matrices lcf::Skeleton::boneMatrices() const
{
    Matrices matrices(m_offset_matrices->size());
    for (int i = 0; i < m_offset_matrices->size(); ++i) {
        matrices[i] = m_bones[i]->worldMatrix() * m_offset_matrices->at(i);
    }
    return matrices;
}
