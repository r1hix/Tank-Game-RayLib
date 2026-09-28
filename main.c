#include <math.h>
#include <raylib.h>
#include <raymath.h>

int main(void) {
  // --- Window Dimensions ---
  const int windowWidth = 800;
  const int windowHeight = 800;

  // --- Tank Dimensions ---
  const float tankWidth = 100.0f;
  const float tankHeight = 70.0f;

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

  // --- Game Loop ---
  while (!WindowShouldClose()) {
    // --- Input Handling ---

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

    // --- Drawing ---
    BeginDrawing();
    ClearBackground(WHITE);

    // Drawing Tanks
    DrawRectanglePro(tank1, tank1Origin, tank1Rotation, BLACK);
    DrawRectanglePro(tank2, tank2Origin, tank2Rotation, RED);

    EndDrawing();
  }
  // --- Cleanup ---
  CloseWindow();
  return 0;
}