#include "PhysicalWorld.h"
#include "RigidBody.h"
#include "SignalSender.h"

lcf::PhysicalWorld::UniquePtr lcf::PhysicalWorld::createUnique()
{
    return std::make_unique<lcf::PhysicalWorld>();
}

lcf::PhysicalWorld::PhysicalWorld()
{
    m_collision_config = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collision_config);
    m_broadphase = new btDbvtBroadphase();
    m_solver = new btSequentialImpulseConstraintSolver();
    m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collision_config);
    m_world->setGravity(btVector3(0, -1.0f, 0));
}

lcf::PhysicalWorld::~PhysicalWorld()
{
    delete m_world;
    delete m_broadphase;
    delete m_dispatcher;
    delete m_solver;
    delete m_collision_config;
}

void lcf::PhysicalWorld::activate(bool active)
{
    if (active) {
        connect(SignalSender::instance(), &SignalSender::rigidBodyCreated, this, &lcf::PhysicalWorld::addRigidBody);
    } else {
        disconnect(SignalSender::instance(), &SignalSender::rigidBodyCreated, this, nullptr);
    }
}

int lcf::PhysicalWorld::stepSimulation(float time_step, int max_sub_steps, float fixed_time_step)
{
    for (auto rigid_body : m_rigid_bodies) {
        rigid_body->checkUpdateFromTransformer();
    }
    int result = m_world->stepSimulation(time_step, max_sub_steps, fixed_time_step);
    for (auto rigid_body : m_rigid_bodies) {
        rigid_body->applyUpdateToTransformer();
    }
    return result;
}

void lcf::PhysicalWorld::addRigidBody(RigidBody *rigid_body)
{
    auto iter = std::ranges::find(m_rigid_bodies, rigid_body);
    if (iter != m_rigid_bodies.end()) { return; }
    m_rigid_bodies.emplace_back(rigid_body);
    m_world->addRigidBody(rigid_body->m_impl);
}
