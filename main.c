#include <math.h>
#include <raylib.h>
#include <raymath.h>

// === Constants & Definitions ===

#define maxLives 3
#define maxBullets 2
#define bulletSpeed 5
#define bulletSize 10

typedef enum Scene { IntroScene, GameScene, EndScene } Scene;

// === Structs ===

typedef struct Bullet {
  bool active;
  Vector2 position;
  Vector2 velocity;
  float size;
  Color color;
  float lifetime;
} Bullet;

typedef struct TankControls {
  int key_Up, key_Down, key_Left, key_Right, key_Fire;
} TankControls;

typedef struct Tank {
  Vector2 position, origin;
  float width, height, halfW, halfH, radius, rotation, speed, flashTimer;
  int lives, playerID;
  Color currentColor, baseColor;
  Bullet bullets[maxBullets];
  TankControls controls;
  Rectangle body;
} Tank;

typedef struct Sounds {
  Sound shootSFX, tankExplosionSFX, bulletExplosionSFX, bulletBounceSFX,
      gameOverSFX, gameStartSFX;
} Sounds;

// === Configuration ===

const int windowWidth = 800;
const int windowHeight = 800;

const float tankWidth = 100.0f;
const float tankHeight = 70.0f;
const float tankCollisionRad = 44.0f;
const float tankFlashTime = 0.15f;
const float tankSpeed = 3.0f;
const float tankRotationSpeed = 2.0f;
Vector2 tankOrigin = {tankWidth / 2, tankHeight / 2};

const Color initialTank1Color = BLUE;
const Color initialTank2Color = RED;
const Color tankFlashColor = PURPLE;

const int initialTank1XPos = 50;
const int initialTank1YPos = (windowHeight - tankHeight) / 2;
const float initialTank1Rotation = 0.0f;

const int initialTank2XPos = windowWidth - initialTank1XPos - tankWidth;
const int initialTank2YPos = initialTank1YPos;
const float initialTank2Rotation = 180.0f;

short unsigned int winner = 0;
Scene currentScene;
Sounds sounds;

// === Render / UI Functions ===

void DrawIntroUI(void) {
  DrawText("Tank Game", windowWidth / 2 - MeasureText("Tank Game", 40) / 2,
           windowHeight / 2 - 40, 40, BLACK);

  DrawText("Press space to start",
           windowWidth / 2 - MeasureText("Press space to start", 20) / 2,
           windowHeight / 2 + 40, 20, BLACK);
}

void DrawHUD(const Tank *tank1, const Tank *tank2) {
  DrawText(TextFormat("P1 Lives: %i", tank1->lives), 10, 10, 20, BLACK);
  DrawText(TextFormat("P2 Lives: %i", tank2->lives),
           windowWidth - MeasureText("P2 Lives: 3", 20) - 10, 10, 20, BLACK);
}

void DrawEndUI(void) {
  if (winner == 1) {
    DrawText("Player 1 Wins!",
             windowWidth / 2 - MeasureText("Player 2 Wins!", 40) / 2,
             windowHeight / 2 - 40, 40, BLACK);
  } else {
    DrawText("Player 2 Wins!",
             windowWidth / 2 - MeasureText("Player 2 Wins!", 40) / 2,
             windowHeight / 2 - 40, 40, BLACK);
  }
  DrawText("Press space to restart",
           windowWidth / 2 - MeasureText("Press space to restart", 20) / 2,
           windowHeight / 2 + 40, 20, BLACK);
}

void DrawTank(const Tank *tank) {
  DrawRectanglePro(tank->body, tank->origin, tank->rotation,
                   tank->currentColor);

  for (int i = 0; i < maxBullets; i++) {
    if (tank->bullets[i].active) {
      DrawCircleV(tank->bullets[i].position, tank->bullets[i].size,
                  tank->bullets[i].color);
    }
  }
}

// === Game Logic & Updates ===

void ResetGameState(Tank *tank1, Tank *tank2) {
  winner = 0;

  for (int i = 0; i < maxBullets; i++) {
    tank1->bullets[i].active = false;
    tank1->bullets[i].lifetime = 0.0f;
    tank2->bullets[i].active = false;
    tank2->bullets[i].lifetime = 0.0f;
  }

  tank1->body.x = initialTank1XPos;
  tank1->body.y = initialTank1YPos;
  tank1->rotation = initialTank1Rotation;
  tank1->lives = maxLives;
  tank1->flashTimer = 0.0f;
  tank1->currentColor = tank1->baseColor;

  tank2->body.x = initialTank2XPos;
  tank2->body.y = initialTank2YPos;
  tank2->rotation = initialTank2Rotation;
  tank2->lives = maxLives;
  tank2->flashTimer = 0.0f;
  tank2->currentColor = tank2->baseColor;
}

void UpdateTank(Tank *tank) {
  float rad = DEG2RAD * tank->rotation;
  float dx = cosf(rad);
  float dy = sinf(rad);

  // Tank Movement
  if (IsKeyDown(tank->controls.key_Up)) {
    tank->body.x += tankSpeed * dx;
    tank->body.y += tankSpeed * dy;
  }

  if (IsKeyDown(tank->controls.key_Down)) {
    tank->body.x -= tankSpeed * dx;
    tank->body.y -= tankSpeed * dy;
  }

  if (IsKeyDown(tank->controls.key_Left))
    tank->rotation -= tankRotationSpeed;

  if (IsKeyDown(tank->controls.key_Right))
    tank->rotation += tankRotationSpeed;

  // Bullet Firing
  if (IsKeyPressed(tank->controls.key_Fire)) {
    for (int i = 0; i < maxBullets; i++) {
      if (!tank->bullets[i].active) {
        float offset = (tank->width / 2) + tank->bullets->size;
        Vector2 spawnLocation = {tank->body.x + (dx * offset),
                                 tank->body.y + (dy * offset)};
        Vector2 velocity = {dx * bulletSpeed, dy * bulletSpeed};

        tank->bullets[i].size = bulletSize;
        tank->bullets[i].position = spawnLocation;
        tank->bullets[i].velocity = velocity;
        tank->bullets[i].color = BLACK;
        tank->bullets[i].active = true;
        tank->bullets[i].lifetime = 0.0f;

        PlaySound(sounds.shootSFX);
        break;
      }
    }
  }

  // Bullet Movement & Lifetime Management
  for (int i = 0; i < maxBullets; i++) {
    if (tank->bullets[i].active) {
      tank->bullets[i].position.x += tank->bullets[i].velocity.x;
      tank->bullets[i].position.y += tank->bullets[i].velocity.y;

      tank->bullets[i].lifetime += GetFrameTime();
      if (tank->bullets[i].lifetime > 2) {
        tank->bullets[i].active = false;
        tank->bullets[i].lifetime = 0.0f;
      }
    }
  }

  // AABB Calculation
  tank->halfW =
      (tankWidth / 2) * fabsf(cosf(rad)) + (tankHeight / 2) * fabsf(sinf(rad));
  tank->halfH =
      (tankWidth / 2) * fabsf(sinf(rad)) + (tankHeight / 2) * fabsf(cosf(rad));

  // Screen Boundary Collision
  if (tank->body.x - tank->halfW < 0)
    tank->body.x = tank->halfW;
  if (tank->body.x + tank->halfW > windowWidth)
    tank->body.x = windowWidth - tank->halfW;
  if (tank->body.y - tank->halfH < 0)
    tank->body.y = tank->halfH;
  if (tank->body.y + tank->halfH > windowHeight)
    tank->body.y = windowHeight - tank->halfH;

  // Screen Edge Collision
  for (int i = 0; i < maxBullets; i++) {
    if (tank->bullets[i].position.x < 0) {
      tank->bullets[i].position.x = 0;
      tank->bullets[i].velocity.x = -tank->bullets[i].velocity.x;
      PlaySound(sounds.bulletBounceSFX);
    }
    if (tank->bullets[i].position.x > windowWidth) {
      tank->bullets[i].position.x = windowWidth;
      tank->bullets[i].velocity.x = -tank->bullets[i].velocity.x;
      PlaySound(sounds.bulletBounceSFX);
    }
    if (tank->bullets[i].position.y < 0) {
      tank->bullets[i].position.y = 0;
      tank->bullets[i].velocity.y = -tank->bullets[i].velocity.y;
      PlaySound(sounds.bulletBounceSFX);
    }
    if (tank->bullets[i].position.y > windowHeight) {
      tank->bullets[i].position.y = windowHeight;
      tank->bullets[i].velocity.y = -tank->bullets[i].velocity.y;
      PlaySound(sounds.bulletBounceSFX);
    }
  }
}

// === Collision Handling ===

void CheckTankTankCollision(Tank *tank1, Tank *tank2) {
  if (CheckCollisionRecs(tank1->body, tank2->body)) {
    float overlapX =
        (tank1->halfW + tank2->halfW) - fabsf(tank1->body.x - tank2->body.x);
    float overlapY =
        (tank1->halfH + tank2->halfH) - fabsf(tank1->body.y - tank2->body.y);

    if (overlapX < overlapY) {
      if (tank1->body.x < tank2->body.x) {
        tank1->body.x -= overlapX / 2;
        tank2->body.x += overlapX / 2;
      } else {
        tank1->body.x += overlapX / 2;
        tank2->body.x -= overlapX / 2;
      }
    } else {
      if (tank1->body.y < tank2->body.y) {
        tank1->body.y -= overlapY / 2;
        tank2->body.y += overlapY / 2;
      } else {
        tank1->body.y += overlapY / 2;
        tank2->body.y -= overlapY / 2;
      }
    }
  }
}

void CheckBulletBulletCollision(Tank *tank1, Tank *tank2) {
  for (int i = 0; i < maxBullets; i++) {
    for (int j = 0; j < maxBullets; j++) {
      if (tank1->bullets[i].active && tank2->bullets[j].active &&
          CheckCollisionCircles(
              tank1->bullets[i].position, tank1->bullets[i].size,
              tank2->bullets[j].position, tank2->bullets[j].size)) {
        tank1->bullets[i].active = false;
        tank1->bullets[i].lifetime = 0.0f;

        tank2->bullets[j].active = false;
        tank2->bullets[j].lifetime = 0.0f;

        PlaySound(sounds.bulletExplosionSFX);
      }
    }
  }
}

void CheckBulletTankCollision(Tank *tank1, Tank *tank2) {
  for (int i = 0; i < maxBullets; i++) {
    if (tank1->bullets[i].active &&
        CheckCollisionCircles(
            tank1->bullets[i].position, tank1->bullets[i].size,
            (Vector2){tank2->body.x, tank2->body.y}, tankCollisionRad)) {
      tank2->lives -= 1;
      tank2->flashTimer = tankFlashTime;
      tank1->bullets[i].active = false;
      tank1->bullets[i].lifetime = 0.0f;
      PlaySound(sounds.tankExplosionSFX);

      if (tank2->lives <= 0) {
        winner = tank1->playerID;
        PlaySound(sounds.gameOverSFX);
        currentScene = EndScene;
      }
    }
  }
}

// === Main Program ===

int main(void) {
  // Tank 1 Initialization
  Tank tank1 = {.position = {initialTank1XPos, initialTank1YPos},
                .width = tankWidth,
                .height = tankHeight,
                .radius = tankCollisionRad,
                .origin = tankOrigin,
                .rotation = initialTank1Rotation,
                .speed = tankSpeed,
                .flashTimer = tankFlashTime,
                .lives = maxLives,
                .playerID = 1,
                .currentColor = initialTank1Color,
                .baseColor = initialTank1Color,
                .controls = {KEY_W, KEY_S, KEY_A, KEY_D, KEY_LEFT_SHIFT},
                .body = {.x = initialTank1XPos,
                         .y = initialTank1YPos,
                         .width = tankWidth,
                         .height = tankHeight},
                .halfW = (tankWidth / 2) * fabsf(cosf(initialTank1Rotation)) +
                         (tankHeight / 2) * fabsf(sinf(initialTank1Rotation)),
                .halfH = (tankWidth / 2) * fabsf(sinf(initialTank1Rotation)) +
                         (tankHeight / 2) * fabsf(cosf(initialTank1Rotation))};

  // Tank 2 Initialization
  Tank tank2 = {
      .position = {initialTank2XPos, initialTank2YPos},
      .width = tankWidth,
      .height = tankHeight,
      .radius = tankCollisionRad,
      .origin = tankOrigin,
      .rotation = initialTank2Rotation,
      .speed = tankSpeed,
      .flashTimer = tankFlashTime,
      .lives = maxLives,
      .playerID = 2,
      .currentColor = initialTank2Color,
      .baseColor = initialTank2Color,
      .controls = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_RIGHT_SHIFT},
      .body = {.x = initialTank2XPos,
               .y = initialTank2YPos,
               .width = tankWidth,
               .height = tankHeight},
      .halfW = (tankWidth / 2) * fabsf(cosf(initialTank2Rotation)) +
               (tankHeight / 2) * fabsf(sinf(initialTank2Rotation)),
      .halfH = (tankWidth / 2) * fabsf(sinf(initialTank2Rotation)) +
               (tankHeight / 2) * fabsf(cosf(initialTank2Rotation))};

  InitWindow(windowWidth, windowHeight, "Tank Game");
  SetTargetFPS(60);

  InitAudioDevice();

  sounds.shootSFX = LoadSound("resources/shoot.wav");
  sounds.tankExplosionSFX = LoadSound("resources/tankExplosion.wav");
  sounds.bulletExplosionSFX = LoadSound("resources/bulletExplosion.wav");
  sounds.bulletBounceSFX = LoadSound("resources/bulletBounce.wav");
  sounds.gameOverSFX = LoadSound("resources/gameOver.wav");
  sounds.gameStartSFX = LoadSound("resources/gameStart.wav");

  currentScene = IntroScene;

  // -- Game Loop --

  while (!WindowShouldClose()) {
    switch (currentScene) {
    case IntroScene: {
      if (IsKeyPressed(KEY_SPACE)) {
        PlaySound(sounds.gameStartSFX);
        currentScene = GameScene;
      }
      break;
    }

    case GameScene: {
      UpdateTank(&tank1);
      UpdateTank(&tank2);

      // Collision Detection
      CheckTankTankCollision(&tank1, &tank2);
      CheckBulletTankCollision(&tank1, &tank2);
      CheckBulletTankCollision(&tank2, &tank1);
      CheckBulletBulletCollision(&tank1, &tank2);

      // Flash Timers
      if (tank1.flashTimer > 0.0f) {
        tank1.flashTimer -= GetFrameTime();
        tank1.currentColor = tankFlashColor;
      } else {
        tank1.currentColor = initialTank1Color;
      }

      if (tank2.flashTimer > 0.0f) {
        tank2.flashTimer -= GetFrameTime();
        tank2.currentColor = tankFlashColor;
      } else {
        tank2.currentColor = initialTank2Color;
      }

      break;
    }

    case EndScene: {
      if (IsKeyPressed(KEY_SPACE)) {
        ResetGameState(&tank1, &tank2);
        PlaySound(sounds.gameStartSFX);
        currentScene = GameScene;
      }
      break;
    }
    }

    // -- Draw Phase --

    BeginDrawing();
    ClearBackground(WHITE);

    switch (currentScene) {
    case IntroScene: {
      DrawIntroUI();
      break;
    }

    case GameScene: {
      DrawTank(&tank1);
      DrawTank(&tank2);
      DrawHUD(&tank1, &tank2);
      break;
    }

    case EndScene: {
      DrawEndUI();
      break;
    }
    }

    EndDrawing();
  }

  // -- Cleanup --

  UnloadSound(sounds.shootSFX);
  UnloadSound(sounds.tankExplosionSFX);
  UnloadSound(sounds.bulletExplosionSFX);
  UnloadSound(sounds.bulletBounceSFX);
  UnloadSound(sounds.gameOverSFX);
  UnloadSound(sounds.gameStartSFX);

  CloseAudioDevice();
  CloseWindow();

  return 0;
}