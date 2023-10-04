// SPDX-License-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
#version 450 core
#pragma import common

layout(location = 0) in vec2 texcoord;
layout(location = 1) in vec2 pixcoord;

layout(location = 0) out vec4 target;

layout(binding = 0) uniform sampler2D font;
layout(binding = 1) uniform usampler2D label;

layout(std140, binding = 0) uniform UiDraw_UBO {
    vec4 screen;
    vec4 glyph;
    vec4 color;
    vec4 rect;
};

void main(void)
{
    const uint unicode = texture(label, texcoord).x;
    const float cx = float(0U + unicode % uint(glyph.z)) / glyph.z;
    const float cy = float(1U + unicode / uint(glyph.z)) / glyph.w;
    const float gx = mod(pixcoord.x, glyph.x) / glyph.x / glyph.z;
    const float gy = mod(glyph.y - pixcoord.y, glyph.y) / glyph.y / glyph.w;
    target = color * texture(font, vec2(cx + gx, 1.0 - cy + gy)).x;
}

