#pragma once

#include <vector>
#include <string_view>
#include <algorithm>
#include "my_math.h"
#include "Object3D.h"
#include "KeyFrame.h"

namespace lcf {
    struct EvaluateResult
    {
        using Position = Vector3D;
        using Rotation = Quaternion;
        using Scale = Vector3D;
        EvaluateResult() = default;
        EvaluateResult(Position &&p, Rotation &&r, Scale &&s);
        Position position;
        Rotation rotation;
        Scale scale;
    };

    class KeyFrameTrack
    {
    public:
        template <typename T>
        using KeyFrames = std::vector<KeyFrame<T>>;
        using Position = KeyFrame<Vector3D>;
        using Rotation = KeyFrame<Quaternion>;
        using Scale = KeyFrame<Vector3D>;
        using Positions = std::vector<Position>;
        using Rotations = std::vector<Rotation>;
        using Scales = std::vector<Scale>;
        void setPositions(Positions &&positions);
        void setRotations(Rotations &&rotations);
        void setScales(Scales &&scales);
        KeyFrameTrack(std::string_view bone_name, Positions &&positions, Rotations &&rotations, Scales &&scales);
        KeyFrameTrack(const KeyFrameTrack &) = default;
        KeyFrameTrack(KeyFrameTrack && other);
        EvaluateResult evaluate(float time) const;
        const std::string &boneName() const;
    private:
        template <typename T>
        int indexOfKeyFrame(float time, const KeyFrames<T> &key_frames) const;
        template <typename T>
        T evaluate(float time, const KeyFrames<T> &key_frames) const;
    private:
        std::string m_bone_name;
        Positions m_positions;
        Rotations m_rotations;
        Scales m_scales;
    };
    template <typename T>
    inline int KeyFrameTrack::indexOfKeyFrame(float time, const KeyFrames<T> &key_frames) const
    {
        auto iter = std::lower_bound(key_frames.begin(), key_frames.end(), time);
        return std::distance(key_frames.begin(), iter) - 1;
    }
    template <typename T>
    inline T KeyFrameTrack::evaluate(float time, const KeyFrames<T> &key_frames) const
    {
        if (key_frames.empty()) { return T{}; }
        if (key_frames.size() == 1) { return key_frames.front().value; }
        int index = indexOfKeyFrame(time, key_frames);
        if (index < 0) { return key_frames.front().value; }
        if (index >= key_frames.size() - 1) { return key_frames.back().value; }
        const auto &[prev_time, prev_value] = key_frames[index];
        const auto &[next_time, next_value] = key_frames[index + 1];
        float factor = linear_interpolation_factor(time, prev_time, next_time);
        if (std::isnan(factor)) { return prev_value; }
        return lerp<T>(prev_value, next_value, factor);
    }
}