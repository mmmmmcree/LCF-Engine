#pragma once

#include "my_math.h"

namespace lcf {
    template <typename T>
    struct KeyFrame
    {
        KeyFrame(float t, const T &val);
        KeyFrame(const KeyFrame<T> &other) = default;
        KeyFrame(KeyFrame<T> &&other);
        friend bool operator<(const KeyFrame<T> &lhs, const KeyFrame<T> &rhs)
        { return lhs.time < rhs.time; }
        friend bool operator<(const KeyFrame<T> &key_frame, float time)
        { return key_frame.time < time; }
        float time;
        T value;
    };
}

template <typename T>
inline lcf::KeyFrame<T>::KeyFrame(float t, const T &val) :
    time(t), value(val)
{
}

template <typename T>
inline lcf::KeyFrame<T>::KeyFrame(KeyFrame<T> &&other) :
    time(other.time), value(std::move(other.value))
{
}
