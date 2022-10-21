#include <unistd.h>

#include "display.h"
#include "core.h"

//X 20 Y 30

int main() {

    bool finishMorpion = false;
    int tourJeu = 0;
    pos position_played;

    token*** tab = create_game_tab(X,Y);
    initialize_Tab(tab,X,Y);

    player* pl1 = init_player('c',true);
    player* pl2 = init_player('r',true);

    init_value(tab, pl1); //initialisation des valeurs du tableau


    while(!finishMorpion)
    {
        if (tourJeu % 2 == 0){
            //print_tab_c(tab,X,Y);
            position_played = bestPosition(tab, pl1);
            //printf("best pos : %d %d\n",position_played.posX,position_played.posY);

            poseTokenOnGameTab(tab, position_played, pl1->team);
            calc_value(tab, pl1, position_played);
            //init_tab(tab, pl1);
            //print_tab_c(tab, X, Y);

            finishMorpion = thefinishquintuplet(tab, position_played,pl1->team);

        } else if (tourJeu % 2 == 1){
            //print_tab_r(tab,X,Y);
            position_played = bestPosition(tab, pl2);
            //printf("best pos : %d %d\n",position_played.posX,position_played.posY);
            poseTokenOnGameTab(tab, position_played, pl2->team);
            calc_value(tab, pl2, position_played);
            //init_tab(tab, pl2); //initialisation des valeurs du tableau
            //print_tab_r(tab, X, Y);

            finishMorpion = thefinishquintuplet(tab, position_played, pl2->team);

        }

        tourJeu++;

        system("CLS");

        printf("Tour °%d\n", tourJeu);
        printf("Position jouée : %d %d\n", position_played.posX, position_played.posY);
        print_tab_V2(tab, X, Y);
        sleep(3);
    }
/*
    token*** tab = create_game_tab(X,Y);
    initialize_Tab(tab,X,Y);

    player* pl1 = init_player('c',true);
    player* pl2 = init_player('r',true);

    init_tab(tab, pl1);

    //print_tab_V2(tab, X, Y);

    // print_tab_c(tab, X, Y);

    //sleep(10);

    pos position_played = {9,9};

    poseTokenOnGameTab(tab, position_played, pl2->team);

    init_tab(tab, pl2);

    print_tab_c(tab, X, Y);

    sleep(10);*/

    return 0;
}
