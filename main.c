#include <raylib.h>
#include <stdio.h>

int main(void) {
  // Window Dimensions
  const int windowWidth = 800;
  const int windowHeight = 800;

  // Tank Dimensions
  const int tankWidth = 70;
  const int tankHeight = 100;

  // Initial Tank Positions
  int tank1XPos = 50;
  int tank1YPos = (windowHeight - tankHeight) / 2;
  Vector2 tank1Origin = {(float)tankWidth / 2, (float)tankHeight / 2};

  int tank2XPos = windowWidth - tank1XPos - tankWidth;
  int tank2YPos = tank1YPos;
  Vector2 tank2Origin = tank1Origin;

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
    if (IsKeyDown(KEY_W))
      tank1.y -= 5;

    if (IsKeyDown(KEY_S))
      tank1.y += 5;

    if (IsKeyDown(KEY_A))
      tank1.x -= 5;

    if (IsKeyDown(KEY_D))
      tank1.x += 5;

    // Tank 2 Movement
    if (IsKeyDown(KEY_UP))
      tank2.y -= 5;

    if (IsKeyDown(KEY_DOWN))
      tank2.y += 5;

    if (IsKeyDown(KEY_LEFT))
      tank2.x -= 5;

    if (IsKeyDown(KEY_RIGHT))
      tank2.x += 5;

    // Boundary Constraints
    // Tank 1
    if (tank1.x < 0)
      tank1.x = 0;
    if (tank1.x > windowWidth - (float)tankWidth / 2)
      tank1.x = windowWidth - (float)tankWidth / 2;
    if (tank1.y < 0)
      tank1.y = 0;
    if (tank1.y > windowHeight - (float)tankHeight / 2)
      tank1.y = windowHeight - (float)tankHeight / 2;

    // Tank 2
    if (tank2.x < 0)
      tank2.x = 0;
    if (tank2.x > windowWidth - (float)tankWidth / 2)
      tank2.x = windowWidth - (float)tankWidth / 2;
    if (tank2.y < 0)
      tank2.y = 0;
    if (tank2.y > windowHeight - (float)tankHeight / 2)
      tank2.y = windowHeight - (float)tankHeight / 2;

    // Drawing
    BeginDrawing();
    ClearBackground(WHITE);

    // Drawing Tanks
    // DrawRectangle(tank1XPos, tank1YPos, tankWidth, tankHeight, BLACK);
    // DrawRectangle(tank2XPos, tank2YPos, tankWidth, tankHeight, RED);
    DrawRectanglePro(tank1, tank1Origin, 0, BLACK);
    DrawRectanglePro(tank2, tank2Origin, 0, RED);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}