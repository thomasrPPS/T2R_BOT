#include <stdio.h>
#include <stdlib.h>
#include "TicketToRideAPI.h"
#include "move.h"
#include "game.h"
//BZH_VIE
#define BOTNAME "BZH_VIE"
#define NB_CITY_MAX "36"

/*!
* @mainpage How it works ?
* @author Thomas Rio
* @date JANV 2021
* @section STRATEGIE
* My bot place as much as he can of big tracks (5 or 6 of length)
* to gain points very fast and use a lot of wagons to limit the ennemi
* in doing his/her objectives.
*
* @section HOW HOW TO DO IT ?
* There is a main function which plays the move see bzhPlay.
* Then there is the Dijkstra Algorithm which allow me to list the tracks
* I need to take to succeed in doing my objectives.
* Also there is a function which check if a big track is available with
* the card I have, return a t_track with a city1 of -1 is not
*
* @section A TYPICAL GAME
* taking 2 objs, try to fill big tracks if he can.
* My strategie is too be the fastest as possible with a high score
* of claimed tracks to avoid to loose a lot on failed objectives
*/


/*!
* @brief to show some infos about my data into the terminal which are filled into
* the t_game G which contains everything
* @param t_game *G
*/
void displayObjs(t_game *G){
    printf("My objectives : \n");
    for (int i= 0; i < G->players[0].nbObjectives ; i++){
        printf("%d. %d (", i, G->players[0].objectives[i].city1);
        printCity(G->players[0].objectives[i].city1);
        printf(") -> (");
        printCity(G->players[0].objectives[i].city2);
        printf(") %d (%d pts)\n", G->players[0].objectives[i].city2, G->players[0].objectives[i].score);
    }
    printf("\n");
    for (int i=0; i<G->players[0].nbTracks2take; i++){
        printf("city1: %d city2: %d length: %d color1: %d color2: %d taken: %d \n",G->players[0].tracks2take[i].city1, G->players[0].tracks2take[i].city2, G->players[0].tracks2take[i].length,G->players[0].tracks2take[i].color1,G->players[0].tracks2take[i].color2,G->players[0].tracks2take[i].taken);
    }
    printf("\n");
    printf("Number of cards : ");
    printf("%d\n",G->players[0].nbCards);
    printf("\n");
    printf("My cards : \n");
    for (int i= 1; i < 10; i++){
        printf("%d ",G->players[0].cards[i]);
    }
    printf("\n \n");
}

/*!
* @brief connect to the server and play the moves
*/
int main() {
    //int prec[78] = {0};      /*array of city for Dijkstra*/

	/* connect to server */
	connectToServer("li1417-56.members.linode.com", 1234, BOTNAME);

	/* wait for a game and get the map */
    // NICE_BOT
    // PLAY_RANDOM
    // DO_NOTHING
    // TRAINING
    // TOURNAMENT
    // timeout=10

    while (1) {
        t_game G;
    	int* arrayTracks;		    /* array of int for the tracks */
    	t_color ourCards[4];	    /* our first cards */
    	t_return_code retCode;
    	int replay = 0;			    /* boolean, tells if the player replays */
    	t_move move;			    /* current move */
    	t_color lastMove = NONE;	/* last Card taken we need to replay else NONE */
        waitForT2RGame("TRAINING PLAY_RANDOM", G.name, &G.nbCities, &G.nbTracks);
    	arrayTracks = malloc(5*G.nbTracks*sizeof(int));
    	G.player = getMap(arrayTracks, G.faceUp, ourCards);
        initGame(&G,ourCards,arrayTracks);
    	do{

    		/* display map */
    		if (!replay){
                printMap();
                displayObjs(&G);
            }
    		/* the opponent plays ??*/
    		if (G.player == 1){
    			retCode = getMove(&move, &replay);
                updateHisData(&move, &G);
    		}
    		else {
    			/* we play */
    			//askMove(&move);
                bzhPlay(&move, &G, &lastMove);
                replay = needReplay(&move, lastMove);
    			retCode = playOurMove(&move, &lastMove);
                updateMyData(&move, &G);
    			// update game
    		}
    		/* change player */
    		if (retCode == NORMAL_MOVE && !replay){
    			G.player = !G.player;
            }
            // displayObjs(&bzh);
    	} while (retCode == NORMAL_MOVE);

    	/* check who wins */
    	if ((retCode == WINNING_MOVE && G.player == 0) || (retCode == LOOSING_MOVE && G.player == 1)){
            printf("lol bv\n");
        }
        else{
            printf("il été cho la wiss\n");
        }
    }

	closeConnection();
	return 0;
}
