> I wanted to learn how to make a game from scratch using C and raylib.
> And this documents the entire process of making it from scratch.

# Tank Game

A 2D local two-player tank game built from scratch in C using [raylib](https://www.raylib.com/).

## Requirements

- C compiler (`clang` or `gcc`)
- `raylib` (v5.0+)
- `make` (or `mingw32-make` on Windows)
- `pkg-config` (macOS / Linux)

## Build & Run

```bash
# Build and run
make run

# Build only
make

# Clean build artifacts
make clean
```

## Controls

| Tank | Move | Rotate | Shoot |
|---|---|---|---|
| **Tank 1 (Blue)** | `W` / `S` | `A` / `D` | `Left Shift` |
| **Tank 2 (Red)** | `Up` / `Down` | `Left` / `Right` | `Right Shift` |

## Features

- **Rotational Movement**: Direction-based forward/backward translation based on tank heading angle.
- **Dynamic Bounding Constraints**: Screen boundary collisions calculated from the tank's rotated bounding box (AABB).
- **Shooting & Ballistics**: Up to 2 active bullets per tank with automatic lifetime despawn after 2 seconds.
- **Collision Detection & Feedback**:
  - **Bullet vs. Bullet**: Direct projectile collisions neutralize both shots with an explosion effect.
  - **Bullet vs. Tank**: Direct hits register on the opposing tank with damage flash feedback and life counters (3 lives per tank).
  - **Bullet vs. Wall**: Screen edges ricochet active bullets with bounce audio.
- **Sound Effects (SFX)**: Full audio feedback for tank shots, ricochets, collisions, damage, game start, and game over.
- **Scene Management**: Intro start screen, active match scene, and game-over winner announcement with instant restart (`SPACE`).


