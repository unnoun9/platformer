#include "game.h"



// ......................................... SETS UP AN ENEMY .........................................
void enemy_init(Enemy *enemy, Vector2 position, double speed, int health, double scale, double size, int buff, int delay)
{
                enemy->scale_size = 1.6;

                enemy->rect.width = 38 * enemy->scale_size * size + buff;
                enemy->rect.height = 80 * enemy->scale_size * size;
                enemy->rect.x = position.x;
                enemy->rect.y = position.y;

                enemy->vision_rect.width = 1.1 * WINDOW_X * scale;
                enemy->vision_rect.height = 0.8 * WINDOW_X * scale;
                enemy->vision_rect.x = enemy->rect.x;
                enemy->vision_rect.y = enemy->rect.y;

                enemy->shooting_range_rect = enemy->vision_rect;
                enemy->shooting_range_rect.width = 0.55 * enemy->vision_rect.width * scale;
                enemy->shooting_range_rect.height = 0.3 * enemy->vision_rect.height * scale;

                enemy->walking_rect = enemy->vision_rect;
                enemy->walking_rect.width = 0.75 * enemy->vision_rect.width * scale;
                enemy->walking_rect.height = 0.25 * enemy->vision_rect.height * scale;

                enemy->velocity = (Vector2) { 0, 0 };
                enemy->speed = speed;
                enemy->health = health;
                enemy->gravity = 0.85;
                enemy->attack_cooldown = 0;
                enemy->attack_delay = delay;

                enemy->activating_frame = 0;
                enemy->attack_frame = 0;
                enemy->death_frame = 0;
                enemy->frame_time = 0.0;

                enemy->alive = true;
                enemy->activated = false;
                enemy->is_deactivated = true;
                enemy->is_walking = false;
                enemy->is_activating = false;
                enemy->is_idle = false;
                enemy->is_attacking = false;
                enemy->is_bullet_initiated = false;
                int random = rand() % 2;
                if (random == 0) enemy->direction = 1;
                else enemy->direction = -1;

                enemy->texture_deactivated_right = LoadTexture("assets/enemy/deactivated_right.png");
                enemy->texture_deactivated_left = LoadTexture("assets/enemy/deactivated_left.png");
                enemy->texture_activating_right = LoadTexture("assets/enemy/activating_right.png");
                enemy->texture_activating_left = LoadTexture("assets/enemy/activating_left.png");
                enemy->texture_idle_right = LoadTexture("assets/enemy/idle_right.png");
                enemy->texture_idle_left = LoadTexture("assets/enemy/idle_left.png");
                enemy->texture_walking_right = LoadTexture("assets/enemy/walking_right.png");
                enemy->texture_walking_left = LoadTexture("assets/enemy/walking_left.png");
                enemy->texture_attack_right = LoadTexture("assets/enemy/attack_right.png");
                enemy->texture_attack_left = LoadTexture("assets/enemy/attack_left.png");
                enemy->texture_death_right = LoadTexture("assets/enemy/death_right.png");
                enemy->texture_death_left = LoadTexture("assets/enemy/death_left.png");
}



// ......................................... CHECKS ON WHICH PLATFORM THE ENEMY COLLIDES .........................................
int enemy_on_platform(const Enemy *enemy, const Platform *platforms, int count)
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

                    Rectangle enemy_rec =
                    {
                        .x = enemy->rect.x + 10,
                        .y = enemy->rect.y + enemy->rect.height - enemy->rect.height * 0.2,
                        .width = enemy->rect.width - 15,
                        .height = enemy->rect.height * 0.2 + 1
                    };

                    if(CheckCollisionRecs(enemy_rec, platform_rec))
                    {
                        return i;
                    }
                }

                return -1;
}



// ......................................... UPDATES THE ENEMY ON EACH FRAME .........................................
void enemy_update(Enemy *enemy, Player * player, Platform *platform, Bullet *bullets[], const Sfx *sound, int index)
{
                // DO THE FOLLOWING ONLY IF THE ENEMY IS ALIVE
                if (enemy->alive)
                {
                    // ......................................... DIRECTION UPDATION
                    // DIRECTION IS WHERE THE PLAYER IS
                    enemy->direction = (player->rect.x < enemy->rect.x)? -1: 1;
                    


                    // ......................................... RECTANGLES USED FOR ENEMY'S AI
                    // UPDATING THE POSITIONS OF RECTANGLES REPRESENTING THE ENEMY'S VISION, SHOOTING AND WALKING RADIUS 
                    enemy->vision_rect.x = enemy->rect.x - enemy->vision_rect.width / 2 + enemy->rect.width / 2;
                    enemy->vision_rect.y = enemy->rect.y - enemy->vision_rect.height / 2 + enemy->rect.height / 2;

                    enemy->shooting_range_rect.x = enemy->rect.x - enemy->shooting_range_rect.width / 2 + enemy->rect.width / 2;
                    enemy->shooting_range_rect.y = enemy->rect.y - enemy->shooting_range_rect.height / 2 + enemy->rect.height / 2;

                    enemy->walking_rect.x = enemy->rect.x - enemy->walking_rect.width / 2 + enemy->rect.width / 2;
                    enemy->walking_rect.y = enemy->rect.y - enemy->walking_rect.height / 2 + enemy->rect.height / 2;



                    // ......................................... ACTIVATING
                    // WHEN THE PLAYER WALKS INSIDE THE VISION RADIUS, THE ENEMY IS OFFICIALLY ACTIVATED
                    if (CheckCollisionRecs(enemy->vision_rect, player->rect))
                    {
                        enemy->is_activating = true;
                        enemy->is_deactivated = false;
                    }



                    // ......................................... MOVING TOWARDS THE PLAYER
                    // IF THE PLAYER IS INSIDE THE WALKING RADIUS, AND THE ENEMY ISN'T CURRENTLY ATTACKING, THE ENEMY WALKS TOWARDS THE PLAYER
                    if (!enemy->is_attacking && !enemy->is_deactivated && player->alive  && 
                        CheckCollisionRecs(player->rect, enemy->walking_rect) && !player->hiding)
                    {
                        enemy->is_walking = true;
                        // WALKING LOGIC
                        enemy->rect.x += enemy->direction * enemy->speed;
                    }
                    else
                    {
                        enemy->is_walking = false;
                    }



                    // ......................................... ATTACKING THE PLAYER
                    // IF THE PLAYER IS INSIDE THE SHOOTING RADIUS, THE ENEMY SHOOTS THE PLAYER SO LONG AS THE PLAYER IS ALIVE AND IS NOT HIDING
                    if (CheckCollisionRecs(player->rect, enemy->shooting_range_rect) && !player->hiding && player->alive)
                    {
                        // IF THE ENEMT ATTACK COOLDOWN IS ZERO THEN THE ENEMY CAN ATTACK
                        if (enemy->attack_cooldown <= 0)
                        {
                            enemy->is_attacking = true;

                            // INITIATE THE BULLET AT THE FIFTH FRAME OF THE ATTACKING / SHOOTING ANIMATION OF THE ENEMY, SATISFYING THE CONDITION THAT IT ISN'T ALREADY INITIALIZED
                            if (enemy->attack_frame == 5 && !enemy->is_bullet_initiated)
                            {
                                // BULLET INITIALIZATION ACCORDING TO THE DIRECTION
                                if (enemy->direction == 1)
                                {
                                    bullet_init(bullets, (Vector2) { enemy->rect.x + enemy->rect.width - 10, enemy->rect.y + 24 } , 8);
                                }
                                else
                                {
                                    bullet_init(bullets, (Vector2) { enemy->rect.x - 30, enemy->rect.y + 24 } , -8);
                                }
                                enemy->is_bullet_initiated = true;
                                enemy->attack_cooldown = enemy->attack_delay;
                                PlaySound(sound->shot);
                            }
                        }
                    }
                    // IF THE ENEMY CAN'T ATTACK (POSSIBLY DUE TO COOLDOWN OR THE is_bullet_initiated FLAG), MAKE SURE THAT THE ANIMATION ISN'T TRIGGERED
                    else
                    {
                        enemy->is_attacking = false;
                        enemy->attack_frame = 0;
                    }

                    // ONCE THE BULLET IS INITIATED MAKE SURE THAT ANOTHER IS NOT INITIATED (SO THAT ONLY ONE BULLET EXISTS FOR THAT ENEMY)
                    if (enemy->is_bullet_initiated && enemy->attack_frame == 0)
                    {
                        enemy->is_bullet_initiated = false;
                    }

                    // IF COOLDOWN IS MORE THAN ZERO, DECREMENT IT UNTIL IT IS (COOLDOWN IS COUNTED BY FRAMES)
                    if (enemy->attack_cooldown > 0)
                    {
                        enemy->attack_cooldown--;
                    }



                    // ......................................... IS THE ENEMY DEAD?
                    enemy_dead(enemy, player, sound, &platform[0]);
                }
                // UPDATE THE ALREADY INITIALIZED BULLETS (IF ANY), REGARDLESS OF THE ENEMY BEING DEAD OR NOT
                bullet_update(bullets, enemy);




                // ......................................... COLLISION WITH THE PLAYER
                if (CheckCollisionRecs(player->rect, enemy->rect))
                {

                }



                // ......................................... GRAVITY
                // GRAVITY LOGIC
                enemy->velocity.y += enemy->gravity;
                enemy->rect.y += enemy->velocity.y;
                // COLLISION DETECTION FOR PLATFORMS
                bool current_platform = enemy_on_platform(enemy, platform, PLATFORM_COUNT);
                if (current_platform != -1)
                {
                    enemy->rect.y = platform[current_platform].rect.y - enemy->rect.height;    
                    enemy->velocity.y = 0;
                }


                // ......................................... HITBOX CHANGES ACCORDING TO THE ACTIONS OF THE ENEMY, TO DRAW THE TEXTURES PROPERLY
                if (enemy->is_activating)
                {
                    enemy->rect.width = 49 * enemy->scale_size;
                }
                if (enemy->is_deactivated)
                {
                    enemy->rect.width = 38 * enemy->scale_size;
                }
                if (enemy->is_idle)
                {
                    enemy->rect.width = 38 * enemy->scale_size;
                }
                if (enemy->is_attacking)
                {
                    enemy->rect.width = 62 * enemy->scale_size;
                }
                if (!enemy->alive)
                {
                    enemy->rect.width = 70 * enemy->scale_size;
                }
}



// ......................................... DRAW / RENDER THE ENEMY WITH ANIMATION  .........................................
void enemy_draw(Enemy *enemy, int *frame, double *interval, int *frame_count, const Sfx *sound)
{
                // LOGIC IS QUITE SIMILAR TO PLAYER'S ANIMATION
                Rectangle source_rect;

                Vector2 origin = {
                    .x = 0,
                    .y = 0
                };

                // ......................................... DEATH ANIMATION
                if (!enemy->alive)
                {
                    *interval = 1.0 / 10.0;
                    *frame_count = 6;
                    enemy->frame_time += GetFrameTime();
                    if (enemy->frame_time >= *interval)
                    {
                        enemy->frame_time = 0.0;
                        enemy->death_frame++;
                        if (enemy->death_frame >= *frame_count)
                        {
                            enemy->death_frame = *frame_count;
                        }
                    }
                    source_rect = (Rectangle) { enemy->death_frame * (enemy->texture_death_right.width / 7.0), 0, enemy->texture_death_right.width / 7.0, enemy->texture_death_right.height };               
                    enemy->texture = (enemy->direction == 1)? enemy->texture_death_right: enemy->texture_death_left; 
                }

                // ......................................... ATTACKING ANIMATION
                else if (enemy->is_attacking)
                {
                    *interval = 1.0 / 7.0;
                    *frame_count = 7;
                    
                    enemy->frame_time += GetFrameTime();
                    if (enemy->frame_time >= *interval)
                    {
                        enemy->frame_time = 0;
                        enemy->attack_frame++;
                        if (enemy->attack_frame >= *frame_count)
                        {
                            enemy->is_attacking = false;
                            enemy->is_idle = true;
                            enemy->attack_frame = 0;
                        }
                    }

                    source_rect = (Rectangle) {(enemy->attack_frame * enemy->texture_attack_right.width / 8.0), 0, (enemy->texture_attack_right.width / 8.0), enemy->texture_attack_right.height};
                    enemy->texture = (enemy->direction == 1)? enemy->texture_attack_right: enemy->texture_attack_left;
                }

                // ......................................... WALKING ANIMATION
                else if (enemy->is_walking)
                {
                    *interval = 1.0 / 8.0;
                    *frame_count = 7;
                    source_rect = (Rectangle) {(*frame * enemy->texture_walking_right.width / 8.0), 0, (enemy->texture_walking_right.width / 8.0), enemy->texture_walking_right.height};
                    enemy->texture = (enemy->direction == 1)? enemy->texture_walking_right: enemy->texture_walking_left;
                    if (global_time % (int) (60 / 1.95) == 0) PlaySound(sound->step);             
                }

                // ......................................... IDLE ANIMATION
                else if (enemy->is_idle)
                {
                    *interval = 1.0 / 8.0;
                    *frame_count = 4;
                    source_rect = (Rectangle) {(*frame * enemy->texture_idle_right.width / 5.0), 0, (enemy->texture_idle_right.width / 5.0), enemy->texture_idle_right.height};
                    enemy->texture = (enemy->direction == 1)? enemy->texture_idle_right: enemy->texture_idle_left;
                }

                // ......................................... ACTIVATION ANIMATION
                else if (enemy->is_activating)
                {
                    if (enemy->activating_frame == 0) PlaySound(sound->activated);
                    *interval = 1.0 / 4.0;
                    *frame_count = 3;

                    enemy->frame_time += GetFrameTime();
                    if (enemy->frame_time >= *interval)
                    {
                        enemy->frame_time = 0;
                        enemy->activating_frame++;
                        if (enemy->activating_frame >= *frame_count)
                        {
                            enemy->activating_frame = *frame_count;
                            enemy->is_activating = false;
                            enemy->is_idle = true;
                        }
                        if (enemy->activating_frame == 4)
                        {
                            enemy->activated = true;
                        }
                    }

                    source_rect = (Rectangle) {(enemy->activating_frame * enemy->texture_activating_right.width / 4.0), 0, (enemy->texture_activating_right.width / 4.0), enemy->texture_activating_right.height};
                    enemy->texture = (enemy->direction == 1)? enemy->texture_activating_right: enemy->texture_activating_left;
                }

                // ......................................... DEACTIVATED FORM
                else if (enemy->is_deactivated)
                {
                    source_rect = (Rectangle) {0, 0, enemy->texture_deactivated_right.width, enemy->texture_deactivated_right.height};
                    enemy->texture = (enemy->direction == 1)? enemy->texture_deactivated_right: enemy->texture_deactivated_left;
                }

                // ......................................... FINALLY DRAW THE TEXTURE AFTER CHOOSING THE APPROPRIATE ONE
                DrawTexturePro(enemy->texture, source_rect, enemy->rect, origin, 0, WHITE);

                // DrawRectangleRec(enemy->rect, Fade(RED, 0.05));
                DrawRectangleLines(enemy->vision_rect.x, enemy->vision_rect.y, enemy->vision_rect.width, enemy->vision_rect.height, Fade(GRAY, 0.1));
                DrawRectangleLines(enemy->shooting_range_rect.x, enemy->shooting_range_rect.y, enemy->shooting_range_rect.width, enemy->shooting_range_rect.height, Fade(RED,0.2));
                DrawRectangleLines(enemy->walking_rect.x, enemy->walking_rect.y, enemy->walking_rect.width, enemy->walking_rect.height, Fade(BLUE, 0.2));
}



// ......................................... ADDRESS THE DEATH OF THE ENEMY .........................................
void enemy_dead(Enemy *enemy, Player *player, const Sfx *sound, Platform *ground)
{
    // LOGIC IS QUITE SIMILAR TO PLAYER'S DEATH LOGIC
    if (enemy->health <= 0 || (enemy->rect.y + enemy->rect.height > ground->rect.y + 2 * ground->rect.height))
    {
        enemy->health = 0;
        enemy->alive = false;
        enemy->is_attacking = false;
        enemy->is_walking = false;
    }
}