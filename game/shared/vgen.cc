// SPDX-License-Identifier: Zlib
// Copyright (C) 2024, Voxelius Contributors
#include <entt/entity/registry.hpp>
#include <FastNoiseLite.h>
#include <game/shared/game_voxels.hh>
#include <game/shared/globals.hh>
#include <game/shared/vgen.hh>
#include <game/shared/world.hh>
#include <random>

constexpr static std::int64_t FLOATLANDS_VAR = INT64_C(64);
constexpr static std::int64_t FLOATLANDS_MIDDLE = INT64_C(640);
constexpr static std::int32_t FLOATLANDS_START = INT32_C(16);

constexpr static std::int64_t OVERWORLD_VAR = INT64_C(64);
constexpr static std::int64_t OVERWORLD_SURFACE = INT64_C(0);
constexpr static std::int32_t OVERWORLD_START = INT32_C(-16);

constexpr static std::int64_t DEPTHS_VAR1 = INT64_C(64);
constexpr static std::int64_t DEPTHS_VAR2 = INT64_C(16);
constexpr static std::int64_t DEPTHS_MIDDLE = INT64_C(-640);
constexpr static std::int64_t DEPTHS_START = INT32_C(-24);

static fnl_state ow_terrain = {};
static fnl_state ow_caves_a = {};
static fnl_state ow_caves_b = {};

static std::mt19937_64 twister = {};

static void generate_overworld(ChunkCoord::value_type cx, ChunkCoord::value_type cz)
{
    constexpr static std::int32_t overworld_height = FLOATLANDS_START - OVERWORLD_START;
    constexpr static std::int64_t overworld_height_v = CHUNK_SIZE * overworld_height;

    // As it turns out LocalCoord::to_index and LocalCoord::from_index
    // both work perfectly fine with values way out of range on the Y axis
    std::array<Voxel, CHUNK_AREA * overworld_height_v> voxels = {};
    std::array<std::int64_t, CHUNK_AREA> heightmap = {};
    
    voxels.fill(NULL_VOXEL);
    heightmap.fill(INT64_MIN);

    // FIXME: influence this by a 2D heightmap
    // FIXME: influence surface level by a 2D heightmap as well
    const std::int64_t variation = 64;

    // Generate base noise terrain
    for(std::size_t idx = 0; idx < voxels.size(); idx += 1) {
        const LocalCoord lpos = LocalCoord::from_index(idx);
        const VoxelCoord vpos = ChunkCoord::to_voxel(ChunkCoord(cx, OVERWORLD_START, cz), lpos);
        const std::int64_t surface = vpos[1] + OVERWORLD_SURFACE;
        
        if(cxpr::abs(surface) > variation) {
            if(surface < INT64_C(0))
                voxels[idx] = game_voxels::stone;
            continue;
        }

        const auto heightmap_at = lpos[0] + lpos[2] * CHUNK_SIZE;
        const float density = OVERWORLD_VAR * fnlGetNoise3D(&ow_terrain, vpos[0], vpos[1], vpos[2]) - surface;

        if(density > 0.0f) {
            if(vpos[1] > heightmap[heightmap_at])
                heightmap[heightmap_at] = vpos[1];
            voxels[idx] = game_voxels::stone;
            continue;
        }
    }

    // Sprinkle surface voxels
    for(std::int16_t lx = 0; lx < CHUNK_SIZE; lx += 1)
    for(std::int16_t lz = 0; lz < CHUNK_SIZE; lz += 1)
    for(std::int16_t ly = 4; ly < overworld_height_v; ly += 1) {
        const auto vpos = ChunkCoord::to_voxel(ChunkCoord(cx, OVERWORLD_START, cz), LocalCoord(lx, ly, lz));
        const auto pdx = LocalCoord::to_index(LocalCoord(lx, ly - 1, lz));
        const auto cdx = LocalCoord::to_index(LocalCoord(lx, ly, lz));
        
        if((cxpr::abs(vpos[1]) <= variation) && (voxels[cdx] == NULL_VOXEL) && (voxels[pdx] == game_voxels::stone)) {
            for(std::int16_t bias = 0; bias < 4; bias += 1) {
                const auto bdx = LocalCoord::to_index(LocalCoord(lx, ly - bias - 1, lz));
                if(voxels[bdx] == game_voxels::stone) {
                    if(bias == 0)
                        voxels[bdx] = game_voxels::grass;
                    else voxels[bdx] = game_voxels::dirt;
                }
            }
        }
    }

    // Carve noise caves out
    for(std::size_t idx = 0; idx < voxels.size(); idx += 1) {
        const auto lpos = LocalCoord::from_index(idx);
        const auto vpos = ChunkCoord::to_voxel(ChunkCoord(cx, OVERWORLD_START, cz), lpos);

        if(vpos[1] <= variation) {
            const float na = fnlGetNoise3D(&ow_caves_a, vpos[0], 1.5f * vpos[1], vpos[2]);
            const float nb = fnlGetNoise3D(&ow_caves_b, vpos[0], 1.5f * vpos[1], vpos[2]);

            if((na * na + nb * nb) <= (1.0f / 1024.0f)) {
                voxels[idx] = NULL_VOXEL;
                continue;
            }
        }
    }

    // Convert anything below 64 voxels in depth into slate
    for(std::size_t idx = 0; idx < voxels.size(); idx += 1) {
        const auto lpos = LocalCoord::from_index(idx);
        const auto vpos = ChunkCoord::to_voxel(ChunkCoord(cx, OVERWORLD_START, cz), lpos);
        const auto thres = OVERWORLD_SURFACE - (64 + (static_cast<std::int64_t>(twister()) % INT64_C(4)));
        
        if((voxels[idx] == game_voxels::stone) && (vpos[1] <= thres)) {
            voxels[idx] = game_voxels::slate;
            continue;
        }
    }

    // UNDONE: actually store this in a separate hashmap
    for(std::size_t idx = 0; idx < voxels.size(); idx += 1) {
        const auto lpos = LocalCoord::from_index(idx);
        const auto vpos = ChunkCoord::to_voxel(ChunkCoord(cx, OVERWORLD_START, cz), lpos);
        
        if(vpos[1] == heightmap.at(lpos[0] + lpos[2] * CHUNK_SIZE)) {
            voxels[idx] = game_voxels::vtest;
            continue;
        }
    }

    // Copy voxel data into cubic chunks
    for(std::int32_t cy = 0; cy < overworld_height; ++cy) {
        const auto cpos = ChunkCoord(cx, cy + OVERWORLD_START, cz);
        const auto offset = CHUNK_VOLUME * cy;

        if(Chunk *chunk = world::assign(cpos, globals::registry.create())) {
            for(std::size_t i = 0; i < CHUNK_VOLUME; ++i) {
                chunk->voxels[i] = voxels[offset + i];
            }
        }
    }
}

void vgen::init(std::uint64_t seed)
{
    twister.seed(seed);

    ow_terrain = fnlCreateState();
    ow_terrain.seed = static_cast<int>(twister());
    ow_terrain.noise_type = FNL_NOISE_OPENSIMPLEX2S;
    ow_terrain.fractal_type = FNL_FRACTAL_FBM;
    ow_terrain.frequency = 0.005f;
    ow_terrain.octaves = 4;

    ow_caves_a = fnlCreateState();
    ow_caves_a.seed = static_cast<int>(twister());
    ow_caves_a.noise_type = FNL_NOISE_PERLIN;
    ow_caves_a.frequency = 0.0075f;

    ow_caves_b = fnlCreateState();
    ow_caves_b.seed = static_cast<int>(twister());
    ow_caves_b.noise_type = FNL_NOISE_PERLIN;
    ow_caves_b.frequency = 0.0075f;
}

void vgen::init_late(void)
{

}

void vgen::deinit(void)
{
    
}

void vgen::generate(ChunkCoord::value_type cx, ChunkCoord::value_type cz)
{
    generate_overworld(cx, cz);
}
