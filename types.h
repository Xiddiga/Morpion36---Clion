#ifndef MORPION36_TYPES_H
#define MORPION36_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define X 30
#define Y 20

typedef struct player{
    char team;
    bool robot;
}player;


typedef struct pos{
    int posX;
    int posY;
}pos;

typedef struct token{
    char sign; // c (X) ou r (O) , _ si vide
    int value_r;
    int value_c;
    pos position;
    bool filled;
}token;

typedef token*** game_tab;

#endif //MORPION36_TYPES_H
