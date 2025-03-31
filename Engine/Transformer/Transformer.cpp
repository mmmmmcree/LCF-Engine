#include "Transformer.h"
#include "my_math.h"

lcf::Transformer::UniquePtr lcf::Transformer::createUnique()
{
    return std::make_unique<Transformer>();
}

lcf::Transformer::Transformer(const Transformer &other) :
    m_need_update(true),
    m_scale(other.m_scale)
{
}

lcf::Transformer &lcf::Transformer::operator=(const Transformer &other)
{
    this->requireUpdate();
    m_scale = other.m_scale;
    return *this;
}

void lcf::Transformer::setLocalMatrix(const Matrix4x4 &matrix)
{
    m_matrix = matrix;
    this->requireUpdate();
}

const lcf::Matrix4x4 &lcf::Transformer::getLocalMatrix() const
{
    m_need_update = false;
    return m_matrix;
}

void lcf::Transformer::translateWorld(float x, float y, float z)
{
    this->translateWorld(Vector3D(x, y, z));
}

void lcf::Transformer::translateWorld(const Vector3D &translation)
{
    if (translation.isNull()) { return; }
    m_matrix.setColumn(3, Vector4D(this->getTranslation() + translation, 1.0f));
    this->requireUpdate();
}

void lcf::Transformer::translateLocal(float x, float y, float z)
{
    this->translateLocal(Vector3D(x, y, z));
}

void lcf::Transformer::translateLocal(const Vector3D &translation)
{
    // if (translation.isNull()) { return; }
    // m_matrix.translate(translation);
    m_matrix.translateLocal(translation);
    this->requireUpdate();
}

void lcf::Transformer::translateLocalXAxis(float x)
{
    this->translateLocal({x, 0.0f, 0.0f});
    // // m_matrix.translate(x, 0.0f, 0.0f);
    // m_matrix.translateLocalX(x);
    // this->requireUpdate();
}

void lcf::Transformer::translateLocalYAxis(float y)
{
    this->translateLocal({0.0f, y, 0.0f});
    // m_matrix.translate(0.0f, y, 0.0f);
    // m_matrix.translateLocalY(y);
    // this->requireUpdate();
}

void lcf::Transformer::translateLocalZAxis(float z)
{
    this->translateLocal({0.0f, 0.0f, z});
    // m_matrix.translate(0.0f, 0.0f, z);
}

void lcf::Transformer::rotateLocal(const Quaternion &rotation)
{
    if (rotation.isNull() or rotation.isIdentity()) { return; }
    // m_matrix.rotate(rotation.normalized());
    m_matrix.rotateLocal(rotation.normalized());
    this->requireUpdate();
}

void lcf::Transformer::rotateLocalXAxis(float angle_degrees)
{
    this->rotateLocal(Quaternion::fromAxisAndAngle(this->getXAxis(), angle_degrees));
}

void lcf::Transformer::rotateLocalYAxis(float angle_degrees)
{
    this->rotateLocal(Quaternion::fromAxisAndAngle(this->getYAxis(), angle_degrees));
}

void lcf::Transformer::rotateLocalZAxis(float angle_degrees)
{
    this->rotateLocal(Quaternion::fromAxisAndAngle(this->getZAxis(), angle_degrees));
}

void lcf::Transformer::rotateAround(const Quaternion &rotation, const Vector3D &position)
{
    m_matrix.rotateAround(rotation, position);
    this->requireUpdate();
}

void lcf::Transformer::rotateAroundOrigin(const Quaternion &rotation)
{
    m_matrix.rotateAroundOrigin(rotation);
    this->requireUpdate();
}

void lcf::Transformer::rotateAroundSelf(const Quaternion &rotation)
{
    m_matrix.rotateAroundSelf(rotation);
    this->requireUpdate();
}

void lcf::Transformer::scale(float factor)
{
    this->scale(Vector3D(factor, factor, factor));
}

void lcf::Transformer::scale(float x, float y, float z)
{
    this->scale(Vector3D(x, y, z));
}

void lcf::Transformer::scale(const Vector3D &scale)
{
    if (scale == Vector3D(1.0f, 1.0f, 1.0f)) { return; }
    Vector3D s = max(scale, std::numeric_limits<float>::epsilon());
    m_scale *= s;
    m_matrix.scale(s);
    this->requireUpdate();
}

void lcf::Transformer::setTranslation(float x, float y, float z)
{
    this->setTranslation(Vector3D(x, y, z));
}

void lcf::Transformer::setTranslation(const Vector3D &position)
{
    m_matrix.setColumn(3, Vector4D(position, 1.0f));
    this->requireUpdate();
}

void lcf::Transformer::setRotation(const Quaternion &rotation)
{
    // auto mat = Matrix4x4(rotation.toRotationMatrix());
    // for (int i = 0; i < 3; ++i) {
    //     m_matrix.setColumn(i, mat.column(i) * m_scale[i]);
    // }
    m_matrix.setRotation(rotation);
    this->requireUpdate();
}

void lcf::Transformer::setRotation(float angle_degrees, float x, float y, float z)
{
    this->setRotation(angle_degrees, Vector3D(x, y, z));
}

void lcf::Transformer::setRotation(float angle_degrees, const Vector3D &axis)
{
    this->setRotation(Quaternion::fromAxisAndAngle(axis.normalized(), angle_degrees));
}

void lcf::Transformer::setScale(float x, float y, float z)
{
    this->setScale(Vector3D(x, y, z));
}

void lcf::Transformer::setScale(const Vector3D &scale)
{
    if (m_scale == scale) { return; }
    m_scale = max(scale, std::numeric_limits<float>::epsilon());
    for (int i = 0; i < 3; ++i) {
        m_matrix.setColumn(i, m_matrix.column(i) * m_scale[i]);
    }
    this->requireUpdate();
}

void lcf::Transformer::setScale(float factor)
{
    this->setScale(Vector3D(factor, factor, factor));
}

void lcf::Transformer::setTRS(const Vector3D &translation, const Quaternion &rotation, const Vector3D &scale)
{
    m_scale = max(scale, std::numeric_limits<float>::epsilon());
    this->setTranslation(translation);
    this->setRotation(rotation);
    this->setScale(scale);
}

lcf::Vector3D lcf::Transformer::getXAxis() const
{
    return this->getLocalMatrix().column(0).toVector3D();
}

lcf::Vector3D lcf::Transformer::getYAxis() const
{
    return this->getLocalMatrix().column(1).toVector3D();
}

lcf::Vector3D lcf::Transformer::getZAxis() const
{
    return this->getLocalMatrix().column(2).toVector3D();
}

lcf::Vector3D lcf::Transformer::getTranslation() const
{
    return m_matrix.column(3).toVector3D();
}

lcf::Quaternion lcf::Transformer::getRotation() const
{
    return m_matrix.getRotation();
    // Matrix4x4 mat;
    // for (int i = 0; i < 3; ++i) {
    //     mat.setColumn(i, this->getLocalMatrix().column(i) / m_scale[i]);
    // }
    // return Quaternion::fromRotationMatrix(mat.toGenericMatrix<3, 3>());
}

lcf::Vector3D lcf::Transformer::getScale() const
{
    return m_scale;
}

bool lcf::Transformer::isUpdated() const
{
    return not m_need_update;
}

btTransform lcf::Transformer::toBtTransform() const
{
    return btTransform(toBtQuaternion(this->getRotation()), toBtVector3(this->getTranslation()));
}

void lcf::Transformer::requireUpdate()
{
    m_need_update = true;
}