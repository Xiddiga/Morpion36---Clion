#ifndef MORPION36_DISPLAY_H
#define MORPION36_DISPLAY_H

#include <string.h>

#include "types.h"

/*
#define HORIZONTAL_BAR 205
#define VERTICAL 186
#define CORNER_LU 201
#define CORNER_RU 187
#define VERTICAL_L 204
#define VERTICAL_R 185
#define CORNER_LB 200
#define CORNER_RB 188
#define CROSS 206
#define HORIZONTAL_U 203
#define HORIZONTAL_B 202
#define BLACK_SQUARE 219
*/

#define HORIZONTAL_BAR "\xE2\x95\x90"
#define VERTICAL "\xE2\x95\x91"
#define CORNER_LU "\xE2\x95\x94"
#define CORNER_RU "\xE2\x95\x97"
#define VERTICAL_L "\xE2\x95\xA0"
#define VERTICAL_R "\xE2\x95\xA3"
#define CORNER_LB "\xE2\x95\x9A"
#define CORNER_RB "\xE2\x95\x9D"
#define HORIZONTAL_U "\xE2\x95\xA6"
#define HORIZONTAL_B "\xE2\x95\xA9"
#define CROSS "\xE2\x95\xAC"
#define BLACK_SQUARE "\xE2\x96\x88"

void print_bar(int len);

void style(bool selected, char *demand, char *selec_1, char *selec_2);

bool who_play();

bool all_results();

void print_board(game_tab board);

char print_char(token* Token);

void print_token(token* Token);

void print_tab_V1(token*** Token, int x, int y);

void print_tab_V2(token*** Token, int x, int y);

void print_tab_r(token*** Token, int x, int y);

void print_tab_c(token*** Token, int x, int y);

pos ask_pos();

void print_botwinner(player *pl);

void you_win();

void you_lose();

void print_who_win(player *pl1, bool bot_vs_human);

#endif //MORPION36_DISPLAY_H