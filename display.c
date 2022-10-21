#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "types.h"
#include "display.h"

// affiche une barre horizontale de la longueur de len
void print_bar(int len)
{
    for (int i = 0; i < len; i++)
        printf("%s", HORIZONTAL_BAR);
}

// DEBUT AFFICHAGE QUI JOUE

// gere l'affichage de la question "qui joue ?"
void style(bool selected)
{
    char *question = "selectionnez avec les touches (z + entrer) et (s + entrer) puis valider avec (espace + entrer)";
    int len = strlen(question);

    printf("%c", CORNER_LU);
    print_bar(len);
    printf("%c", CORNER_RU);
    printf("\n%c", VERTICAL);
    printf("%s", question);
    printf("%c\n", VERTICAL);
    printf("%c", VERTICAL_L);
    print_bar(len);
    printf("%c", VERTICAL_R);

    if (selected)
        printf("\n%s      bot vs human                                                                            %s\n%s    > bot vs bot                                                                              %s\n", VERTICAL, VERTICAL, VERTICAL, VERTICAL);
    else
        printf("\n%s    > bot vs human                                                                            %s\n%s      bot vs bot                                                                              %s\n", VERTICAL, VERTICAL, VERTICAL, VERTICAL);

    printf("%c", CORNER_LB);
    print_bar(len);
    printf("%c", CORNER_RB);
    printf("\n");
}

// question a l'utilisateur pour sab=voir si il veut jouer contre un bot ou voir une partie d'un bot contre un bot
bool who_play() {
    bool selected = false;
    char touch;

    style(selected);

    while(touch != 32) {
        touch = getchar();

        //system("clear"); //unix
        system("CLS"); //windows

        style(selected);

        switch(touch)
        {
            case 122 :
                selected = false;
                break;

            case 115 :
                selected = true;
                break;
        }
    }

    //system("clear"); //unix
    system("CLS"); //windows

    return selected;
}

// FIN AFFICHAGE QUI JOUE
// DEBUT AFFICHAGE TABLEAU

// retourne le signe a afficher dans le tableau
char print_char(token* Token)
{
    if ((Token->sign) == 'c')
        return 'X';
    else if ((Token->sign) == 'r')
        return 'O';
    else
        return '_';
}

// affiche le tableau
void print_tab_V1(token*** Token, int x, int y)
{
    system("CLS");

    for (int i = 0; i <= x; i++)
    {
        for (int j = 0; j <= y; j++)
        {
            if (j == 0 && i == 0) {
                printf("    ");
            } else if (j == 0 && i != 0) {
                printf("%4d", i - 1);
            } else if (i == 0 && j != 0) {
                printf("%4d", j - 1);
            } else if (i > 0 && j > 0) {
                char value = print_char(Token[i-1][j-1]);
                printf("%4c", value);
            }
        }
        printf("\n");

    }
}

void print_tab_V2(token*** Token, int x, int y)
{
    system("CLS");

    for (int i = 0; i <= (2 * x + 3); i++)
    {
        for (int j = 0; j <= (2 * y + 3); j++)
        {

            int i_print = (i / 2) - 2;
            int i_hun = i_print/100;
            int i_ten = (i_print - (i_hun * 100)) / 10;
            int i_unit = i_print % 10;

            int j_print = (j / 2) - 2;
            int j_hun = j_print/100;
            int j_ten = (j_print - (j_hun * 100)) / 10;
            int j_unit = j_print % 10;

            if ((j < 3 && (i < 4 || i % 2 == 1)) || (i < 3 && (j < 4 ||  j % 2 == 1))) {
                printf(" ");
            } else if (j == 0) {
                if (i_hun == 0)
                    printf(" ");
                else printf("%d", i_hun);
            } else if (j == 1) {
                if (i_ten == 0)
                    printf(" ");
                else printf("%d", i_ten);
            } else if (j == 2) {
                printf("%d", i_unit);
            } else if (i == 0) {
                if (j_hun == 0)
                    printf(" ");
                else printf("%d", j_hun);
            } else if (i == 1) {
                if (j_ten == 0)
                    printf(" ");
                else printf("%d", j_ten);
            } else if (i == 2) {
                printf("%d", j_unit);
            } else if (i == 3 && j == 3) {
                printf("%c", CORNER_LU);
            } else if (i == 3 && j == (2 * y + 3)) {
                printf("%c", CORNER_RU);
            } else if (i == (2 * x + 3) && j == 3) {
                printf("%c", CORNER_LB);
            } else if (i == (2 * x + 3) && j == (2 * y + 3)) {
                printf("%c", CORNER_RB);
            } else if (i > 3 && j > 3 && i % 2 == 1 && j % 2 == 1 && i < (2 * x + 3) && j < (2 * y + 3)) {
                printf("%c", CROSS);
            } else if (i > 2 && j > 2 && i % 2 == 1 && j % 2 == 0) {
                printf("%c", HORIZONTAL_BAR);
            } else if (i > 2 && j > 2 && i % 2 == 0 && j % 2 == 1) {
                printf("%c", VERTICAL);
            } else if (i == 3 && j > 2 && j % 2 == 1) {
                printf("%c", HORIZONTAL_U);
            } else if (i == (2 * x + 3) && j > 2 && j % 2 == 1) {
                printf("%c", HORIZONTAL_B);
            } else if (i > 2 && j == 3 && i % 2 == 1) {
                printf("%c", VERTICAL_L);
            } else if (i > 2 && j == (2 * y + 3) && i % 2 == 1) {
                printf("%c", VERTICAL_R);
            } else if (i > 3 && j > 3 && i % 2 == 0 && j % 2 == 0) {
                char value = print_char(Token[i_print][j_print]);
                printf("%c", value);
            }
        }
        printf("\n");
    }
}

void print_tab_r(token*** Token, int x, int y)
{
    for (int i = 0; i <= x; i++)
    {
        for (int j = 0; j <= y; j++)
        {
            if (j == 0 && i == 0) {
                printf("    ");
            } else if (j == 0 && i != 0) {
                printf("%4d", i - 1);
            } else if (i == 0 && j != 0) {
                printf("%4d", j - 1);
            } else if (i > 0 && j > 0) {
                printf("%4d", Token[i-1][j-1]->value_r);
            }
        }
        printf("\n");

    }
}

void print_tab_c(token*** Token, int x, int y)
{
    for (int i = 0; i <= x; i++)
    {
        for (int j = 0; j <= y; j++)
        {
            if (j == 0 && i == 0) {
                printf("    ");
            } else if (j == 0 && i != 0) {
                printf("%4d", i - 1);
            } else if (i == 0 && j != 0) {
                printf("%4d", j - 1);
            } else if (i > 0 && j > 0) {
                printf("%4d", Token[i-1][j-1]->value_r);
            }
        }
        printf("\n");

    }
}
// FIN AFFICHAGE TABLEAU

pos ask_pos()
{
    pos position;
    int x, y;

    print_bar(2 * X + 2);

    printf("\nEntrez la position de la case a jouer (x puis y) :\n");

    printf("x : ");
    scanf("%d", &x);
    printf("y : ");
    scanf("%d", &y);

    position.posX = x;
    position.posY = y;

    return position;
}

void print_botwinner(player *pl)
{
    char *msg;

    if (pl->team == 'c')
    {
        msg = "Le bot utilisant les croix a gagne la partie !";
    } else {
        msg = "Le bot utilisant les ronds a gagne la partie !";
    }

    printf("%c", CORNER_LU);
    print_bar(strlen(msg) + 2);
    printf("%c\n", CORNER_RU);
    printf("%c %s %c", VERTICAL, msg, VERTICAL);
    printf("%c", CORNER_LB);
    print_bar(strlen(msg) + 2);
    printf("%c\n", CORNER_RB);
}

void you_win()
{
    printf("??    ??  ??????  ??    ??     ??     ?? ?? ???    ??     ??\n");
    printf(" ??  ??  ??    ?? ??    ??     ??     ?? ?? ????   ??     ??\n");
    printf("  ????   ??    ?? ??    ??     ??  ?  ?? ?? ?? ??  ??     ??\n");
    printf("   ??    ??    ?? ??    ??     ?? ??? ?? ?? ??  ?? ??       \n");
    printf("   ??     ??????   ??????       ??? ???  ?? ??   ????     ?? ");
}

void you_lose()
{
    printf("??    ??  ??????  ??    ??     ??       ??????  ??????? ???????     ??\n");
    printf(" ??  ??  ??    ?? ??    ??     ??      ??    ?? ??      ??          ??\n");
    printf("  ????   ??    ?? ??    ??     ??      ??    ?? ??????? ?????       ??\n");
    printf("   ??    ??    ?? ??    ??     ??      ??    ??      ?? ??            \n");
    printf("   ??     ??????   ??????      ???????  ??????  ??????? ???????     ??\n");
}

// appeler la fonction avec pl1 le gagnant et pl2 le perdant
void print_who_win(player *pl1, player *pl2)
{
    if (pl1->robot && pl2->robot)
        print_botwinner(pl1);
    else {
        if (pl2->robot)
            you_win();
        else
            you_lose();
    }
}