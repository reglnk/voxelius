// SPDX-License-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
#ifndef AF5EC318_9471_46CE_B408_CBD968EC6F7C
#define AF5EC318_9471_46CE_B408_CBD968EC6F7C
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace view
{
void update();
const glm::vec3 &get_angles();
const glm::vec3 &get_position();
const glm::mat4x4 &get_matrix();
} // namespace view

#endif/* AF5EC318_9471_46CE_B408_CBD968EC6F7C */
