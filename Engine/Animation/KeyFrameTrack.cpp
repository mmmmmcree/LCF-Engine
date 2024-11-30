#include "KeyFrameTrack.h"

lcf::EvaluateResult::EvaluateResult(Position &&p, Rotation &&r, Scale &&s) :
    position(std::move(p)), rotation(std::move(r)), scale(std::move(s))
{
}

void lcf::KeyFrameTrack::setPositions(Positions &&positions)
{
    m_positions = std::move(positions);
}

void lcf::KeyFrameTrack::setRotations(Rotations &&rotations)
{
    m_rotations = std::move(rotations);
}

void lcf::KeyFrameTrack::setScales(Scales &&scales)
{
    m_scales = std::move(scales);
}

lcf::KeyFrameTrack::KeyFrameTrack(std::string_view bone_name, Positions &&positions, Rotations &&rotations, Scales &&scales) :
    m_bone_name(bone_name),
    m_positions(std::move(positions)),
    m_rotations(std::move(rotations)),
    m_scales(std::move(scales))
{
}

lcf::KeyFrameTrack::KeyFrameTrack(KeyFrameTrack &&other) :
    m_bone_name(std::move(other.m_bone_name)),
    m_positions(std::move(other.m_positions)),
    m_rotations(std::move(other.m_rotations)),
    m_scales(std::move(other.m_scales))
{
}

lcf::EvaluateResult lcf::KeyFrameTrack::evaluate(float time) const
{
    return EvaluateResult(evaluate(time, m_positions), evaluate(time, m_rotations), evaluate(time, m_scales));
}

const std::string &lcf::KeyFrameTrack::boneName() const
{
    return m_bone_name;
}
