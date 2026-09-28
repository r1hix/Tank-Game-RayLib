#include <raylib.h>

int main(void) {
  // Window Dimensions
  const int windowWidth = 800;
  const int windowHeight = 800;

  // Tank Dimensions
  const int tankWidth = 70;
  const int tankHeight = 100;

  // Tank Positions
  int tank1XPos = 50;
  int tank1YPos = (windowHeight - tankHeight) / 2;

  int tank2XPos = windowWidth - tank1XPos - tankWidth;
  int tank2YPos = (windowHeight - tankHeight) / 2;

  // Initialize Window
  InitWindow(windowWidth, windowHeight, "Tank Game");
  SetTargetFPS(60);

  // Game Loop
  while (!WindowShouldClose()) {
    // Input Handling

    // Tank 1 Movement
    if (IsKeyDown(KEY_W)) {
      tank1YPos -= 5;
    }

    if (IsKeyDown(KEY_S)) {
      tank1YPos += 5;
    }

    if (IsKeyDown(KEY_A)) {
      tank1XPos -= 5;
    }

    if (IsKeyDown(KEY_D)) {
      tank1XPos += 5;
    }

    // Tank 2 Movement
    if (IsKeyDown(KEY_UP)) {
      tank2YPos -= 5;
    }

    if (IsKeyDown(KEY_DOWN)) {
      tank2YPos += 5;
    }

    if (IsKeyDown(KEY_LEFT)) {
      tank2XPos -= 5;
    }

    if (IsKeyDown(KEY_RIGHT)) {
      tank2XPos += 5;
    }

    // Drawing
    BeginDrawing();
    ClearBackground(WHITE);

    // Drawing Tanks
    DrawRectangle(tank1XPos, tank1YPos, tankWidth, tankHeight, BLACK);
    DrawRectangle(tank2XPos, tank2YPos, tankWidth, tankHeight, RED);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}