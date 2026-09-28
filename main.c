#include <math.h>
#include <raylib.h>
#include <raymath.h>

int main(void) {
  // Window Dimensions
  const int windowWidth = 800;
  const int windowHeight = 800;

  // Tank Dimensions
  const float tankWidth = 100.0f;
  const float tankHeight = 70.0f;

  // Initial Tank Positions
  int tank1XPos = 50;
  int tank1YPos = (windowHeight - tankHeight) / 2;
  float tank1Rotation = 0.0f;
  float speed = 3.0f;
  float tank1HalfW = tankWidth / 2;
  float tank1HalfH = tankHeight / 2;
  float tankHitboxRadius =
      sqrtf(powf(tank1HalfW, 2) + powf(tank1HalfH, 2));
  
  Vector2 tank1Origin = {tank1HalfH, tank1HalfH};

  int tank2XPos = windowWidth - tank1XPos - tankWidth;
  int tank2YPos = tank1YPos;
  Vector2 tank2Origin = tank1Origin;
  float tank2Rotation = 180.0f;

  // Tank Rectangles
  Rectangle tank1 = {tank1XPos, tank1YPos, tankWidth, tankHeight};
  Rectangle tank2 = {tank2XPos, tank2YPos, tankWidth, tankHeight};

  // Initialize Window
  InitWindow(windowWidth, windowHeight, "Tank Game");
  SetTargetFPS(60);

  // Game Loop
  while (!WindowShouldClose()) {
    // Input Handling

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

    // Boundary Constraints
    // Tank 1
    if (tank1.x - tankHitboxRadius < 0)
      tank1.x = tankHitboxRadius;
    if (tank1.x > windowWidth - tankHitboxRadius)
      tank1.x = windowWidth - tankHitboxRadius;
    if (tank1.y - tankHitboxRadius < 0)
      tank1.y = tankHitboxRadius;
    if (tank1.y > windowHeight - tankHitboxRadius)
      tank1.y = windowHeight - tankHitboxRadius;

    // Tank 2
    if (tank2.x < 0)
      tank2.x = 0;
    if (tank2.x > windowWidth - tankWidth / 2)
      tank2.x = windowWidth - tankWidth / 2;
    if (tank2.y < 0)
      tank2.y = 0;
    if (tank2.y > windowHeight - tankHeight / 2)
      tank2.y = windowHeight - tankHeight / 2;

    // Drawing
    BeginDrawing();
    ClearBackground(WHITE);

    // Drawing Tanks
    DrawRectanglePro(tank1, tank1Origin, tank1Rotation, BLACK);
    DrawRectanglePro(tank2, tank2Origin, tank2Rotation, RED);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}