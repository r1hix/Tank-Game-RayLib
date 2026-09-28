#include <raylib.h>

int main(void) {

  const int windowWidth = 800;
  const int windowHeight = 800;

  const int tankWidth = 70;
  const int tankHeight = 100;

  int tank1XPos = 50;
  int tank1YPos = windowHeight / 2;

  int tank2XPos = windowWidth - 50 - tankWidth;
  int tank2YPos = windowHeight / 2;

  InitWindow(windowWidth, windowHeight, "Tank Game");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(WHITE);
    DrawRectangle(tank1XPos, tank1YPos, tankWidth, tankHeight, BLACK);
    DrawRectangle(tank2XPos, tank2YPos, tankWidth, tankHeight, RED);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}