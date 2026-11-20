#pragma once// CoastVista 3D - config.h
// Shared constants for the whole team. Scene layout numbers live HERE so that
// Ramisa's and Nafija's modules always agree on where things are.
// If you change a number, tell your teammate (it is a shared file).
#pragma once

// ---------------------------------------------------------------- window ---
#define WINDOW_TITLE "CoastVista 3D"
#define USE_MSAA 1      // 1 = smooth edges (anti-aliasing). Set to 0 if the window fails to open on an old GPU.

static const int   WINDOW_W = 1280;
static const int   WINDOW_H = 720;

// ------------------------------------------------------------ projection ---
static const float FOV_Y = 60.0f;      // vertical field of view (degrees)
static const float Z_NEAR = 0.5f;       // keep this >= 0.3, otherwise road markings flicker
static const float Z_FAR = 1500.0f;

// ------------------------------------------------------------ scene layout ---
// Units: 1 unit = 1 metre.
//   +X = along the road      +Y = up      +Z = towards the SEA (land is -Z)
// Looking along +X, the sea is on your RIGHT. See docs/SCENE_LAYOUT.md.
static const float ROAD_HALF_LENGTH = 1000.0f;                 // road runs from x=-1000 to +1000
static const float WORLD_HALF_LENGTH = ROAD_HALF_LENGTH + 100.0f;

static const float ROAD_HALF_WIDTH = 6.0f;                    // asphalt: z = -6 .. +6
static const float SIDEWALK_W = 3.0f;                    // footpath on both sides
static const float SIDEWALK_H = 0.15f;                   // kerb height

static const float SAND_Z0 = ROAD_HALF_WIDTH + SIDEWALK_W;      // beach starts at z = 9
static const float SAND_Z1 = 45.0f;                             // sand slopes down to here
static const float SAND_Y0 = -0.2f;                             // sand height at z = SAND_Z0
static const float SAND_Y1 = -1.2f;                             // sand height at z = SAND_Z1
static const float SEA_Y = -0.9f;                             // sea level -> shoreline at about z = 34
static const float SEA_Z0 = 30.0f;                             // (hidden under the sand until the shoreline)
static const float WORLD_Z_SEA = 1400.0f;                     // far edge of the sea
static const float WORLD_Z_LAND = -1200.0f;                     // far edge of the land

// Land side (negative Z), measured from the road centre:
static const float BUILDING_Z_NEAR = -14.0f;                    // hotels/resorts front line
static const float BUILDING_Z_FAR = -45.0f;                    // back of the building strip
static const float FOREST_Z_FAR = -110.0f;                   // trees fill -45 .. -110
static const float HILL_Z_NEAR = -180.0f;                   // hills start here ...
static const float HILL_Z_FAR = -300.0f;                   // ... and reach full height here

// --------------------------------------------------------- default camera ---
// Standing on the sea-side footpath, looking along the road, slightly towards the sea.
static const float CAM_START_X = -60.0f;
static const float CAM_START_Y = 1.7f;                      // eye height of a person
static const float CAM_START_Z = 7.5f;
static const float CAM_START_YAW = 4.0f;                      // degrees, 0 = looking along +X, 90 = towards the sea
static const float CAM_START_PITCH = 1.0f;                      // degrees, + = look up

// ---------------------------------------------------------- sun and sky ---
// Day mode values. Nafija's Day 3 work (sunset / night) replaces these at run time.
static const float SUN_DIR[3] = { 0.45f, 0.85f, 0.30f };   // direction TO the sun
static const float SKY_TOP[3] = { 0.18f, 0.40f, 0.78f };
static const float SKY_HORIZON[3] = { 0.74f, 0.86f, 0.95f };   // later: fog colour = this colour

