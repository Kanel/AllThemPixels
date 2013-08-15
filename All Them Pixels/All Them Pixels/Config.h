// UI
#define UI_FONT_COLOR Color(0, 0, 0)
#define UI_FONT_PATH "C:/Windows/Fonts/segoeui.ttf"
#define UI_FONT_SIZE 24
#define UI_FONT_SMALL_SIZE 16
#define UI_BACKGROUND_COLOR Color(255, 255, 255)
#define UI_TEXT_SPACING 10
#define UI_PLAYER_CUSTOMIZATION_WIDTH 400
#define UI_PLAYER_CUSTOMIZATION_HEIGHT 200
#define UI_PLAYER_CUSTOMIZATION_MODIFY_VALUE 4

// Pause State
#define PAUSE_BACKGROUND Color(0, 0, 0, 200)
#define PAUSE_TEXT "Paused"
#define PAUSE_FONT_PATH UI_FONT_PATH
#define PAUSE_FONT_SIZE 36
#define PAUSE_FONT_COLOR Color(255, 255, 255, 200)
#define PAUSE_HEXAGON_RADIUS_RATIO 0.0889f
#define PAUSE_HEXAGON_PADDING 2.5f
#define PAUSE_HEXAGON_COLOR Color(0, 0, 0, 150)

// Sounds
#define SOUNDS_BACKGROUND_MUSIC "battle_prelude.ogg"
#define SOUNDS_ENEMY_DEATH "coin1.wav"
#define SOUNDS_PLAYER_HIT "hurt1.wav"
#define SOUNDS_SELECTED "select1.wav"
#define SOUNDS_FIREING "shoot1.wav"
#define SOUNDS_VOLUME_MUSIC 1
#define SOUNDS_VOLUME_EFFECTS 2
#define SOUNDS_VOLUME_UI 2

// Game
#define GAME_UPDATE_INTERVAL 10
#define GAME_TITLE "All them Pixels!"
#define GAME_FADE_TIME 2000 // Milliseconds

// Player
#define PLAYER_SIZE 30
#define PLAYER_AIM_BOX_SIZE 4
#define PLAYER_COLOR Color(0, 0, 0)
#define PLAYER_DEFAULT_HP 1000000
#define PLAYER_DEFAULT_SPEED 1000.0f
#define PLAYER_DEFAULT_WEAPON_COOLDOWN 100
#define PLAYER_DEFAULT_WEAPON_DAMAGE 100
#define PLAYER_DEFAULT_WEAPON_PIERCING 1
#define PLAYER_DEFAULT_WEAPON_SPEED 1300
#define PLAYER_DEFAULT_WEAPON_SPREAD 5
#define PLAYER_DEFAULT_WEAPON_TTL 55

// Joytstick
#define GAMEPAD_A 0
#define GAMEPAD_B 1
#define GAMEPAD_X 2
#define GAMEPAD_y 3
#define GAMEPAD_LB 4
#define GAMEPAD_RB 5
#define GAMEPAD_BACK 6
#define GAMEPAD_START 7
#define GAMEPAD_JOYSTICK_LEFT 8
#define GAMEPAD_JOYSTICK_RIGHT 9
#define GAMEPAD_JOYSTICK_THRESHOLD 30.0f

// World
#define WORLD_LAYERS 10
#define WORLD_VIEW_WIDTH 1440
#define WORLD_VIEW_HEIGHT 900
#define WORLD_TERRITORY_RADIUS 2048
#define WORLD_TERRITORY_SPACING 0
#define WORLD_TERRITORY_SPAWN_POINTS 100000
#define WORLD_TERRITORY_SPAWN_RATE 500

// Map
#define MAP_HEXAGON_RADIUS 20.0f
#define MAP_HEXAGON_SPACING 2.0f
#define MAP_OPACITY 190
#define MAP_DEFAULT_COLOR Color(180, 180, 180, MAP_OPACITY)
#define MAP_VISITED_COLOR Color(20, 20, 200, MAP_OPACITY)
#define MAP_CLEARED_COLOR Color(20, 200, 20, MAP_OPACITY)
#define MAP_PLAYER_COLOR Color(20, 20, 20, MAP_OPACITY)


// Territory
#define TERRITORY_CELL_SIZE 50.0f
#define TERRITORY_TILE_SIZE 10
#define TERRITORY_ENEMY_SPAWN_RING_SIZE 50
#define TERRITORY_ENEMY_AURA_SIZE 10 // Layers.
#define TERRITORY_ENEMY_AURA_INTENSITY 2
#define TERRITORY_ENEMY_AURA_COLOR Color(0, 0, 0)
#define TERRITORY_PROBE_COUNT 24
#define TERRITORY_PROBE_SIZE 10
#define TERRITORY_PROBE_INTENSITY 4
#define TERRITORY_FADE_TILES_BASE_COLOR Color(255, 255, 255, 0)
#define TERRITORY_FLOOR_TILES_BASE_COLOR Color(255, 255, 255, 255)
#define TERRITORY_FLOOR_TILES_GRAYNESS 30
#define TERRITORY_BORDER_TILES_REDNESS 40
#define TERRITORY_SAFE_TILES_GREENESS 40
#define TERRITORY_SAFE_SIZE 10

// Skill related constants.
#define SKILL_CHANGE_COOLDOWN 200
#define SKILL_MODIFY_COOLDOWN 20