// MAKE SURE THAT THIS LIBRARY IS INCLUDED ONLY ONCE
#pragma once

// INCLUDE REQUIRED LIBRARIES
#include <raylib.h>

#include <math.h>  
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>


// WINDOW RESOLUTION
#define WINDOW_X 1100
#define WINDOW_Y 800


// GAME OBJECT COUNTS
#define MAX_PLAYERS 1
#define MAX_ENEMIES 7
#define MAX_BULLETS 50
#define PLATFORM_COUNT 10


// IMPORTANT VARIABLES
extern long long global_time;
extern bool is_gameover;

// COLORS TO MATCH THE GAME-THEME
extern Color Dark_Red;
extern Color Grass_Green;
extern Color Dirt_Brown;



// ENUM FOR GAMESTATE
typedef enum { MAIN_MENU, INFO, GAMEPLAY, GAMEOVER} Game_State;



// STRUCT FOR PLAYER CONTROLS
typedef struct
{
                short move_left;
                short move_right;
                short sprint;
                short jump;
                short special;
                short attack;
                short grab;
}
Key_Binds;



// PLAYER STRUCT
typedef struct 
{
                Rectangle rect;
                Vector2 velocity;
                Key_Binds controls;

                double scale_size;
                double speed;
                short health;
                double sprint;
                int jumpforce;
                double gravity;
                int strength;
                int attack_count;

                int special_frame;
                int unspecial_frame;
                int attack_frame;
                int attack_alt_frame;
                int death_frame;
                double frame_time;

                bool is_walking;
                bool is_sprinting;
                bool is_jumping;
                bool is_attacking;
                bool attack_toggle;
                bool is_specialing;
                bool is_unspecialing;
                bool is_doublejumping;
                bool on_ground;
                bool hiding;
                bool can_doublejump;
                bool alive;
                int direction;

                Texture texture;
                Texture texture_standing_right;
                Texture texture_standing_left;
                Texture texture_walking_right;
                Texture texture_walking_left;
                Texture texture_sprinting_right;
                Texture texture_sprinting_left;
                Texture texture_jumping_right;
                Texture texture_jumping_left;
                Texture texture_attack_right;
                Texture texture_attack_left;
                Texture texture_attack_alt_right;
                Texture texture_attack_alt_left;
                Texture texture_special_right;
                Texture texture_special_left;
                Texture texture_unspecial_right;
                Texture texture_unspecial_left;
                Texture texture_death_right;
                Texture texture_death_left;
}
Player;



// ENEMY STRUCT
typedef struct
{
                Rectangle rect;
                Rectangle vision_rect;
                Rectangle shooting_range_rect;
                Rectangle walking_rect;
                Vector2 velocity;

                double scale_size;
                double speed;
                short health;
                double sprint;
                int jumpforce;
                double gravity;
                int attack_cooldown;
                int attack_delay;

                int activating_frame;
                int attack_frame;
                int death_frame;
                double frame_time;

                // int type;
                bool alive; 
                bool activated;
                bool is_deactivated;
                bool is_activating;
                bool is_idle;
                bool is_walking;
                bool is_attacking;
                bool is_bullet_initiated;
                int direction;

                Texture texture;
                Texture texture_deactivated_right;
                Texture texture_deactivated_left;
                Texture texture_activating_right;
                Texture texture_activating_left;
                Texture texture_idle_right;
                Texture texture_idle_left;
                Texture texture_walking_right;
                Texture texture_walking_left;
                Texture texture_attack_right;
                Texture texture_attack_left;
                Texture texture_death_right;
                Texture texture_death_left;                
}
Enemy;



// STRUCT FOR BULLET
typedef struct
{
                Vector2 position;
                double speed;
                Texture texture;
}
Bullet;



// STRUCT FOR PLATFORM(S)
typedef struct
{
                Rectangle rect;
                Color color;
}
Platform;



// STRUCT FOR BACKGROUND IN THE GAMEPLAY
typedef struct
{
                Rectangle srec;
                Rectangle drec;
                Texture texture;
                Texture texture2;
}
Background;



// STRUCT FOR GUI BUTTONS
typedef struct
{
                Rectangle srect;
                Rectangle drect;

                const char* text;
                bool hovered;
                bool clicked;  
                int click_count;

                Texture texture;
}
Button;



// STRUCT FOR ALL GAME SOUNDS INCLUDING MUSIC
typedef struct
{
                Sound button_hover;
                Sound button_click;

                Sound dead;
                Sound jump;
                Sound hiding;
                Sound attack;
                Sound attack_miss;

                Sound activated;
                Sound explosion;
                Sound shot;
                Sound hit;
                Sound step;

                Music main_menu;
                Music gameplay;
                Music gameover;
}
Sfx;


// ......................................... FUNCTION PROTOTYPES .........................................
// GAME
void initialize_game(void);
void end_game(Player *players, Enemy *enemies, Bullet *bullets[], Background *background, int size, Texture *main_menu, Button *b1, Button *b2, Button *b3, Button *b4, Button *b5, Button *b6, Sfx *sound);

// PLAYER
void player_init(Player *player, Vector2 position, double speed, Key_Binds controls);
int  player_on_platform(const Player *player, const Platform *platforms, int count);
void player_update(Player *player, Platform *platform, Enemy *enemy, Bullet *bullets[], int world_width, const Sfx *sound);
void player_draw(Player *player, int *frame, double *interval, int *frame_count, const Sfx *sound);
void player_dead(Player *player, Platform *ground, const Sfx *sound);

// CAMERA
void camera_update(Camera2D *camera, const Player *player, Platform *ground, int world_width);

// ENEMIES
void enemy_init(Enemy *enemy, Vector2 position, double speed, int health, double scale, double size, int buff, int delay);
void enemy_update(Enemy *enemy, Player *player, Platform *platform, Bullet *bullets[], const Sfx *sound, int index);
void enemy_dead(Enemy *enemy, Player *player, const Sfx *sound, Platform *ground);
void enemy_draw(Enemy *enemy, int *frame, double *interval, int *frame_count, const Sfx *sound);

// BULLETS
void bullet_init(Bullet *bullets[], Vector2 position, double speed);
bool bullet_ready(Bullet *bullets[]);
void bullet_free(Bullet *bullets[], int index);
void bullet_update(Bullet *bullets[], Enemy *enemy);

// MAP OBJECTS
void platform_init(Platform *platform, Rectangle rect, Color color);
void background_init(Background *background, int size);
void background_update(Background *background, int size);
void background_draw(Background *background, int size);

// MISC / GUI BUTTONS
void button_init(Button *button, Vector2 position, const char* text);
void button_update(Button *button, const Sfx *sound);
void sound_init(Sfx *sound);