#include <iostream>
#include <string>
#include "blocks.h"
#include "balls.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include "sstream"
using namespace std;
const int SCREEN_WIDTH = 494;
const int SCREEN_HEIGHT = 690;
SDL_Renderer *gRenderer = NULL;
SDL_Window *window = NULL;
SDL_Surface *screenSurface = NULL;
SDL_Texture *texture = NULL;
TTF_Font *font = NULL;
int texW = 100;
int texH = 100;
SDL_Rect dstrect;
SDL_Color color;
float u = 0;
const float pi = 3.1428;

void Init()
{
    window = SDL_CreateWindow("ball", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    int w = 2;
    int h = 152;
    for (int i = 0; i < 7; i++)
    {
        w = 2;
        for (int j = 0; j < 6; j++)
        {
            rect[i][j].value = 0;
            rect[i][j].xrect = w;
            rect[i][j].yrect = h;
            w += 82;
        }
        h += 52;
    }
}
void randomize(blocks rect[7][6], int level)
{
    int q;
    if (level <= 5)
        q = 1;
    else if (level > 5 && level <= 9)
        q = rand() % 2 + 1;
    else if (level > 9 && level <= 25)
        q = rand() % 2 + 2;
    else if (level > 25 && level <= 45)
        q = rand() % 3 + 2;
    else
        q = rand() % 3 + 3;

    int random[6] = {0};
    for (int i = 0; i < 6; i++)
        rect[0][i].value = 0;

    int r = rand() % 6;
    random[r] = -1;
    rect[0][r].value = -1;
    for (int i = 1; i <= q; i++)
    {
        r = rand() % 6;
        if (random[r] == 0)
            rect[0][r].value = level;
        else
            i--;
    }
}
void shift(blocks rect[7][6], int level)
{
    for (int i = 5; i >= 0; i--)
    {
        for (int j = 0; j < 6; j++)
        {
            rect[i + 1][j].value = rect[i][j].value;
        }
    }
    randomize(rect, level);
}

void shownumber(blocks rect[7][6], int i, int j)
{
    rect[i][j].convert(rect[i][j].value, rect[i][j].number);
    screenSurface = TTF_RenderText_Solid(font, rect[i][j].number, color);
    texture = SDL_CreateTextureFromSurface(gRenderer, screenSurface);
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    dstrect = {(j * 80) + ((j + 1) * 2) + 35, (i * 50) + ((i + 1) * 2) + 160, texW, texH};
    SDL_RenderCopyEx(gRenderer, texture, NULL, &dstrect, 0.0, NULL, SDL_FLIP_NONE);
    SDL_FreeSurface(screenSurface);
    SDL_DestroyTexture(texture);
}
void ShowBlock(blocks rect[7][6])
{
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (rect[i][j].value > 0)
            {
                SDL_Rect fillRect = {rect[i][j].xrect, rect[i][j].yrect, rect[i][j].Bwidth, rect[i][j].Bheight};
                SDL_SetRenderDrawColor(gRenderer, 100, 100, 100, 100);
                SDL_RenderFillRect(gRenderer, &fillRect);
                shownumber(rect, i, j);
            }
            else if (rect[i][j].value == -1)
            {
                circleRGBA(gRenderer, rect[i][j].xrect + 40, rect[i][j].yrect + 25, 12, 1, 1, 1, 255);
                filledCircleRGBA(gRenderer, rect[i][j].xrect + 40, rect[i][j].yrect + 25, 9, 200, 100, 100, 255);
            }
        }
    }
}
void drawline()
{
    SDL_SetRenderDrawColor(gRenderer, 1, 1, 1, 255);
    for (int i = 1; i <= 5; i++)
    {
        SDL_RenderDrawLine(gRenderer, 0, 95 + i, 494, 95 + i);
        SDL_RenderDrawLine(gRenderer, 0, 585 + i, 494, 585 + i);
    }
}
void showball(int s, balls gball[100])
{
    if (gball[s].value == 1)
        filledCircleRGBA(gRenderer, gball[s].xball, gball[s].yball, gball[s].rball, 255, 100, 50, 255);
    else if (gball[s].value == -1)
        filledCircleRGBA(gRenderer, gball[s].xendball, gball[s].yendball, gball[s].rball, 255, 100, 50, 255);
}
bool nextshot(int ballcount, balls gball[100])
{
    bool shot = false;
    for (int i = 0; i < ballcount; i++)
    {
        if (gball[i].value == 1)
            shot = true;
    }
    return shot;
}
void aimdrawline(float axstart, float aystart, float teta)
{
    int s = 0;
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
    while (axstart >= 2 && axstart <= 490 && aystart >= 102)
    {
        if ((s % 15 != int(u) && s % 15 != int(u) + 1 && s % 15 != int(u) + 2 && s % 15 != int(u) + 3))
            SDL_RenderDrawPoint(gRenderer, axstart, aystart);
        s++;

        axstart += cos(teta);
        aystart -= sin(teta);
    }
    u += 0.004;
    if (u > 7)
        u = 0;
}
bool lose(int ballcount, blocks rect[7][6])
{

    for (int i = 0; i < 6; i++)
    {
        if (rect[6][i].value > 0)
            return false;
        else if (rect[6][i].value == -1)
            ballcount++;
    }
    return true;
}
void effect1(int firstobject, int ballcount, balls gball[100], blocks rect[7][6])
{
    if (ballcount > 1)
    {
        for (int i = 0; i < ballcount; i++)
        {
            while (gball[i].xball > gball[firstobject].xball)
            {
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);
                gball[i].xball -= 0.25;
                drawline();
                ShowBlock(rect);
                filledCircleRGBA(gRenderer, gball[i].xball, gball[i].yball, gball[i].rball, 255, 100, 50, 255);
                SDL_RenderPresent(gRenderer);
            }
            while (gball[i].xball < gball[firstobject].xball)
            {
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);
                gball[i].xball += 0.25;
                drawline();
                ShowBlock(rect);
                filledCircleRGBA(gRenderer, gball[i].xball, gball[i].yball, gball[i].rball, 255, 100, 50, 255);
                SDL_RenderPresent(gRenderer);
            }
        }
    }
}

int main()
{
    srand(time(0));

    int level;
    bool flag1 = true;
    int count = 0;
    float xlimit;
    float axstart = 247, aystart = 579;
    float teta = pi / 2;
    int first = 0, firstobject = 0;
    int ballcount = 1;
    level = 1;
    Init();
    TTF_Init();
    font = TTF_OpenFont("Bloomsburg DEMO.ttf", 20);

    color = {200, 200, 255};
    bool quit = false;
    SDL_Event e;

    shift(rect, level);
    while (!quit)
    {

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);
        while (!(nextshot(ballcount, gball)) && (!quit))
        {
            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
            SDL_RenderClear(gRenderer);
            drawline();
            ShowBlock(rect);
            aimdrawline(axstart, aystart, teta);
            aimdrawline(axstart + 1, aystart, teta);
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
                else if (e.type == SDL_KEYDOWN)
                {

                    switch (e.key.keysym.sym)
                    {
                    case SDLK_LEFT:
                    {
                        if (teta <= 3)
                            teta += 0.01;
                    }
                    break;
                    case SDLK_RIGHT:
                    {
                        if (teta >= 0.15)
                            teta -= 0.01;
                    }
                    break;
                    case SDLK_SPACE:
                    {
                        for (int i = 0; i < ballcount; i++)
                        {
                            gball[i].value = 1;
                            gball[i].xdelta = 0.15 * cos(teta);
                            gball[i].ydelta = 0.15 * sin(teta);
                        }
                    }
                    break;
                    }
                }
            }
            filledCircleRGBA(gRenderer, gball[0].xball, gball[0].yball, gball[0].rball, 255, 100, 50, 255);
            SDL_RenderPresent(gRenderer);
        }
        first = 0;
        count = 1;
        flag1 = true;
        while (nextshot(ballcount, gball) && (!quit))
        {
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
            }
            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
            SDL_RenderClear(gRenderer);
            drawline();
            ShowBlock(rect);
            if (flag1 == true)
            {
                if (gball[count - 1].yball < 564 || gball[count - 1].xball > xlimit + 14 || gball[count - 1].xball < xlimit - 14)
                {
                    if (count < ballcount)
                        count++;
                    else
                        flag1 = false;
                }
            }
            for (int p = 0; p < count; p++)
            {
                gball[p].moveball(first);
                if (first == 1)
                {
                    firstobject = p;
                    first++;
                }
                showball(p, gball);
            }
            SDL_RenderPresent(gRenderer);
        }
        //effect1(firstobject, ballcount, gball, rect);
        for (int i = 0; i < ballcount; i++)
        {
            ballcount = ballcount + gball[i].increaseball;
        }
        level++;
        for (int i = 0; i < ballcount; i++)
            gball[i].increaseball = 0;
        for (int i = 0; i < ballcount; i++)
        {
            if (i != firstobject)
                gball[i].value = 0;
            gball[i].xball = gball[firstobject].xendball;
            gball[i].yball = gball[firstobject].yendball;
        }
        axstart = gball[firstobject].xendball;
        aystart = gball[firstobject].yendball;
        xlimit = gball[firstobject].xball;
        teta = pi / 2;
        drawline();
        shift(rect, level);
        while(!lose(ballcount,rect))
            return 0;
        SDL_RenderPresent(gRenderer);
    }
    return 0;
}