#pragma once

#include "bullet/btBulletDynamicsCommon.h"
#include "Vector.h"
#include "Quaternion.h"
#include "Transformer.h"
#include "Component.h"

namespace lcf {
    class PhysicalWorld;
    class RigidBody : public ComponentMixin<ComponentType::RigidBody>
    {
        friend PhysicalWorld;
    public:
        using CollisionShape = btBoxShape;
        using MotionState = btDefaultMotionState;
        using RigidBodyIMPL = btRigidBody;
        using Transform = btTransform;
        using UniquePtr = std::unique_ptr<RigidBody>;
        RigidBody();
        void setControlledTransformer(Transformer *transformer);
        ~RigidBody();
        void checkUpdateFromTransformer();
        void applyUpdateToTransformer();
        void setMass(float mass);
        void setBoxHalfExtents(const Vector3D& extents);
        void applyCentralImpulse(const Vector3D &impulse);
    private:
        bool m_owns_transformer;
        Transformer *m_controlled_transformer;
        CollisionShape *m_shape;
        MotionState *m_motion_state;
        RigidBodyIMPL *m_impl;
    };
}