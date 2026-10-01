#include <math.h>
#include <raylib.h>
#include <raymath.h>

// - Constants -

typedef enum Scene { IntroScene, GameScene, EndScene } Scene;

// --- Window Dimensions ---
const int windowWidth = 800;
const int windowHeight = 800;

// --- Tank Dimensions ---
const float tankWidth = 100.0f;
const float tankHeight = 70.0f;
const float tankCollisionRad = 44.0f;
Color tank1Color = BLUE;
Color tank2Color = RED;

// --- Bullets ---
#define maxBullets 2
#define bulletSpeed 5

typedef struct Bullet {
  bool active;
  Vector2 position;
  Vector2 velocity;
  float size;
  Color color;
  float lifetime;
} Bullet;

Bullet tank1Bullets[maxBullets];
Bullet tank2Bullets[maxBullets];

int main(void) {

  // --- Initial Tank Positions ---
  int tank1XPos = 50;
  int tank1YPos = (windowHeight - tankHeight) / 2;
  float tank1Rotation = 0.0f;
  float speed = 3.0f;
  float tank1HalfW = tankWidth / 2;
  float tank1HalfH = tankHeight / 2;
  Vector2 tank1Origin = {tank1HalfW, tank1HalfH};

  int tank2XPos = windowWidth - tank1XPos - tankWidth;
  int tank2YPos = tank1YPos;
  Vector2 tank2Origin = tank1Origin;
  float tank2Rotation = 180.0f;

  // --- Tank Rectangles ---
  Rectangle tank1 = {tank1XPos, tank1YPos, tankWidth, tankHeight};
  Rectangle tank2 = {tank2XPos, tank2YPos, tankWidth, tankHeight};

  // --- Initialize Window ---
  InitWindow(windowWidth, windowHeight, "Tank Game");
  SetTargetFPS(60);
  Scene currentScene = IntroScene;

  // --- Game Loop ---
  while (!WindowShouldClose()) {
    // --- Input Handling ---

    switch (currentScene) {

    // Scene: Intro
    case IntroScene: {
      if (IsKeyPressed(KEY_SPACE)) {
        currentScene = GameScene;
      }
      break;
    }

    // Scene: Game
    case GameScene: {

      // Tank 1 Movement
      if (IsKeyDown(KEY_W)) {
        float dx = cosf(DEG2RAD * tank1Rotation);
        float dy = sinf(DEG2RAD * tank1Rotation);

        tank1.x += speed * dx;
        tank1.y += speed * dy;
      }

      if (IsKeyDown(KEY_S)) {
        float dx = cosf(DEG2RAD * tank1Rotation);
        float dy = sinf(DEG2RAD * tank1Rotation);

        tank1.x -= speed * dx;
        tank1.y -= speed * dy;
      }

      if (IsKeyDown(KEY_A))
        tank1Rotation -= 2;

      if (IsKeyDown(KEY_D))
        tank1Rotation += 2;

      // Tank 2 Movement
      if (IsKeyDown(KEY_UP)) {
        float dx = cosf(DEG2RAD * tank2Rotation);
        float dy = sinf(DEG2RAD * tank2Rotation);

        tank2.x += speed * dx;
        tank2.y += speed * dy;
      }

      if (IsKeyDown(KEY_DOWN)) {
        float dx = cosf(DEG2RAD * tank2Rotation);
        float dy = sinf(DEG2RAD * tank2Rotation);

        tank2.x -= speed * dx;
        tank2.y -= speed * dy;
      }

      if (IsKeyDown(KEY_LEFT))
        tank2Rotation -= 2;

      if (IsKeyDown(KEY_RIGHT))
        tank2Rotation += 2;

      // Tank 1 Shooting
      if (IsKeyPressed(KEY_LEFT_SHIFT)) {
        for (int i = 0; i < maxBullets; i++) {
          if (!tank1Bullets[i].active) {
            float rad = DEG2RAD * tank1Rotation;
            float dx = cosf(rad);
            float dy = sinf(rad);
            float offset = (tankWidth / 2) + tank1Bullets->size;
            Vector2 spawnLocation = {tank1.x + (dx * offset),
                                     tank1.y + (dy * offset)};
            Vector2 velocity = {dx * bulletSpeed, dy * bulletSpeed};

            tank1Bullets[i].size = 10.0f;
            tank1Bullets[i].position = spawnLocation;
            tank1Bullets[i].velocity = velocity;
            tank1Bullets[i].color = BLACK;
            tank1Bullets[i].active = true;
            tank1Bullets[i].lifetime = 0.0f;
            break;
          }
        }
      }

      // Tank 2 Shooting
      if (IsKeyPressed(KEY_RIGHT_SHIFT)) {
        for (int i = 0; i < maxBullets; i++) {
          if (!tank2Bullets[i].active) {
            float rad = DEG2RAD * tank2Rotation;
            float dx = cosf(rad);
            float dy = sinf(rad);
            float offset = (tankWidth / 2) + tank2Bullets->size;
            Vector2 spawnLocation = {tank2.x + (dx * offset),
                                     tank2.y + (dy * offset)};
            Vector2 velocity = {dx * bulletSpeed, dy * bulletSpeed};

            tank2Bullets[i].size = 10.0f;
            tank2Bullets[i].position = spawnLocation;
            tank2Bullets[i].velocity = velocity;
            tank2Bullets[i].color = BLACK;
            tank2Bullets[i].active = true;
            tank2Bullets[i].lifetime = 0.0f;
            break;
          }
        }
      }

      // --- Updating Bullets ---

      // Tank1
      for (int i = 0; i < maxBullets; i++) {
        if (tank1Bullets[i].active) {
          tank1Bullets[i].position.x += tank1Bullets[i].velocity.x;
          tank1Bullets[i].position.y += tank1Bullets[i].velocity.y;

          tank1Bullets[i].lifetime += GetFrameTime();
          if (tank1Bullets[i].lifetime > 2) {
            tank1Bullets[i].active = false;
            tank1Bullets[i].lifetime = 0.0f;
          }
        }
      }

      // Tank 2
      for (int i = 0; i < maxBullets; i++) {
        if (tank2Bullets[i].active) {
          tank2Bullets[i].position.x += tank2Bullets[i].velocity.x;
          tank2Bullets[i].position.y += tank2Bullets[i].velocity.y;

          tank2Bullets[i].lifetime += GetFrameTime();
          if (tank2Bullets[i].lifetime > 2) {
            tank2Bullets[i].active = false;
            tank2Bullets[i].lifetime = 0.0f;
          }
        }
      }

      // --- Boundary Constraints ---

      // Tank 1
      float tank1Rad = DEG2RAD * tank1Rotation;
      tank1HalfW = (tankWidth / 2) * fabsf(cosf(tank1Rad)) +
                   (tankHeight / 2) * fabsf(sinf(tank1Rad));
      tank1HalfH = (tankWidth / 2) * fabsf(sinf(tank1Rad)) +
                   (tankHeight / 2) * fabsf(cosf(tank1Rad));

      if (tank1.x - tank1HalfW < 0)
        tank1.x = tank1HalfW;
      if (tank1.x + tank1HalfW > windowWidth)
        tank1.x = windowWidth - tank1HalfW;
      if (tank1.y - tank1HalfH < 0)
        tank1.y = tank1HalfH;
      if (tank1.y + tank1HalfH > windowHeight)
        tank1.y = windowHeight - tank1HalfH;

      // Tank 2
      float tank2Rad = DEG2RAD * tank2Rotation;
      float tank2HalfW = (tankWidth / 2) * fabsf(cosf(tank2Rad)) +
                         (tankHeight / 2) * fabsf(sinf(tank2Rad));
      float tank2HalfH = (tankWidth / 2) * fabsf(sinf(tank2Rad)) +
                         (tankHeight / 2) * fabsf(cosf(tank2Rad));

      if (tank2.x - tank2HalfW < 0)
        tank2.x = tank2HalfW;
      if (tank2.x + tank2HalfW > windowWidth)
        tank2.x = windowWidth - tank2HalfW;
      if (tank2.y - tank2HalfH < 0)
        tank2.y = tank2HalfH;
      if (tank2.y + tank2HalfH > windowHeight)
        tank2.y = windowHeight - tank2HalfH;

      // --- Tank Collisions ---

      if (CheckCollisionRecs(tank1, tank2)) {
        float overlapX = (tank1HalfW + tank2HalfW) - fabsf(tank1.x - tank2.x);
        float overlapY = (tank1HalfH + tank2HalfH) - fabsf(tank1.y - tank2.y);

        if (overlapX < overlapY) {
          if (tank1.x < tank2.x) {
            tank1.x -= overlapX / 2;
            tank2.x += overlapX / 2;
          } else {
            tank1.x += overlapX / 2;
            tank2.x -= overlapX / 2;
          }
        } else {
          if (tank1.y < tank2.y) {
            tank1.y -= overlapY / 2;
            tank2.y += overlapY / 2;
          } else {
            tank1.y += overlapY / 2;
            tank2.y -= overlapY / 2;
          }
        }
      }

      // --- Bullet Collisions ---

      // Bullet on Bullet Collision
      for (int i = 0; i < maxBullets; i++) {
        if (CheckCollisionCircles(
                tank1Bullets[i].position, tank1Bullets[i].size,
                tank2Bullets[i].position, tank2Bullets[i].size)) {
          tank1Bullets[i].active = false;
          tank2Bullets[i].active = false;

          tank1Bullets[i].lifetime = 0.0f;
          tank2Bullets[i].lifetime = 0.0f;
        }
      }

      // Bullet on Tank Collision
      for (int i = 0; i < maxBullets; i++) {
        // Tank1Bullets -> Tank2
        if (CheckCollisionCircles(
                tank1Bullets[i].position, tank1Bullets[i].size,
                (Vector2){tank2.x, tank2.y}, tankCollisionRad)) {
          tank2Color = PURPLE;
        }

        // Tank2Bullets -> Tank1
        if (CheckCollisionCircles(
                tank2Bullets[i].position, tank2Bullets[i].size,
                (Vector2){tank1.x, tank1.y}, tankCollisionRad)) {
          tank1Color = PURPLE;
        }
      }
      break;
    }

    // Scene: End
    case EndScene: {
      break;
    }
    }

    // --- Drawing ---
    BeginDrawing();
    ClearBackground(WHITE);

    switch (currentScene) {

    // Scene: Intro
    case IntroScene: {
      DrawText("Tank Game", windowWidth / 2 - MeasureText("Tank Game", 40) / 2,
               windowHeight / 2 - 40, 40, BLACK);

      DrawText("Press space to start",
               windowWidth / 2 - MeasureText("Press space to start", 20) / 2,
               windowHeight / 2 + 40, 20, BLACK);
      break;
    }

    // Scene: Game
    case GameScene: {
      // Drawing Tanks
      DrawRectanglePro(tank1, tank1Origin, tank1Rotation, tank1Color);
      DrawRectanglePro(tank2, tank2Origin, tank2Rotation, tank2Color);

      // Drawing Bullets
      for (int i = 0; i < maxBullets; i++) {
        if (tank1Bullets[i].active) {
          DrawCircleV(tank1Bullets[i].position, tank1Bullets[i].size,
                      tank1Bullets[i].color);
        }
      }

      for (int i = 0; i < maxBullets; i++) {
        if (tank2Bullets[i].active) {
          DrawCircleV(tank2Bullets[i].position, tank2Bullets[i].size,
                      tank2Bullets[i].color);
        }
      }
      break;
    }

    // Scene: End
    case EndScene: {
      break;
    }
    }

    EndDrawing();
  }
  // --- Cleanup ---
  CloseWindow();
  return 0;
}