#ifndef MORPION36_CORE_H
#define MORPION36_CORE_H

#include "types.h"


bool valid_coord(int x, int y);

int value_code(int n, bool is_value_max);

void eval_1_position(game_tab tab, pos position, int* code, player* pl_max);

void eval_1_quintuplet(int* code, int* value_max, int* value_min);

void points_verticale(game_tab tab, pos position, int* value_max, int* value_min, player* pl_max);

void points_horizontal(game_tab tab, pos position, int* value_max, int* value_min, player* pl_max);

void points_bas_gauche(game_tab tab, pos position, int* value_max, int* value_min, player* pl_max);

void points_haut_gauche(game_tab tab, pos position, int* value_max, int* value_min, player* pl_max);

void eval_position(game_tab tab, pos position, player* pl_max);

void init_value(game_tab tab, player* pl_max);

void calc_value(game_tab tab, player* pl_max, pos position);

player* init_player(char team, bool robot);

token*** create_game_tab(int x,int y);//Fait

void freeTab(token*** tab, int x,int y); //Fait

token* initToken(int x,int y);//Fait

void initialize_Tab(token*** TplateauJeu,int x,int y);//Fait

//Fonctions pour determiner si un quintuplet a été formé...

bool finishquintupletvertical(game_tab tab,pos thePosition, char player);

bool finishquintuplethorizontal(game_tab tab, pos thePosition, char player);

bool finishquintupletHautGauche(game_tab tab, pos thePosition, char player);

bool finishquintupletBasGauche(game_tab tab, pos thePosition, char player);

bool thefinishquintuplet(game_tab tab, pos thePosition, char player);//A appelé dans le main dans la boucle while ou do while du jeu...

pos bestPosition(game_tab tab, player* pl_max);

void poseTokenOnGameTab(game_tab tab, pos theposition, char sign);

void play(game_tab tab, player *pl1, player *pl2, int tourJeu, bool bot_vs_human, bool *finishMorpion);

void set_players(bool bot_vs_human, player *pl);

bool all_round_display(bool bot_vs_human);

void play_human(game_tab tab, player *pl, bool *finishMorpion, bool bot_vs_human);

void play_bot(game_tab tab, player *pl, player *pl_min, bool *finishMorpion, bool bot_vs_human);

void display(game_tab tab, bool finish_display);

game_tab copy_tab(game_tab tab);
int max(int a, int b);
int min(int a, int b);
tuple minimax(game_tab tab, player *pl_max, player *pl_min, int depth, int alpha, int beta, pos postion, bool isMax);

#endif //MORPION36_CORE_H