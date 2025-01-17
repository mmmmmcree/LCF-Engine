#include "SignalSender.h"

void lcf::SignalSender::sendTransformUpdatedSignal() const
{
    emit transformUpdated();
}

void lcf::SignalSender::sendModelsUpdatedSignal(Model *model) const
{
    emit modelAdded(model);
}
