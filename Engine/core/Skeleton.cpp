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

void lcf::Skeleton::bind(GLShaderProgram *shader)
{
    static QString uniform_name = "bone_matrices[%1]";
    for (int i = 0; i < m_bones.size(); ++i) {
        shader->setUniformValue(uniform_name.arg(i).toLocal8Bit(),
            m_bones[i]->worldMatrix() * m_offset_matrices->at(i)
        );
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
