#pragma once

#include <QObject>

namespace lcf {
    class Model;
    class RigidBody;
    class SignalSender : public QObject
    {
        Q_OBJECT
    public:
        static SignalSender *instance();
        void sendTransformUpdatedSignal() const;
        void sendModelAddedSignal(Model *model) const;
        void sendRigidBodyCreatedSignal(RigidBody *body) const;
        void sendRigidBodyDestroyedSignal(RigidBody *body) const;
    private:
        SignalSender() = default;
    signals:
        void transformUpdated() const;
        void modelAdded(Model *model) const;
        void rigidBodyCreated(RigidBody *body) const;
        void rigidBodyDestroyed(RigidBody *body) const;
    };
}