// SPDX-License-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
#ifndef B34BC61B_B2B6_4909_98AF_337754E06F22
#define B34BC61B_B2B6_4909_98AF_337754E06F22
#include <core/util.hh>

constexpr static const size_t CHUNK_SIZE = 16;
constexpr static const size_t CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE;
constexpr static const size_t CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
constexpr static const size_t CHUNK_SIZE_LOG2 = util::log2(CHUNK_SIZE);

#endif/* B34BC61B_B2B6_4909_98AF_337754E06F22 */
