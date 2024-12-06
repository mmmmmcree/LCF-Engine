#pragma once
#include <memory>
#include <QElapsedTimer>

namespace lcf::utils {
    static std::unique_ptr<QElapsedTimer> s_elapsed_timer = nullptr;

    float elapsed_time_s();

    float elapsed_time_ms();

    float random_bounded(float min, float max);

    float random_bounded(float max);
}