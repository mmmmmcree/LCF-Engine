#include "AnimationPlayer.h"


lcf::AnimationPlayer::AnimationList &lcf::AnimationPlayer::playList()
{
    return m_animations;
}

bool lcf::AnimationPlayer::hasAnimation() const
{
    return not m_animations.empty();
}

void lcf::AnimationPlayer::play(int index, float speed)
{
    m_playing_index = index;
    m_playing = true;
    m_speed = speed;
}

void lcf::AnimationPlayer::play()
{
    m_playing = true;
}

void lcf::AnimationPlayer::stop()
{
    m_playing = false;
}

void lcf::AnimationPlayer::update(float delta_time)
{
    if (not m_playing or m_playing_index < 0 or m_playing_index >= static_cast<int>(m_animations.size())) { return; }
    auto &animation = m_animations[m_playing_index];
    float delta_ticks = delta_time * animation->ticksPerSecond() * m_speed;
    m_current_time = fmod(m_current_time + delta_ticks, animation->duration());
    animation->update(m_current_time);
}

bool lcf::AnimationPlayer::isPlaying() const
{
    return m_playing;
}

int lcf::AnimationPlayer::playingIndex() const
{
    if (m_playing) { return m_playing_index; }
    return -1;
}
