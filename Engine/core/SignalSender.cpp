#include "SignalSender.h"

lcf::SignalSender *lcf::SignalSender::instance()
{
    static SignalSender s_instance;
    return &s_instance;
}

void lcf::SignalSender::sendTransformUpdatedSignal() const
{
    emit transformUpdated();
}

void lcf::SignalSender::sendModelAddedSignal(Model *model) const
{
    emit modelAdded(model);
}

void lcf::SignalSender::sendRigidBodyCreatedSignal(RigidBody *body) const
{
    emit rigidBodyCreated(body);
}

void lcf::SignalSender::sendRigidBodyDestroyedSignal(RigidBody *body) const
{
    emit rigidBodyDestroyed(body);
}
