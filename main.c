#include "display.h"
#include "core.h"
#include <time.h>


int main() {

    system("clear"); //unix
    //system("CLS"); //windows

    srand(time(NULL));

    bool is_finish = false;
    bool *finishMorpion = &is_finish;
    int tourJeu = 0;

    token*** tab = create_game_tab(X,Y);
    initialize_Tab(tab,X,Y);

    bool bot_vs_human = who_play();

    player *pl1 = init_player('c', true, false);
    player *pl2 = init_player('r', true, false);

    set_players(bot_vs_human, pl1);

    bool finish_display = all_round_display(bot_vs_human);

    init_value(tab, pl1); //initialisation des valeurs du tableau

    while(!(*finishMorpion)) {
        display(tab, finish_display);
        play(tab, pl1, pl2, tourJeu, bot_vs_human, finishMorpion);

        tourJeu++;
    }

    freeTab(tab, X, Y);

    return EXIT_SUCCESS;
}