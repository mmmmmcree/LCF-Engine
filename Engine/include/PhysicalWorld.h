#pragma once

#include "bullet/btBulletDynamicsCommon.h"
#include <memory>
#include <QObject>
#include <vector>
#include "RigidBody.h"

namespace lcf {
    class PhysicalWorld : public QObject
    {
        Q_OBJECT
    public:
        using UniquePtr = std::unique_ptr<PhysicalWorld>;
        static UniquePtr createUnique();
        using CollisionWorld = btDiscreteDynamicsWorld;
        using Dispatcher = btDispatcher;
        using CollisionConfiguration = btCollisionConfiguration;
        using BroadphaseInterface = btBroadphaseInterface;
        using ConstraintSolver = btConstraintSolver;
        using RigidBodyPtrList = std::vector<RigidBody *>;
        PhysicalWorld();
        ~PhysicalWorld();
        void activate(bool active);
        int stepSimulation(float time_step, int max_sub_steps = 1, float fixed_time_step = 1.0f / 60.0f);
        void addRigidBody(RigidBody *rigid_body);
    private:
        CollisionConfiguration *m_collision_config = nullptr;
        Dispatcher *m_dispatcher = nullptr;
        BroadphaseInterface *m_broadphase = nullptr;
        ConstraintSolver *m_solver = nullptr;
        CollisionWorld *m_world = nullptr;
        RigidBodyPtrList m_rigid_bodies;
    };
}