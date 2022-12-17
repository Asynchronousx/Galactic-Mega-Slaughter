#pragma once

// Header that contains the enum used in this project to avoid useless import
// in some other headers

// Skybox indices
#define SKY_FRONT 0
#define SKY_RIGHT 1
#define SKY_LEFT 2
#define SKY_BACK 3
#define SKY_UP 4
#define SKY_DOWN 5

// Camera class enums
enum class DIRECTION { FORWARD, BACKWARD, LEFT, RIGHT };
enum class AXIS { X, Y, Z };

// Drawer
enum class fogtype { LIGHT, STANDARD, DARK };

// Enemy
enum class GAMEMODE { EASY, NORMAL, HARD, MADNESS };
enum class IDENTIFIER { RED = 0, BLUE = 1, BRONZE = 2, BLOOD = 3, ORANGE = 4 };

// Game Handler
enum class STATE { TITLE, DIFFICULTY, LOADING, GAME, ABOUT, SCORES, GAMEOVER, EXIT };

// Texture Loader
enum textype { SKY, GROUND, STANDARD };

// Weapon
enum class TYPE { AUTOMATIC, SHOTGUN, PISTOL};