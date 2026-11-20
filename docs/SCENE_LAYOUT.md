# Scene layout (Nafija's Day 1 plan, matches `src/config.h`)

Units: **1 unit = 1 metre**.

```
+X  along the road      +Y  up      +Z  towards the SEA  (land is -Z)
```

Looking along +X, the sea is on your **right**.

## Zones, from the sea to the hills

| Zone | Z range (m) | Notes |
|------|-------------|-------|
| Sea | +34 ... +1400 | Sea level y = -0.9. Shoreline at about z = 34 |
| Beach (sand) | +9 ... +45 | Slopes from y = -0.2 down to y = -1.2 |
| Footpath (sea side) | +6 ... +9 | Kerb height 0.15 m. Lamp posts at z = +7.5 |
| **Road** | **-6 ... +6** | 12 m wide, centre line at z = 0, runs x = -1000 ... +1000 |
| Footpath (land side) | -9 ... -6 | Kerb height 0.15 m |
| Building strip | -14 ... -45 | Hotels, resorts, restaurants face the road. Front line at z = -14 |
| Forest | -45 ... -110 | Trees of different height and shape |
| Hills | -180 ... -300 | Rise to 40-100 m, haze colour at the top |

## Default camera

Position (-60, 1.7, 7.5): standing on the sea-side footpath, eye height 1.7 m,
yaw 4 degrees (0 = along the road, 90 = towards the sea), pitch 1 degree.

## Day colour palette

| Object | Hex |
|--------|-----|
| Sky top | `#2E66C7` |
| Sky at horizon (also fog colour later) | `#BDDBF2` |
| Asphalt | `#26262A` |
| Lane paint | `#F2F2EE` |
| Footpath | `#9E9C94` |
| Sand | `#D8C39A` |
| Sea, shallow | `#1F8FA0` |
| Sea, deep | `#0B3A6B` |
| Grass | `#3A6B2A` |
| Hills, foot / top | `#3F6B3A` / `#6E8C86` |

## Rules

1. Layout numbers live only in `src/config.h`. If you change one, tell your teammate.
2. Ramisa edits `road.cpp`, Nafija edits `sea.cpp`. New modules get their own `.cpp` file.
   Two people editing one file is the main cause of merge conflicts.
3. Reference photos go in `docs/references/`, with a one-line note on where each came from.
