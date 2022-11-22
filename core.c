#include "core.h"
#include "display.h"

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// DEBUT CALCUL VALEURS DES POSITIONS

// verifie que des coordonnees soit dans le tableau de taile X,Y
bool valid_coord(int x, int y){
    return ((x >= 0 && x < X)&&
            (y >= 0 && y < Y));
}

// renvoie la valeur en fonciion du code n recu
int value_code(int n, bool is_value_max){
    if (is_value_max)
        return (int)(5. * pow(10, (n/6)-1));
    else
        return (int)(5. * (pow(10, n-1) - pow(10, n-2)));
}

// met à jour le pointeur code en fonction du signe mis sur le tableau
void eval_1_position(game_tab tab, pos position, int* code, player* pl_max) {
    int x = position.posX;
    int y = position.posY;

    if (tab[x][y]->sign == '_') {
        *code = *code;
    } else if(tab[x][y]->sign == pl_max->team) {
        *code = *code + 6;
    } else {
        *code = *code + 1;
    }
}

// met à jour les valeurs max et min en fonction du code
void eval_1_quintuplet(int* code, int* value_max, int* value_min) {
    if (*code == 0) {
        *value_max += 1;
        *value_min += 1;
    } else if (*code <= 5) {
        *value_max += value_code(*code, false);
        *value_min += value_code((*code) * 6, true);
    } else if (*code % 6 == 0) {
        *value_max += value_code(*code, true);
        *value_min += value_code((*code) / 6, false);
    }
}

// calcul des valeurs max et min pour une position donnée sur l'axe vertical
void points_verticale(game_tab tab, pos position, int* value_max, int* value_min, player* pl_max) {

    pos new_pos = position;

    int tmp_y = position.posY - 4;
    int x = position.posX;

    while(tmp_y <= position.posY) {
        if (!(valid_coord(x, tmp_y))) {
            tmp_y++;
        } else {
            int code = 0;

            for (int j = tmp_y; j < tmp_y + 5; j++) {
                if (!(valid_coord(x, j))) {
                    code += 13;
                } else {
                    new_pos.posY = j;
                    eval_1_position(tab, new_pos, &code, pl_max);
                }
            }

            eval_1_quintuplet(&code, value_max, value_min);

            tmp_y++;
        }
    }
}

// calcul des valeurs max et min pour une position donnée sur l'axe horizontal
void points_horizontal(game_tab tab, pos position, int* value_max, int* value_min, player* pl_max){

    pos new_pos = position;

    int tmp_x = position.posX - 4;
    int y = position.posY;

    while(tmp_x <= position.posX){
        if(!(valid_coord(tmp_x, y))){
            tmp_x++;
        } else {
            int code = 0;

            for (int i = tmp_x; i < tmp_x + 5; i++) {
                if (!(valid_coord(i, y)))
                    code += 13;
                else {
                    new_pos.posX = i;
                    eval_1_position(tab, new_pos, &code, pl_max);
                }
            }

            eval_1_quintuplet(&code, value_max, value_min);

            tmp_x++;
        }
    }
}

// calcul des valeurs max et min pour une position donnée sur l'axe de la premiere diagonale
void points_bas_gauche(game_tab tab, pos position, int* value_max, int* value_min, player* pl_max){

    pos new_pos = position;

    int tmp_x = position.posX - 4;
    int tmp_y = position.posY + 4;

    while(tmp_x <= position.posX) {
        if (!(valid_coord(tmp_x, tmp_y))) {
            tmp_x++;
            tmp_y--;
        } else {
            int code = 0;

            int i = tmp_x;
            for (int j = tmp_y; j > tmp_y - 5; j--) {

                if (!(valid_coord(i, j)))
                    code += 13;
                else {
                    new_pos.posX = i;
                    new_pos.posY = j;
                    eval_1_position(tab, new_pos, &code, pl_max);
                }
                i++;
            }

            eval_1_quintuplet(&code, value_max, value_min);

            tmp_x++;
            tmp_y--;
        }
    }
}

// calcul des valeurs max et min pour une position donnée sur l'axe de la deuxieme diagonale
void points_haut_gauche(game_tab tab, pos position, int* value_max, int* value_min, player* pl_max){

    pos new_pos = position;

    int tmp_x = position.posX - 4;
    int tmp_y = position.posY - 4;

    while(tmp_x <= position.posX){
        if(!(valid_coord(tmp_x, tmp_y))){
            tmp_x++;
            tmp_y++;
        } else {
            int code = 0;

            int i = tmp_x;
            for (int j = tmp_y; j < tmp_y + 5; j++) {

                if (!(valid_coord(i, j)))
                    code += 13;
                else {
                    new_pos.posX = i;
                    new_pos.posY = j;
                    eval_1_position(tab, new_pos, &code, pl_max);
                }
                i++;
            }

            eval_1_quintuplet(&code, value_max, value_min);

            tmp_x++;
            tmp_y++;
        }
    }
}

// evalue la valeur des deux équipes pour une position donnée
void eval_position(game_tab tab, pos position, player* pl_max) {

    int value_max = 0;
    int value_min = 0;

    points_verticale(tab, position, &value_max, &value_min, pl_max);
    points_bas_gauche(tab, position, &value_max, &value_min, pl_max);
    points_haut_gauche(tab, position, &value_max, &value_min, pl_max);
    points_horizontal(tab, position, &value_max, &value_min, pl_max);


    if (pl_max->team == 'c') {
        tab[position.posX][position.posY]->value_c = value_max;
        tab[position.posX][position.posY]->value_r = value_min;
    } else {
        tab[position.posX][position.posY]->value_c = value_min;
        tab[position.posX][position.posY]->value_r = value_max;
    }
}

// calcul des valeurs des deux équipes pour toutes les positions du plateau
void init_value(game_tab tab, player* pl_max){
    for(int i = 0; i < X; i++){
        for(int j = 0; j < Y; j++){
            pos position;
            position.posX = i;
            position.posY = j;
            eval_position(tab, position, pl_max);
        }
    }
}

// calcul des valeurs pour les 32 cases qui entourent la position donnée
void calc_value(game_tab tab, player* pl_max, pos position){

    int x = position.posX;
    int y = position.posY;

    for (int i = x-4; i <= x+4; i++) {
        if (valid_coord(i, position.posY)) {
            pos new_position;
            new_position.posX = i;
            new_position.posY = position.posY;
            eval_position(tab, new_position, pl_max);
        }
    }

    for (int i = y-4; i <= y+4; i++) {
        if (valid_coord(position.posX, i)) {
            pos new_position;
            new_position.posX = position.posX;
            new_position.posY = i;
            eval_position(tab, new_position, pl_max);
        }
    }

    int j = y-4;
    for (int i = x-4; i <= x+4; i++) {
        if (valid_coord(i, j)) {
            pos new_position;
            new_position.posX = i;
            new_position.posY = j;
            eval_position(tab, new_position, pl_max);
        }
        j++;
    }

    j = y+4;
    for (int i = x-4; i <= x+4; i++) {
        if (valid_coord(i, j)) {
            pos new_position;
            new_position.posX = i;
            new_position.posY = j;
            eval_position(tab, new_position, pl_max);
        }
        j--;
    }
}

// FIN CALCUL VALEURS DES POSITIONS
// DEBUT SELECTION DE LA MEILLEURE POSITION

// mise à jour du tableau qui contient les meilleurs positions pour une équipe donnée
void tab_best_position(token new_token, char team_max, pos* best_positions, int* best_value, int *element_nb)
{
    if(team_max == 'c') {
        if(new_token.value_c > *best_value) {
            *best_value = new_token.value_c;
            best_positions[0] = new_token.position;
            *element_nb = 1;
        } else if(new_token.value_c == *best_value) {
            best_positions[*element_nb] = new_token.position;
            (*element_nb)+=1;
        } else {
            return;
        }
    } else {
        if(new_token.value_r > *best_value) {
            *best_value = new_token.value_r;
            best_positions[0] = new_token.position;
            *element_nb = 1;
        } else if(new_token.value_r == *best_value) {
            best_positions[*element_nb] = new_token.position;
            (*element_nb)+=1;
        } else {
            return;
        }
    }
}

// renvoie la meilleure position pour une équipe donnée
pos bestPosition(game_tab tab, player* pl_max)
{
    pos thePosition;
    pos best_positions[264]; //= (pos*)malloc(sizeof(pos));
    int best_value = 0;
    int element_nb = 0;
    int *ptr_nb = &element_nb;

    char team_max = pl_max->team;

    for(int i=0;i<X;i++)
    {
        for(int j=0;j<Y;j++)
        {
            if (tab[i][j]->filled==false)
            {
                token *new_token = tab[i][j];
                tab_best_position(*new_token, team_max, best_positions, &best_value, ptr_nb);
            }
        }
    }

    int index = rand() % (element_nb);
    thePosition = best_positions[index];

    return thePosition;
}

// FIN SELECTION DE LA MEILLEURE POSITION

// poser le pion d'une équipe donnée sur une position donnée
void poseTokenOnGameTab(game_tab tab, pos theposition, char sign)
{
    tab[theposition.posX][theposition.posY]->sign=sign;
    tab[theposition.posX][theposition.posY]->filled=true;
}

// DEBUT TEST SI UN JOUEUR A GAGNE

// test si un quintuplet est fini sur l'axe vertical
bool finishquintupletvertical(game_tab tab, pos thePosition, char sign)
{
    int x = thePosition.posX, y= thePosition.posY,count=0;
    for(int i=y-4;i<=y+4;i++)
    {
        if (valid_coord(x,i))
        {
            if (tab[x][i]->sign==sign)
                count+=1;
            else
                count = 0;
        }
        if (count==5)
            break;
    }

    return (count==5)?true:false;
}

// test si un quintuplet est fini sur l'axe horizontal
bool finishquintuplethorizontal(game_tab tab, pos thePosition, char sign)
{
    int x = thePosition.posX, y= thePosition.posY,count=0;
    for(int i=x-4;i<=x+4;i++)
    {
        if (valid_coord(i,y))
        {
            if (tab[i][y]->sign==sign)
                count+=1;
            else
                count = 0;
        }
        if (count==5)
            break;
    }

    return (count==5)?true:false;
}

// test si un quintuplet est fini sur l'axe de la première diagonale
bool finishquintupletHautGauche(game_tab tab, pos thePosition, char sign)
{
    int x = thePosition.posX, y= thePosition.posY-4,count=0;
    for(int i=x-4;i<=x+4;i++)
    {
        if (valid_coord(i,y))
        {
            if (tab[i][y]->sign==sign)
                count+=1;
            else
                count = 0;
        }
        if (count==5)
            break;
        y+=1;
    }

    return (count==5)?true:false;
}

// test si un quintuplet est fini sur l'axe de la deuxième diagonale
bool finishquintupletBasGauche(game_tab tab, pos thePosition, char sign)
{
    int x = thePosition.posX, y= thePosition.posY+4,count=0;
    for(int i=x-4;i<=x+4;i++)
    {
        if (valid_coord(i,y))
        {
            if (tab[i][y]->sign==sign)
                count+=1;
            else
                count = 0;
        }
        if (count==5)
            break;
        y-=1;
    }

    return (count==5)?true:false;
}

// test si une équipe a fini un quintuplet
bool thefinishquintuplet(game_tab tab, pos thePosition, char sign)
{
    return (finishquintupletvertical(tab,thePosition,sign)|| finishquintuplethorizontal(tab,thePosition,sign) || finishquintupletBasGauche(tab,thePosition,sign) || finishquintupletHautGauche(tab,thePosition,sign) );
}

// FIN TEST SI UN JOUEUR A GAGNE
// DEBUT INITIALISATION DU JEU

// initialisation d'un joueur
player* init_player(char team, bool robot){
    player* pl = (player*)malloc(sizeof(player));
    pl->team = team;
    pl->robot = robot;

    return pl;
}

// allocation de la mémoire pour le tableau de jeu
token*** create_game_tab(int x,int y){
    token ***res = (token***)malloc(x*sizeof(token**));
    for (int i=0;i<x;i++){
        res[i] =(token**)malloc(sizeof(token*)*y);
    }

    return res;
}

// initialisation du tableau de jeu
void initialize_Tab(token*** TplateauJeu,int x,int y){
    for (int i=0;i<x;i++){
        for (int j=0;j<y;j++){
            (TplateauJeu[i][j]) = initToken(i,j);
        }
    }
}

// libération de la mémoire du tableau de jeu
void freeTab(token*** tab, int x,int y){
    for(int i=0;i<x;i++){
        free(*(tab[i]));
    }
    free(*tab);
}

// initialisation d'une case du tableau de jeu
token* initToken(int x,int y){
    token* box = (token*)malloc(sizeof(token));
    box->sign='_';
    box->position.posX=x,box->position.posY=y;
    box->filled=false;
    box->value_c=0;
    box->value_r=0;

    return box;
}

// mise à jour du player si un huamin joue (2 players robots par défaut)
void set_players(bool bot_vs_human, player *pl){
    if (bot_vs_human)
        pl->robot = false;
}

// savoir si on affiche tous les tours ou juste le dernier
bool all_round_display(bool bot_vs_human) {

    bool see_all = false;

    if (!(bot_vs_human))
        see_all = all_results();

    return see_all;
}

// FIN INITIALISATION DU JEU
// DEBUT EXECUTION DU JEU

// execution des fonction pour un tour d'un joueur humain
void play_human(game_tab tab, player *pl, bool *finishMorpion, bool bot_vs_human) {
    print_tab_V2(tab, X, Y);
    pos played_position = ask_pos();
    while(!valid_coord(played_position.posX, played_position.posY) || tab[played_position.posX][played_position.posY]->filled) {
        print_bar(2 * (X+2));
        printf("\n/!\\ Position invalide, veuillez recommencer /!\\\n");
        played_position = ask_pos();
    }
    poseTokenOnGameTab(tab, played_position, pl->team);
    calc_value(tab, pl, played_position);

    if (thefinishquintuplet(tab, played_position, pl->team)) {
        print_tab_V2(tab, X, Y);
        print_who_win(pl, bot_vs_human);
        *finishMorpion = true;
    }
}

// execution des fonction pour un tour d'un joueur robot
void play_bot(game_tab tab, player *pl, player *pl_min, bool *finishMorpion, bool bot_vs_human) {
    //pos played_position = bestPosition(tab, pl);
    pos played_position = minimax(tab, *pl, *pl_min);
    poseTokenOnGameTab(tab, played_position, pl->team);
    calc_value(tab, pl, played_position);

    if (thefinishquintuplet(tab, played_position, pl->team)) {
        print_tab_V2(tab, X, Y);
        print_who_win(pl, bot_vs_human);
        *finishMorpion = true;
    }
}

// selection de quel joueur joue et quel type de joueur
void play(game_tab tab, player *pl1, player *pl2, int tourJeu, bool bot_vs_human, bool *finishMorpion) {

    if (tourJeu%2 == 0) {
        if (!(pl1->robot))
            play_human(tab, pl1, finishMorpion, bot_vs_human);
        else
            play_bot(tab, pl1, pl2, finishMorpion, bot_vs_human);
    } else
        play_bot(tab, pl2, pl1, finishMorpion, bot_vs_human);
}

// fonction affichage du jeu si finish _display selectionné
void display(game_tab tab, bool finish_display) {
    if (finish_display) {
        print_tab_V2(tab, X, Y);
    }
}

// FIN EXECUTION DU JEU

// DEBUT ALGORITHME MINIMAX (PAS FINI)

game_tab copy_tab(game_tab tab){
    game_tab new_tab = create_game_tab(X,Y);
    initialize_Tab(new_tab,X,Y);
    for (int i=0;i<X;i++){
        for (int j=0;j<Y;j++){
            new_tab[i][j]->sign = tab[i][j]->sign;
            new_tab[i][j]->filled = tab[i][j]->filled;
            new_tab[i][j]->value_c = tab[i][j]->value_c;
            new_tab[i][j]->value_r = tab[i][j]->value_r;
        }
    }
    return new_tab;
}

int max(int a, int b){
    if (a>b)
        return a;
    else
        return b;
}

int min(int a, int b){
    if (a<b)
        return a;
    else
        return b;
}

token minimax_calc(game_tab tab, int depth, int alpha, int beta, player pl_max, player pl_min, bool is_max_player) {

    game_tab dummy_tab = copy_tab(tab);

    pos best_pos;
    token best_token;
    best_pos.posX = -1;
    best_pos.posY = -1;
    best_token.position = best_pos;
    best_token.value_c = -1;
    best_token.value_r = -1;

    char sign;
    if (pl_max.team == 'c')
        sign = 'c';
    else
        sign = 'r';

    if (depth == 0 || thefinishquintuplet(tab, best_pos, sign)) {
        return best_token;
    }

    if (is_max_player) {
        int value = -1000;
        for (int i = 0; i < X; i++) {
            for (int j = 0; j < Y; j++) {
                if (!dummy_tab[i][j]->filled) {
                    poseTokenOnGameTab(dummy_tab, dummy_tab[i][j]->position, pl_max.team);
                    calc_value(dummy_tab, &pl_max, dummy_tab[i][j]->position);
                    int new_value;
                    if (sign == 'c')
                        new_value = minimax_calc(dummy_tab, depth - 1, alpha, beta, pl_max, pl_min, false).value_c;
                    else
                        new_value = minimax_calc(dummy_tab, depth - 1, alpha, beta, pl_max, pl_min, false).value_r;
                    if (new_value > value) {
                        value = new_value;
                        best_pos = dummy_tab[i][j]->position;
                    }
                    alpha = max(alpha, value);
                    if (beta <= alpha)
                        break;
                }
            }
        }
        freeTab(dummy_tab, X, Y);
        best_token.position = best_pos;
        return best_token;
    } else {
        int value = 1000000000;
        for (int i = 0; i < X; i++) {
            for (int j = 0; j < Y; j++) {
                if (!dummy_tab[i][j]->filled) {
                    poseTokenOnGameTab(dummy_tab, dummy_tab[i][j]->position, pl_min.team);
                    calc_value(dummy_tab, &pl_min, dummy_tab[i][j]->position);
                    int new_value;
                    if (sign == 'c')
                        new_value = minimax_calc(dummy_tab, depth - 1, alpha, beta, pl_max, pl_min, true).value_c;
                    else
                        new_value = minimax_calc(dummy_tab, depth - 1, alpha, beta, pl_max, pl_min, true).value_r;
                    if (new_value < value) {
                        value = new_value;
                        best_pos = dummy_tab[i][j]->position;
                    }
                    beta = min(beta, value);
                    if (beta <= alpha)
                        break;
                }
            }
        }
        freeTab(dummy_tab, X, Y);
        best_token.position = best_pos;
        return best_token;
    }
}

pos minimax(game_tab tab, player pl_max, player pl_min) {

    token token_case = minimax_calc(tab, 3, -10000, 1000000, pl_max, pl_min, true);

    return token_case.position;
}

// FIN ALGORITHME MINIMAX