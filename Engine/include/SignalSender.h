#pragma once

#include <QObject>

namespace lcf {
    class Model;

    class SignalSender : public QObject
    {
        Q_OBJECT
    public:
        SignalSender() = default;
        void sendTransformUpdatedSignal() const;
        void sendModelsUpdatedSignal(Model *model) const;
    signals:
        void transformUpdated() const;
        void modelAdded(Model *model) const;
    };
}