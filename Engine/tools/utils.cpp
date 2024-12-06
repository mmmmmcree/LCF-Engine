#include "utils.h"
#include <QRandomGenerator>

float lcf::utils::elapsed_time_s()
{
    return elapsed_time_ms() / 1000.0f;
}

float lcf::utils::elapsed_time_ms()
{
    if (not s_elapsed_timer) {
        s_elapsed_timer = std::make_unique<QElapsedTimer>();
        s_elapsed_timer->start();
    }
    return s_elapsed_timer->elapsed();
}

float lcf::utils::random_bounded(float min, float max)
{
    float d = QRandomGenerator::global()->generateDouble();
    return min + d * (max - min);
}

float lcf::utils::random_bounded(float max)
{
    return QRandomGenerator::global()->generateDouble() * max;
}
