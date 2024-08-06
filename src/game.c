#include "game.h"


// IMPORTANT VARIABLES
long long global_time = 0;
bool is_gameover = false;

// COLORS TO MATCH THE GAME THEME
Color Dark_Red = {11, 4, 13, 50};
Color Grass_Green = {56, 55, 35, 255};
Color Dirt_Brown = {20, 15, 16, 255};



//  LOAD THE GAME AND INITIALIZE IMPORTANT THINGS
void initialize_game(void)
{
                InitWindow(WINDOW_X, WINDOW_Y, "game");
                InitAudioDevice();

                SetTargetFPS(60);
}



// UNLOAD THE GAME AND UNINITIALIZE THE INITIALIZED GAME
void end_game(Player *players, Enemy *enemies, Bullet *bullets[], Background *background, int size, Texture *main_menu, Button *b1, Button *b2, Button *b3, Button *b4, Button *b5, Button *b6, Sfx *sound)
{
                for (int i = 0; i < MAX_PLAYERS; i++)
                {
                    // UNLOAD PLAYER TEXTURES
                    UnloadTexture(players[i].texture);
                    UnloadTexture(players[i].texture_standing_right);
                    UnloadTexture(players[i].texture_standing_left);
                    UnloadTexture(players[i].texture_walking_right);
                    UnloadTexture(players[i].texture_walking_left);
                    UnloadTexture(players[i].texture_sprinting_right);
                    UnloadTexture(players[i].texture_sprinting_left);
                    UnloadTexture(players[i].texture_jumping_right);
                    UnloadTexture(players[i].texture_jumping_left);
                    UnloadTexture(players[i].texture_attack_right);
                    UnloadTexture(players[i].texture_attack_left);
                    UnloadTexture(players[i].texture_attack_alt_right);
                    UnloadTexture(players[i].texture_attack_alt_left);
                    UnloadTexture(players[i].texture_special_right);
                    UnloadTexture(players[i].texture_special_left);
                    UnloadTexture(players[i].texture_unspecial_right);
                    UnloadTexture(players[i].texture_unspecial_left);
                    UnloadTexture(players[i].texture_death_right);
                    UnloadTexture(players[i].texture_death_left);
                }

                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    // UNLOAD ENEMY TEXTURES
                    UnloadTexture(enemies[i].texture);
                    UnloadTexture(enemies[i].texture_activating_right);
                    UnloadTexture(enemies[i].texture_activating_left);
                    UnloadTexture(enemies[i].texture_deactivated_right);
                    UnloadTexture(enemies[i].texture_deactivated_left);
                    UnloadTexture(enemies[i].texture_idle_right);
                    UnloadTexture(enemies[i].texture_idle_left);
                    UnloadTexture(enemies[i].texture_attack_right);
                    UnloadTexture(enemies[i].texture_attack_left);
                    UnloadTexture(enemies[i].texture_walking_right);
                    UnloadTexture(enemies[i].texture_walking_left);
                    UnloadTexture(enemies[i].texture_death_right);
                    UnloadTexture(enemies[i].texture_death_left);                    
                }

                for (int i = 0; i < MAX_BULLETS; i++)
                {
                    bullet_free(bullets, i);
                    if (bullets[i])
                    {
                        UnloadTexture(bullets[i]->texture);
                    }
                }

                // UNLOAD GAMEPLAY BACKROUND TEXTURE
                for (int i = 0; i < size; i++)
                {
                    UnloadTexture(background[i].texture);
                }

                // UNLOAD BUTTONS AND MISC TEXTURES
                UnloadTexture(b1->texture);
                UnloadTexture(b2->texture);
                UnloadTexture(b3->texture);
                UnloadTexture(b4->texture);
                UnloadTexture(b5->texture);
                UnloadTexture(b6->texture);
                UnloadTexture(*main_menu);

                // UNLOAD SOUNDS
                UnloadSound(sound->button_click);
                UnloadSound(sound->button_hover);
                UnloadSound(sound->dead);
                UnloadSound(sound->jump);
                UnloadSound(sound->hiding);
                UnloadSound(sound->attack);
                UnloadSound(sound->attack_miss);
                UnloadSound(sound->activated);
                UnloadSound(sound->explosion);
                UnloadSound(sound->shot);
                UnloadSound(sound->hit);
                UnloadSound(sound->step);
                UnloadMusicStream(sound->main_menu);
                UnloadMusicStream(sound->gameplay);
                UnloadMusicStream(sound->gameover);

                CloseAudioDevice();
                CloseWindow();
}



// PLATFORMS INITIALIZATION
void platform_init(Platform *platform, Rectangle rect, Color color)
{
                platform->rect = rect;
                platform->color = color;
}



// BULLETS INITIALIZATION
void bullet_init(Bullet *bullets[], Vector2 position, double speed)
{
                // FIND A FREE AND AN AVAILABLE BULLET
                int found = -1;
                for (int i = 0; i < MAX_BULLETS; i++)
                {
                    if (bullets[i] == NULL)
                    {
                        found = i;
                        break;  
                    }
                }

                // INITIALIZE THE FOUND BULLET
                if (found >= 0)
                {
                    bullets[found] = malloc(sizeof(Bullet));
                    bullets[found]->position = position;
                    bullets[found]->speed = speed;
                    bullets[found]->texture = LoadTexture("assets/enemy/bullet.png");
                }
}



bool bullet_ready(Bullet *bullets[])
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i] != NULL)
        {
            return true;
        }
    }
    return false;
}



void bullet_free(Bullet *bullets[], int index)
{
                UnloadTexture(bullets[index]->texture);
                free(bullets[index]);
                bullets[index] = NULL;
}



void bullet_update(Bullet *bullets[], Enemy *enemy)
{
                for (int i = 0; i < MAX_BULLETS; i++) if (bullets[i])
                {
                    bullets[i]->position.x += bullets[i]->speed;
                    if (bullets[i]->position.x < -300 * enemy->rect.x || bullets[i]->position.x > 300 * enemy->rect.x)
                    {
                        bullet_free(bullets, i);
                    }
                }
}



// CAMERA
void camera_update(Camera2D *camera, const Player *player, Platform *ground, int world_width)
{
                camera->zoom += (float)GetMouseWheelMove() * 0.025;
                if (camera->zoom > 3) 
                {
                    camera->zoom = 3;
                }
                else if (camera->zoom < 0.15)
                {
                    camera->zoom = 0.15;
                }
                if (player->alive && (player->rect.y + player->rect.height < ground->rect.y + 0.125 * ground->rect.height))
                {
                    camera->target = (Vector2) { player->rect.x, player->rect.y};
                }
                if (camera->target.x <= 305)
                {
                    camera->target.x = 305;
                }
                if (camera->target.x >= world_width - 664)
                {
                    camera->target.x = world_width - 664;
                }
}



// MAIN MENU BUTTONS
void button_init(Button * button, Vector2 position, const char* text)
{
                button->srect.x = 0;
                button->srect.y = 0;
                button->srect.width = 16;
                button->srect.height = 16;

                button->drect.x = position.x;
                button->drect.y = position.y;
                button->drect.width = 4 * button->srect.width;
                button->drect.height = 4 * button->srect.height;

                button->text = text;
                button->hovered = false;
                button->clicked = false;
                button->click_count = 0;

                if (!strcmp(text, "PLAY"))
                {
                    button->texture = LoadTexture("assets/ui/play.png");
                }                
                if (!strcmp(text, "QUIT"))
                {
                    button->texture = LoadTexture("assets/ui/quit.png");
                }                
                if (!strcmp(text, "SETTINGS"))
                {
                    button->texture = LoadTexture("assets/ui/settings.png");
                }                
                if (!strcmp(text, "HOME"))
                {
                    button->texture = LoadTexture("assets/ui/home.png");
                }                
                if (!strcmp(text, "RESTART"))
                {
                    button->texture = LoadTexture("assets/ui/restart.png");
                }
                if (!strcmp(text, "PAUSE"))
                {
                    button->texture = LoadTexture("assets/ui/pause.png");
                }                
}



void button_update(Button *button, const Sfx *sound)
{
                button->clicked = false;

                Vector2 current_mouse_pos = GetMousePosition();

                bool is_hovered = CheckCollisionPointRec(current_mouse_pos, button->drect);

                if (is_hovered && !button->hovered)
                {
                    PlaySound(sound->button_hover);                    
                }

                button->hovered = is_hovered;

                if (is_hovered)
                {
                    button->srect.x = 16;
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        PlaySound(sound->button_click);
                        button->clicked = true;
                    }
                }
                else
                {
                    button->srect.x = 0;
                }
}  



void sound_init(Sfx *sound)
{
    sound->button_hover = LoadSound("assets/sfx/button_hover.wav");
    sound->button_click = LoadSound("assets/sfx/button_click.wav");

    sound->dead = LoadSound("assets/sfx/dead.wav");
    sound->jump = LoadSound("assets/sfx/jump.wav");
    sound->hiding = LoadSound("assets/sfx/hiding.wav");
    sound->attack = LoadSound("assets/sfx/attack.wav");
    sound->attack_miss = LoadSound("assets/sfx/attack_miss.wav");

    sound->activated = LoadSound("assets/sfx/activated.mp3");
    sound->explosion = LoadSound("assets/sfx/explosion.wav");
    sound->shot = LoadSound("assets/sfx/shot.wav");
    sound->hit = LoadSound("assets/sfx/hit.wav");
    sound->step = LoadSound("assets/sfx/step.wav");

    sound->main_menu = LoadMusicStream("assets/sfx/main_menu.mp3");
    sound->gameplay = LoadMusicStream("assets/sfx/gameplay.mp3");
    sound->gameover = LoadMusicStream("assets/sfx/gameover.mp3");
    SetMusicVolume(sound->main_menu, 0.4);
    SetMusicVolume(sound->gameplay, 0.4);
    SetMusicVolume(sound->gameover, 0.4);

}