#pragma once

#include "Animation.h"

namespace lcf {
    class AnimationPlayer
    {
    public:
        using AnimationPlayList = std::vector<std::unique_ptr<Animation>>;
        AnimationPlayer() = default;
        AnimationPlayer(const AnimationPlayer &) = delete;
        AnimationPlayList &playList();
        bool hasAnimation() const;
        void play(int index, float speed = 1.0f);
        void play();
        void stop();
        void update(float delta_time);
        bool isPlaying() const;
    private:
        AnimationPlayList m_animations;
        bool m_playing = false;
        float m_current_time = 0.0f;
        float m_speed = 1.0f;
        int m_playing_index = -1;
    };
}