#include "Animation.h"

lcf::Animation::Animation(std::string_view name, float duration, float ticks_ps, const KeyFrameTracksPtr &tracks) :
    m_name(name), m_duration(duration), m_ticks_ps(ticks_ps),
    m_tracks(tracks), m_controlled_bones(tracks->size())
{
}

void lcf::Animation::updateControlledBones(const BoneMap &bone_map)
{
    for (int i = 0; i < m_tracks->size(); ++i) {
        const auto &track = m_tracks->at(i);
        auto iter = bone_map.find(track.boneName());
        if (iter == bone_map.end()) { continue; }
        m_controlled_bones[i] = iter->second;
    }
}

float lcf::Animation::duration() const
{
    return m_duration;
}

float lcf::Animation::ticksPerSecond() const
{
    return m_ticks_ps;
}

const std::string &lcf::Animation::name() const
{
    return m_name;
}

void lcf::Animation::update(float time) const
{
    for (int i = 0; i < m_controlled_bones.size(); ++i) {
        auto bone = m_controlled_bones[i];
        const auto &track = m_tracks->at(i);
        auto result = track.evaluate(time);
        bone->setPosition(result.position);
        bone->setScale(result.scale);
        bone->setRotation(result.rotation);
    }
}