// SPDX-License-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
#ifndef BFEEBAB4_33CD_4BFB_B2C6_DA641B4BEDC5
#define BFEEBAB4_33CD_4BFB_B2C6_DA641B4BEDC5
#include <entt/fwd.hpp>
#include <shared/globals.hh>

struct GLFWwindow;
class World;

namespace globals
{
extern GLFWwindow *window;

extern float frametime;
extern float frametime_avg;
extern uint64_t curtime;
extern uint64_t framecount;

extern World world;
extern entt::entity player;



} // namespace globals

#endif/* BFEEBAB4_33CD_4BFB_B2C6_DA641B4BEDC5 */
