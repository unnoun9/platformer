#include "game.h"



// ......................................... SETS UP A PLAYER .........................................
void player_init(Player * player, Vector2 position, double speed, Key_Binds controls)
{
                player->scale_size = 1.7;
                player->rect.width = 36 * player->scale_size;
                player->rect.height = 70 * player->scale_size;
                player->rect.x = position.x;
                player->rect.y = position.y;
                player->velocity = (Vector2) { 0, 0 };
                player->speed = speed;
                player->health = 10;
                player->sprint = 3.7;
                player->jumpforce = 17;
                player->gravity = 0.85;
                player->strength = 11;
                player->attack_count = 0;
                player->controls = controls;

                player->special_frame = 0;
                player->unspecial_frame = 0;
                player->attack_frame = 0;
                player->attack_alt_frame = 0;
                player->death_frame = 0;
                player->frame_time = 0.0;

                player->is_walking = false;
                player->is_sprinting = false;
                player->is_jumping = false;
                player->is_attacking = false;
                player->attack_toggle = false;
                player->is_specialing = false;
                player->is_unspecialing = false;
                player->is_doublejumping = false;
                
                player->on_ground = false;
                player->hiding = false;
                player->can_doublejump = true;
                player->alive = true;
                player->direction = 1;

                player->texture_standing_right = LoadTexture("assets/player/standing_right.png");
                player->texture_standing_left= LoadTexture("assets/player/standing_left.png");
                player->texture_walking_right = LoadTexture("assets/player/walking_right.png");
                player->texture_walking_left = LoadTexture("assets/player/walking_left.png");
                player->texture_sprinting_right = LoadTexture("assets/player/sprinting_right.png");
                player->texture_sprinting_left = LoadTexture("assets/player/sprinting_left.png");
                player->texture_jumping_right = LoadTexture("assets/player/jumping_right.png");
                player->texture_jumping_left = LoadTexture("assets/player/jumping_left.png");
                player->texture_attack_right = LoadTexture("assets/player/attack_right.png");
                player->texture_attack_left = LoadTexture("assets/player/attack_left.png");
                player->texture_attack_alt_right = LoadTexture("assets/player/attack_alt_right.png");
                player->texture_attack_alt_left = LoadTexture("assets/player/attack_alt_left.png");
                player->texture_special_right = LoadTexture("assets/player/special_right.png");
                player->texture_special_left = LoadTexture("assets/player/special_left.png");
                player->texture_unspecial_right = LoadTexture("assets/player/unspecial_right.png");
                player->texture_unspecial_left = LoadTexture("assets/player/unspecial_left.png");
                player->texture_death_right = LoadTexture("assets/player/death_right.png");
                player->texture_death_left = LoadTexture("assets/player/death_left.png");
}



// ......................................... CHECKS WHETHER A PLAYER IS ON A PLATFORM OR NOT AND RETURNS THE INDEX OF THE PLATFORM WHERE the PLAYER IS .........................................
int player_on_platform(const Player *player, const Platform *platforms, int count)
{
    for(int i = 0; i < count; i++)
    {
        Rectangle platform_rec =
        {
            .x = platforms[i].rect.x,
            .y = platforms[i].rect.y,
            .width = platforms[i].rect.width,
            .height = platforms[i].rect.height
        };

        Rectangle player_rec =
        {
            .x = player->rect.x + 10,
            .y = player->rect.y + player->rect.height - player->rect.height * 0.2,
            .width = player->rect.width - 15,
            .height = player->rect.height * 0.2 + 1
        };

        if(CheckCollisionRecs(player_rec, platform_rec))
        {
            return i;
        }
    }

    return -1;
}



// ......................................... UPDATES THE PLAYER IN EACH FRAME OF THE GAME .........................................
void player_update(Player *player, Platform *platform, Enemy *enemy, Bullet *bullets[], int world_width, const Sfx *sound)
{
                // UPDATE ONLY IF THE PLAYER IS ALIVE
                if (player->alive)
                {   
                    // ......................................... WALKING AND RUNNING LOGIC FOR THE PLAYER
                    // RESET THE VELOCITY BACK TO ZERO IN EACH FRAME
                    player->velocity.x = 0;
                    // BOUND THE PLAYER INSIDE THE WORLD WIDTH
                    player->rect.x = fmin(world_width + 132 - player->rect.width, fmax(0, player->rect.x));

                    // LEFT MOVEMENT
                    if (IsKeyDown(player->controls.move_left) && !player->is_specialing && !player->is_unspecialing)
                    {
                        player->is_walking = true;
                        player->direction = -1;
                        // ADDRESSING SPRINT
                        if (IsKeyDown(player->controls.sprint))
                        {
                            player->is_sprinting = true;
                            player->velocity.x = -player->speed * player->sprint * 1;
                        }
                        else
                        {
                            player->is_sprinting = false;
                            player->velocity.x = -player->speed;
                        }
                    }
                    // RIGHT MOVEMENT
                    else if (IsKeyDown(player->controls.move_right) && !player->is_specialing && !player->is_unspecialing)
                    {
                        player->is_walking = true;
                        player->direction = 1;
                        // ADDRESSING SPRINT  
                        if (IsKeyDown(player->controls.sprint))
                        {
                            player->is_sprinting = true;
                            player->velocity.x = player->speed * player->sprint * 1;
                        }
                        else
                        {
                            player->is_sprinting = false;
                            player->velocity.x = player->speed;
                        }
                    }
                    // IF NOT MOVING THEN SET THE RELATED FLAGS TO FALSE
                    else
                    {
                        player->is_walking = false;
                        player->is_sprinting = false;
                    }

                    // ADD THE RESULTANT VELOCITY GOT FROM MOVEMENT TO THE PLAYER'S POSITION
                    player->rect.x += player->velocity.x;



                    // ......................................... GRAVITY & JUMPING LOGIC
                    // GRAVITY LOGIC
                    player->velocity.y += player->gravity;
                    player->rect.y += player->velocity.y;

                    // CHECK ON WHICH PLATFORM THE PLAYER IS ON 
                    int current_platform = player_on_platform(player, platform, PLATFORM_COUNT);
                    if (current_platform != -1)
                    {
                        // IF PLAYER IS MOVING DOWN, ADDRESS COLLISION WITH THE PLATFROM BENEATH
                        if(player->velocity.y > 0)
                        {
                            // ADDRESSING COLLISION
                            player->rect.y = platform[current_platform].rect.y - player->rect.height;
                            player->velocity.y = 0;
                            // IF THE PLATFORM IS THE GROUND, SET THE RELATED FLAG TO TRUE
                            if (current_platform == 0 || current_platform == 1 || current_platform == 2)
                            {
                                player->on_ground = true;
                            }
                            else
                            {
                                player->on_ground = false;
                            }
                            // OTHER JUMPING RELATED FLAGS
                            player->is_jumping = false;
                            player->is_doublejumping = false;
                            player->can_doublejump = true;
                        }
                        // THE JUMPING LOGIC
                        if (IsKeyPressed(player->controls.jump) && !player->is_specialing && !player->is_unspecialing && !player->is_attacking)
                        {
                            player->is_jumping = true;
                            player->can_doublejump = true;
                            player->velocity.y -= player->jumpforce;
                            player->rect.y += player->velocity.y;
                            PlaySound(sound->jump);
                        }

                    }
                    // THE DOUBLE JUMPING LOGIC
                    else if (IsKeyPressed(player->controls.jump) && player->can_doublejump && !player->is_specialing && !player->is_unspecialing)
                    {
                        player->is_doublejumping = true;
                        player->can_doublejump = false;
                        player->velocity.y -= 1.1 * player->jumpforce;
                        player->rect.y += player->velocity.y;
                        PlaySound(sound->jump);
                    }



                    // ......................................... SPECIAL ABILITY LOGIC
                    if (IsKeyDown(player->controls.special) && !player->is_jumping && !player->is_walking && !player->is_sprinting)
                    {
                        player->is_specialing = true;
                        player->is_unspecialing = false;
                        player->unspecial_frame = 0;

                        if (player->special_frame == 3 || player->special_frame == 4 || player->unspecial_frame == 0 || player->unspecial_frame == 1)
                        {
                            player->hiding = true;  // MAIN FLAG REGARDING SPECIAL ABILITY
                            if (player->special_frame == 3) PlaySound(sound->hiding);
                        }
                    }
                    else
                    {
                        player->is_specialing = false;
                        if (IsKeyReleased(player->controls.special))
                        {
                            player->is_unspecialing = true;
                        }
                        if (player->unspecial_frame == 4)
                        {
                            player->is_unspecialing = false;
                        }
                        player->special_frame = 0;

                        if (!(player->special_frame == 3 || player->special_frame == 4 || player->unspecial_frame == 0 || player->unspecial_frame == 1))
                        {
                            player->hiding = false; // MAIN FLAG REGARDING SPECIAL ABILITY
                        }
                    }

                    
                    
                    // ......................................... ATTACK LOGIC
                    if (IsKeyPressed(player->controls.attack) && !player->is_jumping)
                    {
                        player->is_attacking = true;
                        player->attack_count++;
                        // TOGGLING THE ATTACK ANIMATION AFTER EACH ATTACK
                        if (player->attack_count == 2)
                        {
                            player->attack_frame = 0;
                            player->attack_toggle = false;
                            player->attack_count = 0;
                        }
                        else
                        {
                            player->attack_alt_frame = 0;
                            player->attack_toggle = true;
                        }
                        
                        // DAMAGING THE ENEMY
                        for (int i = 0; i < MAX_ENEMIES; i++) if (CheckCollisionRecs(player->rect, enemy[i].rect))
                        {
                            enemy[i].health--;
                            PlaySound(sound->attack);
                            // printf("!!!!!!!!!##### %d\n", enemy[i].health);
                        }
                        else PlaySound(sound->attack_miss);
                    }



                    // ......................................... GETTING DAMAGED BY BULLETS
                    for (int i = 0; i < MAX_BULLETS; i++) if (bullets[i])
                    {
                        if (CheckCollisionRecs((Rectangle) { bullets[i]->position.x, bullets[i]->position.y, bullets[i]->texture.width / 2 / 2, bullets[i]->texture.height / 2}, player->rect))
                        {
                            player->health--;
                            bullet_free(bullets, i);
                            PlaySound(sound->hit);
                        }
                    }



                    // ......................................... IS PLAYER DEAD?
                    player_dead(player, &platform[0], sound);
                }



                // ......................................... COLLISION WITH THE ENEMY
                for (int i = 0; i < MAX_ENEMIES; i++) if (CheckCollisionRecs(player->rect, enemy[i].rect))
                {
                    if (enemy[i].alive)
                    {    
                        if (player->rect.x < enemy[i].rect.x) if (player->rect.x + player->rect.width >  enemy[i].rect.x + 25)
                        {
                            player->rect.x = enemy[i].rect.x - player->rect.width + 25;
                        }
                        if (player->rect.x > enemy[i].rect.x) if (player->rect.x < enemy[i].rect.x + enemy[i].rect.width - 25)
                        {
                            player->rect.x = enemy[i].rect.x + enemy[i].rect.width - 25;
                        }
                        // if (player->rect.y < enemy[i].rect.y) if (player->rect.y + player->rect.height > enemy[i].rect.y)
                        // {
                        //     player->rect.y = enemy[i].rect.y - player->rect.height;
                        // }
                        // if (player->rect.y > enemy[i].rect.y) if (player->rect.y < enemy[i].rect.y + enemy[i].rect.height)
                        // {
                        //     player->rect.y = enemy[i].rect.y + enemy[i].rect.height;
                        // }
                    }
                }



                // ......................................... CHANGING HITBOX / BOUNDING BOX W.R.T WHAT THE PLAYER DOES, IN ORDER TO DRAW THE TEXTURES PROPERLY
                if (player->is_jumping)
                {
                    player->rect.width = 57 * player->scale_size;
                }
                if (player->is_sprinting && !player->is_jumping)
                {
                    player->rect.width = 49 * player->scale_size;
                }
                if (player->is_walking && !player->is_sprinting && !player->is_jumping)
                {
                    player->rect.width = 36 * player->scale_size;
                }
                if (!player->is_walking && !player->is_sprinting && !player->is_jumping)
                {
                    player->rect.width = 36 * player->scale_size;
                }
                if ((player->is_specialing || player->is_unspecialing) && !player->is_walking && !player->is_sprinting && !player->is_jumping)
                {
                    player->rect.width = 36 * player->scale_size;           
                }
                if (player->is_attacking && !player->is_jumping)
                {
                    if (player->attack_toggle)
                    {
                        player->rect.width = 65 * player->scale_size;
                    }
                    else
                    {
                        player->rect.width = 44 * player->scale_size;
                    }
                }
                 if (!player->alive)
                {
                    player->rect.width = 78 * player->scale_size;
                }
}



// ......................................... DRAW / RENDER AND ANIMATE THE PLAYER .........................................
void player_draw(Player *player, int *frame, double *interval, int *frame_count, const Sfx *sound)
{    
                Rectangle source_rect;
                Vector2 origin = (Vector2) { 0, 0 };

                // ......................................... DEATH ANIMATION
                if (!player->alive)
                {
                    *interval = 1.0 / 8.0;
                    *frame_count = 3;
                    // NEED SEPARATE FRAME-RELATED VARIABLES FOR DEATH ANIMATION
                    player->frame_time += GetFrameTime();
                    if (player->frame_time >= *interval)
                    {
                        player->frame_time = 0.0;
                        player->death_frame++;
                        if (player->death_frame >= *frame_count)
                        {
                            player->death_frame = *frame_count;
                            is_gameover = true;
                        }
                    }
                    source_rect = (Rectangle) { player->death_frame * (player->texture_death_right.width / 4.0), 0, player->texture_death_right.width / 4.0, player->texture_death_right.height };               
                    player->texture = (player->direction == 1)? player->texture_death_right: player->texture_death_left; 
                }

                // ......................................... JUMPING ANIMATION
                else if (player->is_jumping)
                {
                    *interval = 1.0 / 9.0;
                    *frame_count = 5;
                    source_rect = (Rectangle) { *frame * (player->texture_jumping_right.width / 6.0), 0, player->texture_jumping_right.width / 6.0, player->texture_jumping_right.height };               
                    player->texture = (player->direction == 1)? player->texture_jumping_right: player->texture_jumping_left; 
                }

                // DOUBLE JUMPING ANIMATION? . . .

                // ......................................... ATTACKING ANIMATION
                else if (player->is_attacking && !player->is_jumping)
                {
                    // ATTACK ANIMATION TOGGLING
                    if (!player->attack_toggle)
                    {
                        *interval = 1.0 / 14.0;
                        *frame_count = 5;
                        // NEED SEPARATE VARIABLES FOR ATTACKING ANIMATION TOO
                        player->frame_time += GetFrameTime();
                        if (player->frame_time >= *interval)
                        {
                            player->frame_time = 0.0;
                            player->attack_frame++;
                            if (player->attack_frame > *frame_count)
                            {
                                player->is_attacking = false;
                                player->attack_frame = 0;
                            }
                        }
                        source_rect = (Rectangle) { player->attack_frame * (player->texture_attack_right.width / 6.0), 0, player->texture_attack_right.width / 6.0, player->texture_attack_right.height };               
                        player->texture = (player->direction == 1)? player->texture_attack_right: player->texture_attack_left; 
                    }
                    // ATTACK ANIMATION TOGGLING
                    else
                    {
                        *interval = 1.0 / 11.0;
                        *frame_count = 3;
                        // NEED SEPARATE VARIABLES FOR ATTACKING ANIMATION TOO                        
                        player->frame_time += GetFrameTime();
                        if (player->frame_time >= *interval)
                        {
                            player->frame_time = 0.0;
                            player->attack_alt_frame++;
                            if (player->attack_alt_frame >= *frame_count)
                            {
                                player->is_attacking = false;
                                player->attack_alt_frame = 0;
                            }
                        }
                        source_rect = (Rectangle) { player->attack_alt_frame * (player->texture_attack_alt_right.width / 4.0), 0, player->texture_attack_alt_right.width / 4.0, player->texture_attack_alt_right.height };               
                        player->texture = (player->direction == 1)? player->texture_attack_alt_right: player->texture_attack_alt_left; 
                    }
                }

                // ......................................... SPRINTING ANIMATION
                else if (player->is_sprinting)
                {
                    *interval = 1.0 / 9.0;
                    *frame_count = 5;
                    source_rect = (Rectangle) { *frame * (player->texture_sprinting_right.width / 6.0), 0, player->texture_sprinting_right.width / 6.0, player->texture_sprinting_right.height };               
                    player->texture = (player->direction == 1)? player->texture_sprinting_right: player->texture_sprinting_left; 
                }

                // ......................................... WALKING ANIMATION
                else if (player->is_walking)
                {
                    *interval = 1.0 / 9.0;
                    *frame_count = 5;
                    source_rect = (Rectangle) { *frame * (player->texture_walking_right.width / 6.0), 0, player->texture_walking_right.width / 6.0, player->texture_walking_right.height };               
                    player->texture = (player->direction == 1)? player->texture_walking_right: player->texture_walking_left; 
                }

                // ......................................... SPECIAL ABILITY ANIMATION
                else if (player->is_specialing)
                {
                    *interval = 1.0 / 15.0;
                    *frame_count = 4;
                    // NEED SEPARATE VARIABLES FOR SPECIAL ANIMATION TOO
                    player->frame_time += GetFrameTime();
                    if (player->frame_time >= *interval)
                    {
                        player->frame_time = 0.0;
                        player->special_frame++;
                        if (player->special_frame > *frame_count)
                        {
                            player->special_frame = *frame_count;
                        }
                    }
                    source_rect = (Rectangle) { player->special_frame * (player->texture_special_right.width / 5.0), 0, player->texture_special_right.width / 5.0, player->texture_special_right.height };               
                    player->texture = (player->direction == 1)? player->texture_special_right: player->texture_special_left; 
                }

                // ......................................... UNDO-SPECIAL-ABILITY ANIMATION
                else if (player->is_unspecialing)
                {
                    *interval = 1.0 / 15.0;
                    *frame_count = 4;
                    // NEED SEPARATE VARIABLES FOR THIS TOO
                    player->frame_time += GetFrameTime();
                    if (player->frame_time >= *interval)
                    {
                        player->frame_time = 0.0;
                        player->unspecial_frame++;
                        if (player->unspecial_frame > *frame_count)
                        {
                            player->unspecial_frame = *frame_count;
                        }
                    }
                    source_rect = (Rectangle) { player->unspecial_frame * (player->texture_unspecial_right.width / 5.0), 0, player->texture_unspecial_right.width / 5.0, player->texture_unspecial_right.height };               
                    player->texture = (player->direction == 1)? player->texture_unspecial_right: player->texture_unspecial_left; 
                }

                // ......................................... IDLE / STANDING ANIMATION
                else
                {
                    *interval = 1.0 / 8.0;
                    *frame_count = 5;
                    source_rect = (Rectangle) { *frame * (player->texture_standing_right.width / 6.0), 0, player->texture_standing_right.width / 6.0, player->texture_standing_right.height };               
                    player->texture = (player->direction == 1)? player->texture_standing_right: player->texture_standing_left; 
                }

                // ......................................... AFTER SELECTING THE APPROPRIATE TEXTURE, RENDER / DRAW IT
                DrawTexturePro(player->texture, source_rect, player->rect, origin, 0, WHITE);

                // HITBOX / BOUNDING BOX
                // DrawRectangleRec(player->rect, Fade(BLUE, 0.05));
}



// ......................................... ADDRESS THE DEATH OF THE PLAYER .........................................
void player_dead(Player *player, Platform *ground, const Sfx *sound)
{
                // IF THE PLAYER'S HEALTH GOES DOWN TO ZERO OR THE PLAYER FALLS BELOW THE GROUND HEIGHT, THE PLAYER DIES
                if ((player->health <= 0)  && (player->on_ground) || 
                    (player->rect.y + player->rect.height > ground->rect.y + ground->rect.height))
                {
                    player->alive = false;
                    player->health = 0;
                    player->velocity = (Vector2) { 0 };
                    player->speed = 0.0;
                    PlaySound(sound->dead);
                }
}