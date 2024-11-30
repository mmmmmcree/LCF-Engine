#pragma once

#include "Animation.h"

namespace lcf {
    class AnimationPlayer
    {
    public:
        AnimationPlayer() = default;
        void play(Animation *animation, float speed = 1.0f);
        void stop();
        void update(float delta_time);
    private:  
        bool m_playing = false;
        float m_current_time = 0.0f;
        float m_speed = 1.0f;
        Animation *m_animation = nullptr;
    };
}