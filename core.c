#include "core.h"
#include "display.h"

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

bool valid_coord(int x, int y){// verifie que des coordonner soit dans un tableau
    return ((x >= 0 && x < X)&&
            (y >= 0 && y < Y));
}

int value_code(int n, bool value_max){
    if (value_max)
        return (int)(5. * pow(10, (n/6)-1));
    else
        return (int)(5. * (pow(10, n-1) - pow(10, n-2)));
}

void eval_1_quintuplet(game_tab tab, pos position, int* code, player* pl_max) {
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

void eval_1_direction(int* code, int* code_max, int* code_min) {
    if (*code == 0) {
        *code_max += 1;
        *code_min += 1;
    } else if (*code <= 5) {
        *code_max += value_code(*code, false);
        *code_min += value_code((*code) * 6, true);
    } else if (*code % 6 == 0) {
        *code_max += value_code(*code, true);
        *code_min += value_code((*code) / 6, false);
    }
}

void points_verticale(game_tab tab, pos position, int* code_max, int* code_min, player* pl_max) {

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
                    eval_1_quintuplet(tab, new_pos, &code, pl_max);
                }
            }

            eval_1_direction(&code, code_max, code_min);

            tmp_y++;
        }
    }
}

void points_horizontal(game_tab tab, pos position, int* code_max, int* code_min, player* pl_max){

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
                    eval_1_quintuplet(tab, new_pos, &code, pl_max);
                }
            }

            eval_1_direction(&code, code_max, code_min);

            tmp_x++;
        }
    }
}


void points_bas_gauche(game_tab tab, pos position, int* code_max, int* code_min, player* pl_max){

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
                    eval_1_quintuplet(tab, new_pos, &code, pl_max);
                }
                i++;
            }

            eval_1_direction(&code, code_max, code_min);

            tmp_x++;
            tmp_y--;
        }
    }
}

void points_haut_gauche(game_tab tab, pos position, int* code_max, int* code_min, player* pl_max){

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
                    eval_1_quintuplet(tab, new_pos, &code, pl_max);
                }
                i++;
            }

            eval_1_direction(&code, code_max, code_min);

            tmp_x++;
            tmp_y++;
        }
    }
}


void eval_position(game_tab tab, pos position, player* pl_max) {

    int code_max = 0;
    int code_min = 0;

    points_verticale(tab, position, &code_max, &code_min, pl_max);
    points_bas_gauche(tab, position, &code_max, &code_min, pl_max);
    points_haut_gauche(tab, position, &code_max, &code_min, pl_max);
    points_horizontal(tab, position, &code_max, &code_min, pl_max);


    if (pl_max->team == 'c') {
        tab[position.posX][position.posY]->value_c = code_max;
        tab[position.posX][position.posY]->value_r = code_min;
    } else {
        tab[position.posX][position.posY]->value_c = code_min;
        tab[position.posX][position.posY]->value_r = code_max;
    }
}

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

player* init_player(char team, bool robot){
    player* pl = (player*)malloc(sizeof(player));
    pl->team = team;
    pl->robot = robot;

    return pl;
}

token*** create_game_tab(int x,int y){
    token ***res = (token***)malloc(x*sizeof(token**));
    for (int i=0;i<x;i++){
        res[i] =(token**)malloc(sizeof(token*)*y);
    }

    return res;
}

void initialize_Tab(token*** TplateauJeu,int x,int y){
    for (int i=0;i<x;i++){
        for (int j=0;j<y;j++){
            (TplateauJeu[i][j]) = initToken(i,j);
        }
    }
}

void freeTab(token*** tab, int x,int y){
    for(int i=0;i<x;i++){
        free(*(tab[i]));
    }
    free(*tab);
}

token* initToken(int x,int y){
    token* box = (token*)malloc(sizeof(token));
    box->sign='_';
    box->position.posX=x,box->position.posY=y;
    box->filled=false;
    box->value_c=0;
    box->value_r=0;

    return box;
}

void poseTokenOnGameTab(game_tab tab, pos theposition, char sign)
{
    tab[theposition.posX][theposition.posY]->sign=sign;
    tab[theposition.posX][theposition.posY]->filled=true;
}

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


bool thefinishquintuplet(game_tab tab, pos thePosition, char sign)
{
    return (finishquintupletvertical(tab,thePosition,sign)|| finishquintuplethorizontal(tab,thePosition,sign) || finishquintupletBasGauche(tab,thePosition,sign) || finishquintupletHautGauche(tab,thePosition,sign) );
}

void tab_best_position(token new_token, char team_max, pos* best_positions, int* best_value, int *element_nb)
{
    if(team_max == 'c') {
        if(new_token.value_c > *best_value) {
            *best_value = new_token.value_c;
            //free(best_positions);
            //best_positions = NULL;
            //best_positions = (pos*)malloc(sizeof(pos));
            best_positions[0] = new_token.position;
            *element_nb = 1;
        } else if(new_token.value_c == *best_value) {
            //best_positions = (pos*)realloc(best_positions, ((*element_nb) + 1) * sizeof(pos));
            best_positions[*element_nb] = new_token.position;
            (*element_nb)+=1;
        } else {
            return;
        }
    } else {
        if(new_token.value_r > *best_value) {
            *best_value = new_token.value_r;
            //free(best_positions);
            //best_positions = NULL;
            //best_positions = (pos*)malloc(sizeof(pos));
            best_positions[0] = new_token.position;
            *element_nb = 1;
        } else if(new_token.value_r == *best_value) {
            //best_positions = (pos*)realloc(best_positions, ((*element_nb) + 1) * sizeof(pos));
            best_positions[*element_nb] = new_token.position;
            (*element_nb)+=1;
        } else {
            return;
        }
    }
}

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

void set_players(bool bot_vs_human, player *pl1, player *pl2){
    if (bot_vs_human)
        pl1->robot = false;
}

bool all_round_display(bool bot_vs_human) {

    bool see_all = false;

    if (!(bot_vs_human))
        see_all = all_results();

    return see_all;
}

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

void play_bot(game_tab tab, player *pl, bool *finishMorpion, bool bot_vs_human) {
    pos played_position = bestPosition(tab, pl);
    poseTokenOnGameTab(tab, played_position, pl->team);
    calc_value(tab, pl, played_position);

    if (thefinishquintuplet(tab, played_position, pl->team)) {
        print_tab_V2(tab, X, Y);
        print_who_win(pl, bot_vs_human);
        *finishMorpion = true;
    }
}

void play(game_tab tab, player *pl1, player *pl2, int tourJeu, bool bot_vs_human, bool *finishMorpion) {

    if (tourJeu%2 == 0) {
        if (!(pl1->robot))
            play_human(tab, pl1, finishMorpion, bot_vs_human);
        else
            play_bot(tab, pl1, finishMorpion, bot_vs_human);
    } else
        play_bot(tab, pl2, finishMorpion, bot_vs_human);
}

void display(game_tab tab, bool finish_display) {
    if (finish_display) {
        print_tab_V2(tab, X, Y);
    }
}