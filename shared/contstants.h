#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SERVER_PORT 5000
#define MAX_CLIENTS 4

#define TICK_RATE 60
#define TICK_DELAY_MS (1000 / TICK_RATE)

#define PHASE_1_DURATION_SEC 30.0f
#define PHASE_2_DURATION_SEC 180.0f
#define GHOST_TIMEOUT_SEC 15.0f

#define MAP_WIDTH 30
#define MAP_HEIGHT 30
#define TILE_SIZE 36

#define PLAYER_WIDTH 20
#define PLAYER_LENGTH 30
#define DRAIN_RATE 50.0f
#define REGEN_RATE 20.0f
#define MAX_STAMINA 100.0f
#define PLAYER_SPEED 144.0f
#define SEEKER_SPEED 108.0f
#define ABILITY_COOLDOWN 10.0f

#define LOBBY_TIME 30.0f
#define COUNTDOWN_TIME 15.0f
#define GAME_TIME 180.0f
#define GAME_OVER_TIME 15.0f

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define VIRTUAL_WIDTH 1280
#define VIRTUAL_HEIGHT 720

#define START_SCREEN_WIDTH 1280 
#define START_SCREEN_HEIGHT 720

#define SERVER_IP "127.0.0.1"

#define VISION_RADIUS 300.0f
#define VISION_SOFTNESS 150.0f

#define MAP_CSV_PATH "assets/Map.csv"
#define TILESET_PATH "assets/tilemap.png"

#define PLAYER_IDLE_PATH "assets/player/blue/idle.png"
#define PLAYER_WALK_PATH "assets/player/blue/walk.png"
#define PLAYER_DEATH_PATH "assets/player/blue/death.png"

#define SEEKER_IDLE_PATH "assets/player/red/idle.png"
#define SEEKER_WALK_PATH "assets/player/red/walk.png"
#define SEEKER_DEATH_PATH "assets/player/red/death.png"

#define VISION_SHADER_PATH "assets/shaders/vision.fs"

#define MENU_CSV_PATH "assets/Menu.csv"

#define CAMERA_ZOOM 1.5f

#define WALK_FRAMES 4
#define DEATH_FRAMES 3
#define IDLE_FRAMES 2

#define ANIMATION_SPEED 0.2f

#define PLAYER_FRAME_SIZE 32.0f

#endif
