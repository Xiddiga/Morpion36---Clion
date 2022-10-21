#include "core.h"

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


//By Harry


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

    int tmp_y = position.posY - 4;
    int x = position.posX;

    while(tmp_y <= position.posY) {
        if (!(valid_coord(x, tmp_y))) {
            tmp_y++;
        } else {
            int code = 0;

            for (int j = tmp_y; j < tmp_y + 5; j++) {
                if (!(valid_coord(x, j)))
                    code += 13;
                else {
                    position.posY = j;
                    eval_1_quintuplet(tab, position, &code, pl_max);
                }
            }

            eval_1_direction(&code, code_max, code_min);

            tmp_y++;
        }
    }
}

void points_horizontal(game_tab tab, pos position, int* code_max, int* code_min, player* pl_max){

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
                    position.posX = i;
                    eval_1_quintuplet(tab, position, &code, pl_max);
                }
            }

            eval_1_direction(&code, code_max, code_min);

            tmp_x++;
        }
    }
}


void points_bas_gauche(game_tab tab, pos position, int* code_max, int* code_min, player* pl_max){

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
                    position.posX = i;
                    position.posY = j;
                    eval_1_quintuplet(tab, position, &code, pl_max);
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
                    position.posX = i;
                    position.posY = j;
                    eval_1_quintuplet(tab, position, &code, pl_max);
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
        if (valid_coord(position.posX + i, position.posY)) {
            pos new_position;
            new_position.posX=position.posX + i, new_position.posY=position.posY;
            eval_position(tab, new_position, pl_max);
        }
    }

    for (int i = y-4; i <= y+4; i++) {
        if (valid_coord(position.posX, position.posY + i)) {
            pos new_position;
            new_position.posX = position.posX,new_position.posY=position.posY + i;
            eval_position(tab, new_position, pl_max);
        }
    }

    int j = y-4;
    for (int i = x-4; i <= x+4; i++) {
        if (valid_coord(position.posX + i, position.posY + j)) {
            pos new_position;
            new_position.posX=position.posX + i, new_position.posY=position.posY+j;
            eval_position(tab, new_position, pl_max);
        }
        j++;
    }

    j = y+4;
    for (int i = x-4; i <= x+4; i++) {
        if (valid_coord(position.posX + i, position.posY + j)) {
            pos new_position;
            new_position.posX=position.posX + i, new_position.posY=position.posY+j;
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

//By Bill

/*
void create_game_tab(token*** TplateauJeu,int x,int y){
    (*TplateauJeu) = (token**)malloc(sizeof(token*)*x);
    for (int i=0;i<x;i++){
        (*TplateauJeu)[i] =(token)malloc(sizeof(token)*y);
    }
}
*/



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

/*
void place_token(char sign, token*** game_tab){//poser le token sur le tableau de jeu
    pos thePosition = best_value(sign, game_tab,X,Y);
    int x =thePosition.posX , y=thePosition.posY;
    game_tab[x][y]=fillToken(thePosition,sign);
}
*/

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

pos bestPosition(game_tab tab, player* pl_max)
{
    int bestValue = 0;
    pos thePosition;

    char team_max = pl_max->team;

    for(int i=0;i<X;i++)
    {
        for(int j=0;j<Y;j++)
        {
            if (tab[i][j]->filled==false)
            {
                if (team_max == 'r') {
                    if (tab[i][j]->value_r > bestValue)
                    {
                        bestValue = tab[i][j]->value_r;
                        thePosition.posX = i;
                        thePosition.posY = j;
                    }
                } else if (team_max == 'c') {
                    if (tab[i][j]->value_c > bestValue)
                    {
                        bestValue = tab[i][j]->value_c;
                        thePosition.posX = i;
                        thePosition.posY = j;
                    }
                }
            }
        }
    }
    return thePosition;
}
