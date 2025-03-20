#include "RigidBody.h"
#include "ControlManager.h"
#include "SignalSender.h"

lcf::RigidBody::RigidBody() :
    m_owns_transformer(true),
    m_controlled_transformer(new Transformer),
    m_shape(new btBoxShape(btVector3(1, 1, 1))),
    m_motion_state(new btDefaultMotionState()),
    m_impl(new RigidBodyIMPL(1.0f, m_motion_state, m_shape))
{
    SignalSender::instance()->sendRigidBodyCreatedSignal(this);
}

void lcf::RigidBody::setControlledTransformer(Transformer *transformer)
{
    if (transformer == m_controlled_transformer) { return; }
    if (m_owns_transformer and not transformer) { return; }
    if (not transformer) {
        m_owns_transformer = true;
        m_controlled_transformer = new Transformer;
        return;
    }
    if (m_owns_transformer) {
        m_owns_transformer = false;
        delete m_controlled_transformer;
    }
    m_controlled_transformer = transformer;
}

lcf::RigidBody::~RigidBody()
{
    SignalSender::instance()->sendRigidBodyDestroyedSignal(this);
    delete m_impl;
    delete m_motion_state;
    delete m_shape;
}

void lcf::RigidBody::checkUpdateFromTransformer()
{
    m_shape->setLocalScaling(toBtVector3(m_controlled_transformer->getScale()));
    m_impl->setWorldTransform(m_controlled_transformer->toBtTransform());
    m_impl->activate();
}

void lcf::RigidBody::applyUpdateToTransformer()
{
    const auto &transform = m_impl->getWorldTransform();
    Vector3D delta_translation = toVector3D(transform.getOrigin()) - m_controlled_transformer->getPosition();
    Quaternion delta_rotation = toQuaternion(transform.getRotation()).inverted() * m_controlled_transformer->getRotation();
    m_controlled_transformer->translate(delta_translation);
    m_controlled_transformer->rotate(delta_rotation);
}

void lcf::RigidBody::setMass(float mass)
{
    btVector3 local_inertia(0, 0, 0);
    if (mass != 0.0f) {
        m_shape->calculateLocalInertia(mass, local_inertia);
    }
    m_impl->setMassProps(mass, local_inertia);
}

void lcf::RigidBody::setBoxHalfExtents(const Vector3D &extents)
{
    m_shape->setImplicitShapeDimensions(toBtVector3(extents));
}

void lcf::RigidBody::applyCentralImpulse(const Vector3D &impulse)
{
    m_impl->applyCentralImpulse(toBtVector3(impulse));
}