#include "game.h"


void background_init(Background *background, int size)
{
    background[0].texture2 = LoadTexture("assets/background2.png");
    background[1].texture2 = LoadTexture("assets/background2.png");
    int multiplier = 0;
    for (int i = 0; i < 10; i++, multiplier++)
    {
        background[i].texture = LoadTexture("assets/background.png");

        background[i].srec.x = 0;
        background[i].srec.y = 0;
        background[i].srec.width = 512;
        background[i].srec.height = 720;

        background[i].drec.width = background->srec.width * 2.2;
        background[i].drec.height = background->srec.height * 2.2;
        background[i].drec.x = multiplier * background[i].drec.width;
        background[i].drec.y = 0;
    }
}

void background_update(Background *background, int size)
{
    
}

void background_draw(Background *background, int size)
{
    DrawTexture(background[0].texture2, 3320, 400, WHITE);
    DrawTexture(background[1].texture2, 8955, 400, WHITE);
    for (int i = 0; i < 10; i++)
    {
        DrawTexturePro(background[i].texture, background[i].srec, background[i].drec, (Vector2) { 0, 0 }, 0, WHITE);
    }
}