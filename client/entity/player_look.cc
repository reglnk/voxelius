// SPDX-License-Identifier: Zlib
// Copyright (C) 2024, Voxelius Contributors
#include <client/entity/player_look.hh>
#include <client/globals.hh>
#include <entt/entity/registry.hpp>
#include <shared/entity/head.hh>

constexpr static float PITCH_MIN = -1.0f * cxpr::radians(90.0f);
constexpr static float PITCH_MAX = +1.0f * cxpr::radians(90.0f);

void player_look::add_angles(const Vector2D &pitch_yaw)
{
    if(globals::registry.valid(globals::player)) {
        auto &head = globals::registry.get<HeadComponent>(globals::player);
        head.angles[0] += pitch_yaw[0];
        head.angles[1] += pitch_yaw[1];
        head.angles[0] = cxpr::clamp(head.angles[0], PITCH_MIN, PITCH_MAX);
        head.angles = EulerAngles::wrap_180(head.angles);
    }
}
