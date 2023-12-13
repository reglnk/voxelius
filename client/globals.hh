// SPDX-License-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
#ifndef CLIENT_GLOBALS_HH
#define CLIENT_GLOBALS_HH
#include <entt/entity/entity.hpp>
#include <shared/globals.hh>

namespace glxx
{
class Framebuffer;
class Texture2D;
class Renderbuffer;
} // namespace glxx

namespace canvas
{
class Font;    
} // namespace canvas

struct GLFWwindow;
class World;

namespace globals
{
extern GLFWwindow *window;

extern int width;
extern int height;
extern double aspect;

extern double frametime;
extern double frametime_avg;
extern uint64_t curtime;
extern uint64_t framecount;
extern size_t vertices_drawn;

extern entt::entity player;

extern glxx::Framebuffer world_fbo;
extern glxx::Texture2D world_fbo_color;
extern glxx::Renderbuffer world_fbo_depth;

extern canvas::Font font_8px;
extern canvas::Font font_16px;

extern unsigned int ui_scale;
extern unsigned int ui_screen;
} // namespace globals

#endif /* CLIENT_GLOBALS_HH */
