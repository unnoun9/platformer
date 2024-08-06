#include "game.h"


// incomplete still . . .


int main(void)
{
	// ......................................... INITIALIZE WINDOW AND RELEVANT THINGS .........................................
	initialize_game();

	// ......................................... REMOVING THE GAME-EXITING KEY .........................................
	SetExitKey(KEY_NULL);

	// ......................................... DEFINITIONS OF THE GAME OBJECTS .........................................
	// PLAYER
	Player players[MAX_PLAYERS];
	Key_Binds p1_controls = { KEY_A, KEY_D, KEY_LEFT_SHIFT, KEY_C, KEY_LEFT_CONTROL, KEY_SPACE, KEY_E };

	// ENEMY
	Enemy enemies[MAX_ENEMIES];

	// WORLD AND BACKGROUND
	int world_width = 10 * WINDOW_X;
	int size = ceil(world_width / 512);
	Background background[size];

	// BULLETS
	Bullet *bullets[MAX_BULLETS] = { NULL };

	// CAMERA
	Camera2D camera = { 0 };

	// SOUNDS
	Sfx sounds;

	// FRAME-RELATED VARIABLES USED FOR ANIMATION
    int frame = 0;
    double running_time = 0.0;
    double interval = 1.0 / 6.0; 



	// ......................................... SETTING UP GAME OBJECTS BEFORE THE RESTART POINT .........................................
	// MAIN MENU BUTTONS
	Button play_button;
	Vector2 play_button_pos = (Vector2){ (WINDOW_X / 2 - 4 * 16 / 2), (WINDOW_Y / 2 - 4 * 16 / 2 - 200) };
    button_init(&play_button, play_button_pos, "PLAY");

	Button settings_button;
	Vector2 settings_button_pos = (Vector2){ (WINDOW_X / 2 - 4 * 16 / 2), (WINDOW_Y / 2 - 4 * 16 / 2) };
	button_init(&settings_button, settings_button_pos, "SETTINGS");

    Button quit_button;
    Vector2 quit_button_pos = (Vector2){ (WINDOW_X / 2 - 4 * 16 / 2), (WINDOW_Y / 2 - 4 * 16 / 2 + 200) };
    button_init(&quit_button, quit_button_pos, "QUIT");

	Button home_button;
	Vector2 home_button_pos = (Vector2){ (WINDOW_X / 2 - 4 * 16 / 2), (0.85 * WINDOW_Y) };
	button_init(&home_button, home_button_pos, "HOME");

	Button restart_button;
	Vector2 restart_button_pos = (Vector2){ (WINDOW_X / 2 - 4 * 16 / 2), (0.85 * WINDOW_Y) };
	button_init(&restart_button, restart_button_pos, "RESTART");

	Button pause_button;
	Vector2 pause_button_pos = (Vector2){ (0.9 * WINDOW_X), (0.9 * WINDOW_Y) };
	button_init(&pause_button, pause_button_pos, "PAUSE");

	Texture texture_main_menu = LoadTexture("assets/main_menu.png");


	// SFX
	sound_init(&sounds);


	// BACKGROUND
	background_init(background, size);
	


	// GROUND AND PLATFORMS
	// GAP 1: 3324 - 3690
	// GAP 2: 8960 - 9330
	Platform platforms[PLATFORM_COUNT];

	Rectangle plat_rect = (Rectangle){ 0, background[0].drec.height, 11100, 400 };
	platform_init(&platforms[0], plat_rect, Dirt_Brown);

	plat_rect = (Rectangle){ 3691, background[0].drec.height, 8960 - 3690, 400 };
	platform_init(&platforms[1], plat_rect, Dirt_Brown);

	plat_rect = (Rectangle){ 9331, background[0].drec.height, 11047 - 9000, 400 };
	platform_init(&platforms[2], plat_rect, Dirt_Brown);

	plat_rect = (Rectangle){ 1126, platforms[0].rect.y - GetRandomValue(200, 500), GetRandomValue(400, 800) , GetRandomValue(25, 60) };
	platform_init(&platforms[3], plat_rect, Dirt_Brown);

	plat_rect = (Rectangle){ 3460, platforms[0].rect.y - GetRandomValue(200, 500), GetRandomValue(400, 800) , GetRandomValue(25, 60) };
	platform_init(&platforms[4], plat_rect, Dirt_Brown);

	plat_rect = (Rectangle){ 4567, platforms[0].rect.y - GetRandomValue(200, 500), GetRandomValue(400, 800) , GetRandomValue(25, 60) };
	platform_init(&platforms[5], plat_rect, Dirt_Brown);
	
	plat_rect = (Rectangle){ 5793, platforms[0].rect.y - GetRandomValue(200, 500), GetRandomValue(400, 800) , GetRandomValue(25, 60) };
	platform_init(&platforms[6], plat_rect, Dirt_Brown);

	plat_rect = (Rectangle){ 7942, platforms[0].rect.y - GetRandomValue(200, 500), GetRandomValue(400, 800) , GetRandomValue(25, 60) };
	platform_init(&platforms[7], plat_rect, Dirt_Brown);

	plat_rect = (Rectangle){ 9125, platforms[0].rect.y - GetRandomValue(200, 500), GetRandomValue(400, 800) , GetRandomValue(25, 60) };
	platform_init(&platforms[8], plat_rect, Dirt_Brown);

	plat_rect = (Rectangle){ 9999, platforms[0].rect.y - GetRandomValue(200, 500), GetRandomValue(400, 800) , GetRandomValue(25, 60) };
	platform_init(&platforms[9], plat_rect, Dirt_Brown);



	// ......................................... RESTARTING POINT .........................................
	RESTART:



	// ......................................... SETTING UP GAME OBJECTS AFTER THE RESTART POINT .........................................
	// SETUP PLAYER
	player_init(&players[0], (Vector2) { 0.1 * WINDOW_X, platforms[0].rect.y - 70 * 1.7}, 3.14, p1_controls);



	// SETUP ENEMIES
	enemy_init(&enemies[0], (Vector2) { 1500, platforms[0].rect.y - 80 * 1.6 }, 3.5, 2, 1, 1, 0, 180);
	enemy_init(&enemies[1], (Vector2) { 3800, platforms[0].rect.y - 90 * 1.6 }, 3.5, 2, 1, 1, 0, 180);
	enemy_init(&enemies[2], (Vector2) { 4350, platforms[0].rect.y - 90 * 1.6 }, 3.5, 2, 1, 1, 0, 180);
	enemy_init(&enemies[3], (Vector2) { GetRandomValue(5000, 6000), platforms[0].rect.y - 200 * 1.6 }, 3.5, 4, 1, 1.1, 10, 120);
	enemy_init(&enemies[4], (Vector2) { GetRandomValue(5700, 6500), platforms[0].rect.y - 200 * 1.6 }, 3.5, 4, 1, 1.1, 10, 120);
	enemy_init(&enemies[5], (Vector2) { GetRandomValue(6400, 7200), platforms[0].rect.y - 200 * 1.6 }, 3.5, 4, 1, 1.1, 10, 120);
	enemy_init(&enemies[6], (Vector2) { GetRandomValue(7600, 8300), platforms[0].rect.y - 200 * 1.6 }, 3.5, 4, 1.3, 1.1, 10, 120);
	// enemy_init(&enemies[7], (Vector2) { GetRandomValue(8000, 8600), platforms[0].rect.y - 200 * 1.6}, 4, 6, 1.3, 1.2, 25, 100);
	// enemy_init(&enemies[8], (Vector2) { GetRandomValue(9400, 9640), platforms[0].rect.y - 200 * 1.6}, 4, 8, 2.0, 1.3, 40, 60);
	// enemy_init(&enemies[9], (Vector2) { 10580, platforms[0].rect.y - 200 * 1.6}, 5, 10, 1.5, 2.0, 60, 30);




	// SETUP CAMERA
	camera.offset = (Vector2) { WINDOW_X / 2 - 4 * players[0].rect.width, WINDOW_Y / 2 };
	camera.target = (Vector2) { players[0].rect.x, players[0].rect.y };
	camera.rotation = 0;
	camera.zoom = 1.0;



	// START MUSIC
	PlayMusicStream(sounds.main_menu);
	PlayMusicStream(sounds.gameplay);
	PlayMusicStream(sounds.gameover);
	


	// GAME-RUNNING VARIABLES
    Game_State current_game_state = MAIN_MENU;
    bool is_running = true;
	is_gameover = false;
    double quit_timer = 0;




	// ......................................... THE GAME LOOP .........................................
    while (!WindowShouldClose() && is_running)
    {
				// ......................................... UPDATING EVERYTHING .........................................
				// HOLDING DOWN ESCAPE WILL QUIT THE GAME, SO CHECKING FOR THAT
				if ((IsKeyDown(KEY_ESCAPE)))
				{
					quit_timer += GetFrameTime();
					if (quit_timer >= 1.0) 
					{
						is_running = false;
					}
				}
				else 
				{
					quit_timer = 0.0;
				}

				// ADDRESS THE UPDATES OF ALL GAME STATES
				switch (current_game_state)
				{
					// MAIN MENU
					case MAIN_MENU:
							UpdateMusicStream(sounds.main_menu);
							button_update(&play_button, &sounds);
							button_update(&quit_button, &sounds);
							button_update(&settings_button, &sounds);

							if (play_button.clicked)
							{
								current_game_state = GAMEPLAY;
							}
							if (quit_button.clicked)
							{
								is_running = false;
							}
							if (settings_button.clicked)
							{
								current_game_state = INFO;
							}
							break;

					// INFO SCREEN
					case INFO:
							UpdateMusicStream(sounds.main_menu);
							button_update(&home_button, &sounds);

							if (home_button.clicked)
							{
								current_game_state = MAIN_MENU;
							}
							break;


					// GAMEPLAYYYYYYY
					case GAMEPLAY:
							UpdateMusicStream(sounds.gameplay);					
							button_update(&pause_button, &sounds);

							if (pause_button.clicked)
							{
								current_game_state = MAIN_MENU;
							}
							if (is_gameover)
							{
								current_game_state = GAMEOVER;
							}

							// PLAYER LOGIC
							for (int i = 0; i < MAX_PLAYERS; i++)
							{
								camera_update(&camera, &players[i], &platforms[0], world_width);
								player_update(&players[i], platforms, enemies, bullets, world_width, &sounds);
							}

							// CPU LOGIC
							for (int i = 0; i < MAX_ENEMIES; i++)
							{
								enemy_update(&enemies[i], &players[0], platforms, bullets, &sounds, i);
							}
							break;

					// GAMEOVER
					case GAMEOVER:
							UpdateMusicStream(sounds.gameover);
							button_update(&restart_button, &sounds);

							if (restart_button.clicked)
							{
								goto RESTART;
							}
							break;
					
				}





				// ......................................... RENDERING / DRAWING EVERYTHING .........................................
				BeginDrawing();

				// ADDRESS AND RENDER ALL GAMESTATES
				switch (current_game_state)
				{
					// RENDER MAIN MENU
					case MAIN_MENU:
							ClearBackground(WHITE);
							DrawTexture(texture_main_menu, 0, 0, WHITE);
							DrawRectangle(0, 0, WINDOW_X, WINDOW_Y, Fade(BLACK, 0.8));
							DrawTexturePro(play_button.texture, play_button.srect, play_button.drect, (Vector2) { 0, 0 }, 0, WHITE);
							DrawTexturePro(quit_button.texture, quit_button.srect, quit_button.drect, (Vector2) { 0, 0 }, 0, WHITE);
							DrawTexturePro(settings_button.texture, settings_button.srect, settings_button.drect, (Vector2) { 0, 0 }, 0, WHITE);
							break;
				

					// RENDER INFO SCREEN
					case INFO:
							ClearBackground(WHITE);
							DrawTexture(texture_main_menu, 0, 0, WHITE);		
							DrawRectangle(0, 0, WINDOW_X, WINDOW_Y, Fade(BLACK, 0.8));
							DrawTexturePro(home_button.texture, home_button.srect, home_button.drect, (Vector2) { 0, 0 }, 0, WHITE);
							DrawText("HOW TO PLAY?", (GetScreenWidth() / 2 - MeasureText("HOW TO PLAY?", 30) / 2), (0.07 * GetScreenHeight()), 30, BLUE);

							DrawText("MOVEMENT KEYS =  W, A, D", (GetScreenWidth() / 2 - MeasureText("MOVEMENT KEYS =  W, A, D", 20) / 2), (0.3 * GetScreenHeight()), 20, WHITE);
							DrawText("W X2 = DOUBLE JUMP", (GetScreenWidth() / 2 - MeasureText("W X2 = DOUBLE JUMP", 20) / 2), (0.35 * GetScreenHeight()), 20, WHITE);
							DrawText("L SHIFT + MOVEMENT KEYS = SPRINT", (GetScreenWidth() / 2 - MeasureText("L SHIFT + MOVEMENT KEYS = SPRINT", 20) / 2), (0.4 * GetScreenHeight()), 20, WHITE);
							DrawText("L CTRL = DISGUISE", (GetScreenWidth() / 2 - MeasureText("L CTRL = DISGUISE", 20) / 2), (0.45 * GetScreenHeight()), 20, WHITE);
							DrawText("SPACEBAR = ATTACK", (GetScreenWidth() / 2 - MeasureText("SPACEBAR = ATTACK", 20) / 2), (0.5 * GetScreenHeight()), 20, WHITE);
							DrawText("P = PAUSE", (GetScreenWidth() / 2 - MeasureText("P = PAUSE", 20) / 2), (0.55 * GetScreenHeight()), 20, WHITE);
							DrawText("HOLD ESC FOR 1 SECOND = QUIT", (GetScreenWidth() / 2 - MeasureText("HOLD ESC FOR 1 SECOND = QUIT", 20) / 2), (0.6 * GetScreenHeight()), 20, WHITE);
							DrawText("BECAREFUL! YOU DON'T WANNA DIE.   >:)", (GetScreenWidth() / 2 - MeasureText("BECAREFUL! YOU DON'T WANNA DIE.   >:)", 20) / 2), (0.65 * GetScreenHeight()), 20, WHITE);
							break;


					// RENDER GAMEPLAY
					case GAMEPLAY:
							ClearBackground(WHITE);
							BeginMode2D(camera);

							// DRAW BACKGROUND AND MAP ELEMENTS
							background_draw(background, size);													
							DrawRectangleRec(platforms[0].rect, platforms[0].color);
							DrawRectangle(platforms[0].rect.x, platforms[0].rect.y, platforms[0].rect.width, 10, Grass_Green);
							DrawRectangleRec(platforms[1].rect, platforms[1].color);
							DrawRectangle(platforms[1].rect.x, platforms[1].rect.y, platforms[1].rect.width, 10, Grass_Green);
							DrawRectangleRec(platforms[2].rect, platforms[2].color);
							DrawRectangle(platforms[2].rect.x, platforms[2].rect.y, platforms[2].rect.width, 10, Grass_Green);
							DrawRectangleRec(platforms[3].rect, platforms[3].color);
							DrawRectangle(platforms[3].rect.x, platforms[3].rect.y, platforms[3].rect.width, 10, Grass_Green);
							DrawRectangleRec(platforms[4].rect, platforms[4].color);
							DrawRectangle(platforms[4].rect.x, platforms[4].rect.y, platforms[4].rect.width, 10, Grass_Green);
							DrawRectangleRec(platforms[5].rect, platforms[5].color);
							DrawRectangle(platforms[5].rect.x, platforms[5].rect.y, platforms[5].rect.width, 10, Grass_Green);
							DrawRectangleRec(platforms[6].rect, platforms[6].color);
							DrawRectangle(platforms[6].rect.x, platforms[6].rect.y, platforms[6].rect.width, 10, Grass_Green);
							DrawRectangleRec(platforms[7].rect, platforms[7].color);
							DrawRectangle(platforms[7].rect.x, platforms[7].rect.y, platforms[7].rect.width, 10, Grass_Green);
							DrawRectangleRec(platforms[8].rect, platforms[8].color);
							DrawRectangle(platforms[8].rect.x, platforms[8].rect.y, platforms[8].rect.width, 10, Grass_Green);
							DrawRectangleRec(platforms[9].rect, platforms[9].color);
							DrawRectangle(platforms[9].rect.x, platforms[9].rect.y, platforms[9].rect.width, 10, Grass_Green);
							
							// FRAME UPDATING LOGIC (USEFUL TO DELAY ANIMATION FRAMES BY THE "INTERVAL")
							int frame_count = 5;
							running_time += GetFrameTime();
							if (running_time >= interval)
							{
								running_time = 0.0;
								frame++;
								if (frame > frame_count)
								{
									frame = 0;
								}
							}

							// DRAW ENEMIES
							for (int i = 0; i < MAX_ENEMIES; i++)
							{
								enemy_draw(&enemies[i], &frame, &interval, &frame_count, &sounds);
							}

							// DRAW PLAYERS
							for (int i = 0; i < MAX_PLAYERS; i++)
							{
								player_draw(&players[i], &frame, &interval, &frame_count, &sounds);
							}

							// DRAW BULLETS
							for (int i = 0; i < MAX_BULLETS; i++) if (bullets[i])
							{
								Rectangle src_rec = (Rectangle) { bullets[i]->texture.width / 2 * frame, 0, bullets[i]->texture.width / 2, bullets[i]->texture.height };
								Rectangle bullet_rec = (Rectangle) { bullets[i]->position.x, bullets[i]->position.y, bullets[i]->texture.width, bullets[i]->texture.height };
								DrawTexturePro(bullets[i]->texture, src_rec, bullet_rec, (Vector2) { 0, 0 }, 0, WHITE);
							}

							EndMode2D();
							DrawTexturePro(pause_button.texture, pause_button.srect, pause_button.drect, (Vector2) { 0, 0 }, 0, WHITE);
							break;

					// GAMEOVER
					case GAMEOVER:
							DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Dark_Red);
							DrawText("YOU.", (GetScreenWidth() / 2 - MeasureText("YOU.", 70) / 2), ( 0.2 * GetScreenHeight()), 70, Fade(MAROON, 0.4));
							DrawText("JUST.", (GetScreenWidth() / 2 - MeasureText("JUST.", 70) / 2), ( 0.3 * GetScreenHeight()), 70, Fade(MAROON, 0.4));
							DrawText("DIED!", (GetScreenWidth() / 2 - MeasureText("DIED!", 70) / 2), ( 0.4 * GetScreenHeight()), 70, Fade(MAROON, 0.4));
							DrawText("MUHAHAHAHAHA!", (GetScreenWidth() / 2 - MeasureText("MUHAHAHAHAHA!", 70) / 2), ( 0.5 * GetScreenHeight()), 70, Fade(MAROON, 0.4));
							DrawText("PLEASE STAY DETEMINED", (GetScreenWidth() / 2 - MeasureText("PLEASE STAY DETEMINED", 20) / 2), (0.8 * GetScreenHeight()), 20, Fade(GOLD, 0.25));
							DrawTexturePro(restart_button.texture, restart_button.srect, restart_button.drect, (Vector2) { 0, 0 }, 0, WHITE);
							break;
				}
				// DRAW THE HOLDING ESC KEY THINGY
				DrawText("(QUITING...) DON'T QUIT!!! >:(", 15, 15, 20, Fade(Dark_Red, quit_timer / 1.5));
			EndDrawing();

			// GLOBAL FRAME PASSED SINCE GAME WAS STARTED
			global_time++;
    }
	// ......................................... END OF GAME LOOP .........................................



	// CLOSE THE WINDOW AND UNLOAD EVERYTHING
   	end_game(players, enemies, bullets, background, size, &texture_main_menu, &play_button, &quit_button, &settings_button, &home_button, &restart_button, &pause_button, &sounds);



	// SUCCESS :)
   	return 0;
}