#ifndef T2R_TEST_MOVE_H
#define T2R_TEST_MOVE_H

#include "game.h"
#include "TicketToRideAPI.h"

int distancemini(int D[78],int Visite[78],t_game *G);

int dijsktra(int city,int dest,t_game *G);

//void chemin2dest(int city, int dest,t_game *G);

void chemin2tracks(t_game *G,t_objective obj);

t_track canPlaceTrack(t_game *G);

t_track randPlace(t_game *G);

void triTabLength(t_game *G);

void move4tracks(t_game *G,t_track *track, t_move *move, int nbLoco, t_color color);

void bzhPlay(t_move* move, t_game *G, t_color* lastCard);

void initGame(t_game *G,t_color ourCards[4],int* arrayTracks);

void addCard(t_player *bzh, t_color card, int add);

void updateMyData(t_move* move,t_game *G);

void updateHisData(t_move* move,t_game *G);

/* ask for a move */
void askMove(t_move* move);

/* plays the move given as a parameter (send to the server)
 * returns the return code */
t_return_code playOurMove(t_move* move, t_color* lastCard);

/* tell if we need to replay */
int needReplay(t_move* move, t_color lastCard);

#endif //T2R_TEST_MOVE_H
