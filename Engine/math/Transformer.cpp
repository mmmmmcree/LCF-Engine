#include "Transformer.h"
#include "my_math.h"

lcf::Transformer::UniquePtr lcf::Transformer::createUnique()
{
    return std::make_unique<Transformer>();
}

lcf::Transformer::Transformer(const Transformer &other) :
    m_need_update(UpdateType::All),
    m_position(other.m_position),
    m_rotation(other.m_rotation),
    m_scale(other.m_scale)
{
}

lcf::Transformer &lcf::Transformer::operator=(const Transformer &other)
{
    m_need_update = true;
    m_position = other.m_position;
    m_rotation = other.m_rotation;
    m_scale = other.m_scale;
    return *this;
}

void lcf::Transformer::setMatrix(const Matrix4x4 &matrix)
{
    this->decompose(matrix);
    m_matrix = matrix;
}

const lcf::Matrix4x4 &lcf::Transformer::getMatrix()
{
    if (not m_need_update) { return m_matrix; }
    if (m_need_update & UpdateType::All) {
        this->updateAll();
        m_need_update = UpdateType::None;
        return m_matrix;
    }
    if (m_need_update & UpdateType::Translated) {
        m_matrix.translate(m_delta_translation / m_scale);
        m_delta_translation = Vector3D(0.0f, 0.0f, 0.0f);
        m_need_update &= ~UpdateType::Translated;
    }
    if (m_need_update & UpdateType::Rotated) {
        m_matrix.rotate(m_delta_rotation);
        m_delta_rotation = Quaternion();
        m_need_update &= ~UpdateType::Rotated;
    }
    if (m_need_update & UpdateType::Scaled) {
        m_matrix.scale(m_delta_scale);
        m_delta_scale = Vector3D(1.0f, 1.0f, 1.0f);
        m_need_update &= ~UpdateType::Scaled;
    }
    return m_matrix;
}

void lcf::Transformer::translate(float x, float y, float z)
{
    this->translate(Vector3D(x, y, z));
}

void lcf::Transformer::translate(const Vector3D &translation)
{
    if (translation.isNull()) { return; }
    m_delta_translation += translation;
    m_position += m_rotation.rotatedVector(translation);
    this->requireUpdate(UpdateType::Translated);
}

void lcf::Transformer::rotate(float angle_degrees, float x, float y, float z)
{
    this->rotate(angle_degrees, Vector3D(x, y, z));
}

void lcf::Transformer::rotate(float angle_degrees, const Vector3D &axis)
{
    this->rotate(Quaternion::fromAxisAndAngle(axis, angle_degrees));
}

void lcf::Transformer::rotate(const Quaternion &rotation)
{
    if (rotation.isNull()) { return; }
    m_delta_rotation = rotation * m_delta_rotation;
    m_delta_rotation.normalize();
    m_rotation = rotation * m_rotation;
    m_rotation.normalize();
    this->requireUpdate(UpdateType::Rotated);
}

void lcf::Transformer::rotateXAxis(float angle_degrees)
{
    this->rotate(angle_degrees, this->getXAxis());
}

void lcf::Transformer::rotateYAxis(float angle_degrees)
{
    this->rotate(angle_degrees, this->getYAxis());
}

void lcf::Transformer::rotateZAxis(float angle_degrees)
{
    this->rotate(angle_degrees, this->getZAxis());
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
    m_delta_scale *= scale;
    m_delta_scale = max(m_delta_scale, std::numeric_limits<float>::epsilon());
    m_scale *= scale;
    m_scale = max(m_scale, std::numeric_limits<float>::epsilon());
    this->requireUpdate(UpdateType::Scaled);
}

void lcf::Transformer::setPosition(float x, float y, float z)
{
    this->setPosition(Vector3D(x, y, z));
}

void lcf::Transformer::setPosition(const Vector3D &position)
{
    if (m_position == position) { return; }
    m_position = position;
    this->requireUpdate(UpdateType::All);
}

void lcf::Transformer::setRotation(float angle_degrees, float x, float y, float z)
{
    this->setRotation(angle_degrees, Vector3D(x, y, z));
}

void lcf::Transformer::setRotation(float angle_degrees, const Vector3D &axis)
{
    this->setRotation(Quaternion::fromAxisAndAngle(axis, angle_degrees));
}

void lcf::Transformer::setRotation(const Quaternion &rotation)
{
    if (m_rotation == rotation) { return; }
    m_rotation = rotation;
    this->requireUpdate(UpdateType::All);
}

void lcf::Transformer::setRotationXAxis(float angle_degrees)
{
    this->setRotation(angle_degrees, this->getXAxis());
}

void lcf::Transformer::setRotationYAxis(float angle_degrees)
{
    this->setRotation(angle_degrees, this->getYAxis());
}

void lcf::Transformer::setRotationZAxis(float angle_degrees)
{
    this->setRotation(angle_degrees, this->getZAxis());
}

void lcf::Transformer::setScale(float x, float y, float z)
{
    this->setScale(Vector3D(x, y, z));
}

void lcf::Transformer::setScale(const Vector3D &scale)
{
    if (m_scale == scale) { return; }
    m_scale = scale;
    m_scale = max(m_scale, std::numeric_limits<float>::epsilon());
    this->requireUpdate(UpdateType::All);
}

void lcf::Transformer::setScale(float factor)
{
    this->setScale(Vector3D(factor, factor, factor));
}

lcf::Vector3D lcf::Transformer::getXAxis()
{
    return this->getMatrix().column(0).toVector3D();
}

lcf::Vector3D lcf::Transformer::getYAxis()
{
    return this->getMatrix().column(1).toVector3D();
}

lcf::Vector3D lcf::Transformer::getZAxis()
{
    return this->getMatrix().column(2).toVector3D();
}

const lcf::Vector3D &lcf::Transformer::getPosition() const
{
    return m_position;
}

const lcf::Quaternion &lcf::Transformer::getRotation() const
{
    return m_rotation;
}

const lcf::Vector3D &lcf::Transformer::getScale() const
{
    return m_scale;
}

bool lcf::Transformer::isUpdated() const
{
    return not m_need_update;
}

btTransform lcf::Transformer::toBtTransform() const
{
    return btTransform(toBtQuaternion(m_rotation), toBtVector3(m_position));
}

void lcf::Transformer::requireUpdate(UpdateType type)
{
    m_need_update |= type;
}

void lcf::Transformer::updateAll()
{
    m_matrix.setToIdentity();
    m_matrix.translate(m_position);
    m_matrix.rotate(m_rotation);
    m_matrix.scale(m_scale);
    m_delta_translation = Vector3D(0.0f, 0.0f, 0.0f);
    m_delta_rotation = Quaternion();
    m_delta_scale = Vector3D(1.0f, 1.0f, 1.0f);
}

void lcf::Transformer::decompose(const Matrix4x4 &matrix)
{
    m_position = matrix.column(3).toVector3D();
    for (int i = 0; i < 3; ++i) {
        m_scale[i] = matrix.column(i).toVector3D().length();
    }
    Matrix4x4 rotation_mat = matrix;
    for (int i = 0; i < 3; ++i) {
        if (m_scale[i] == 0.0f) { continue; }
        rotation_mat.setColumn(i, matrix.column(i) / m_scale[i]);
    }
    m_rotation = Quaternion::fromRotationMatrix(rotation_mat.toGenericMatrix<3, 3>());
    this->requireUpdate(UpdateType::All);
}
