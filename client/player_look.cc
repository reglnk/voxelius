// SPDX-License-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
#include <client/globals.hh>
#include <client/input.hh>
#include <client/player_look.hh>
#include <client/screen.hh>
#include <shared/angle.hh>
#include <shared/head.hh>
#include <shared/world.hh>

constexpr static const float PITCH_MIN = -1.0f * cxmath::radians(89.9f);
constexpr static const float PITCH_MAX = +1.0f * cxmath::radians(89.9f);

static float previous_cx = 0.0f;
static float previous_cy = 0.0f;

static void on_cursor_pos(const CursorPosEvent &event)
{
    if(globals::world.registry.valid(globals::player)) {
        const float dx = event.xpos - previous_cx;
        const float dy = event.ypos - previous_cy;

        auto &head = globals::world.registry.get<HeadComponent>(globals::player);
        head.angles.x -= cxmath::radians(dy) * 0.25f; // UNDONE
        head.angles.y -= cxmath::radians(dx) * 0.25f;
        head.angles.x = cxmath::clamp(head.angles.x, PITCH_MIN, PITCH_MAX);
        head.angles = angle::wrap_180_n(head.angles);
    }

    previous_cx = event.xpos;
    previous_cy = event.ypos;
}

void player_look::init()
{
    float width, height;
    screen::get_size(width, height);

    previous_cx = width / 2.0f;
    previous_cy = height / 2.0f;

    globals::dispatcher.sink<CursorPosEvent>().connect<&on_cursor_pos>();
}
