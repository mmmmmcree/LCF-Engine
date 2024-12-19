#pragma once

#include "KeyFrameTrack.h"
#include "Matrix.h"
#include "Object3D.h"

namespace lcf {
    class Animation
    {
    public:
        using BoneMap = std::unordered_map<std::string, Bone *>;
        using KeyFrameTracks = std::vector<KeyFrameTrack>;
        using KeyFrameTracksPtr = std::shared_ptr<KeyFrameTracks>;
        using ControlledBones = std::vector<Bone *>;
        Animation() = default;
        Animation(const Animation &other) = default; //! 复制完还需要对控制的骨骼做一次更新
        Animation(std::string_view name, float duration, float ticks_ps, const KeyFrameTracksPtr &tracks);
        void updateControlledBones(const BoneMap &bone_map);
        float duration() const;
        float ticksPerSecond() const;
        const std::string &name() const;
        void update(float time) const;
    private:
        std::string m_name;
        float m_duration;
        float m_ticks_ps;
        ControlledBones m_controlled_bones;
        KeyFrameTracksPtr m_tracks;
    };
}