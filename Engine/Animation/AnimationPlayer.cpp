#include "AnimationPlayer.h"

// void lcf::AnimationPlayer::setBoneMap(BoneMap *bone_map)
// {
//     m_bone_map = bone_map;
// }

void lcf::AnimationPlayer::play(Animation *animation, float speed)
{
    if (not animation) { return; }
    m_animation = animation;
    m_speed = speed;
    m_playing = true;
}

void lcf::AnimationPlayer::play()
{
    if (not m_animation) { return; }
    m_playing = true;
}

void lcf::AnimationPlayer::stop()
{
    m_playing = false;
}

void lcf::AnimationPlayer::update(float delta_time)
{
    if (not m_playing) { return; }
    float delta_ticks = delta_time * m_animation->ticksPerSecond() * m_speed;
    m_current_time = fmod(m_current_time + delta_ticks, m_animation->duration());
    m_animation->update(m_current_time);
}

bool lcf::AnimationPlayer::isPlaying() const
{
    return m_playing;
}
