#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "game.h"
#include "move.h"
#define N 78
#define INFF 99

/*!
* @brief add or remove a card from the data stored into a t_player struct
* we use it in the updateMyData or updateHisData fucntions
* @param t_player *bzh
* @param t_color card
* @param int add
*/
void addCard(t_player *bzh, t_color card, int add){
	for (int i=1; i<10; i++){
		if (card == i && add == 1){
			bzh->cards[i]+=1;
			bzh->nbCards +=1;
		}
		if (card == i && add == 0){
			bzh->cards[i]-=1;
			bzh->nbCards -=1;
		}
	}
}

/*!
* @brief little fucntion to find the smallest element in an array
* @param int D[]
* @param int Visite[]
* @param t_game *G
*/
int distancemini(int D[],int Visite[],t_game *G ){
    int min,indice;
    // Initialisation
    min =INFF;
    // Recherche de la ville non visite ́e la plus proche
    for(int i=0; i< G->nbCities; i++){
        if (Visite[i]==0 && D[i]< min){
            min = D[i];
            indice = i;
        }
    }
    return indice;
}

/*!
* @brief Dijsktra Algorithm allows us to find the fastest way to success an objective
* by finding all the towns you need to reach
* @param int city
* @param int dest
* @param t_game *G
*/
int dijsktra(int city,int dest,t_game *G){
    int visite[G->nbCities];
    //int chemin[100]={0};
    int u=city, length;
    int k=dest;
    int j=0;
	int stop=0;

    for (int i=0; i<G->nbCities; i++){
        G->D[i]=INFF;
        visite[i]= 0;
    }
    G->D[u]=0;

    while (u!= dest) {
        u=distancemini(G->D,visite,G);
		//printf("u: %d visite[u]: %d /",u,visite[u] );
		while (visite[u]==1) {
			if (stop == 100){
				return 0;
			}
			u=distancemini(G->D,visite,G);
			stop ++;
		}
        visite[u]=1;
		//printf("*");
        for (int v=0; v< G->nbCities; v++){
            if(G->tabTracks2D[u][v].length <= INFF){
                if(G->tabTracks2D[u][v].me == 1){
                    length =0;
                }
				else if (G->tabTracks2D[u][v].taken == 1 && G->tabTracks2D[u][v].me == 0){
					length =INFF;
				}
                else{
                    length=G->tabTracks2D[u][v].length;
                }
            }
            if (visite[v]==0 && G->tabTracks2D[u][v].length <= INFF && G->tabTracks2D[u][v].taken != 1 && (G->D[u] + length)< G->D[v]){
                G->D[v]=G->D[u] + length;
                G->prec[v]=u;
            }
        }
    }

    do{
        G->chemin[j]=k;
        k=G->prec[k];
        j++;
    }while(k != city);

    for (int i =0; i<6; i++){
        //printf("%d ",G->chemin[i]);
    }
    //printf("\n");
	return 1;
}

/*!
* @brief it is linked to the dijsktra algo and allows us to transform the array of
* cities we have with the algorithm into some t_track we can fill into an array of
* @param t_game *G
* @param t_objective obj
*/
void chemin2tracks(t_game *G,t_objective obj){
    int k=0;
    int bol=0;
    while (G->chemin[k+1] < 40){
        bol=1;
        for (int i=0; i<100; i++){
            if ((G->players[0].tracks2take[i].city1 == G->chemin[k] && G->players[0].tracks2take[i].city2 == G->chemin[k+1]) || (G->players[0].tracks2take[i].city2 == G->chemin[k] && G->players[0].tracks2take[i].city1 == G->chemin[k+1])){
                bol =0;
            }
        }
        if (bol == 1){
            G->players[0].tracks2take[G->players[0].nbTracks2take].city1 = G->chemin[k];
            G->players[0].tracks2take[G->players[0].nbTracks2take].city2 = G->chemin[k+1];
            G->players[0].tracks2take[G->players[0].nbTracks2take].length = G->tabTracks2D[G->chemin[k]][G->chemin[k+1]].length;
            G->players[0].tracks2take[G->players[0].nbTracks2take].color1 = G->tabTracks2D[G->chemin[k]][G->chemin[k+1]].color1;
            G->players[0].tracks2take[G->players[0].nbTracks2take].color2 = G->tabTracks2D[G->chemin[k]][G->chemin[k+1]].color2;
            G->players[0].nbTracks2take+=1;
        }
        k++;
		int oui=0;
		if (G->chemin[k+1] > 40){
			for (int i=0; i< G->players[0].nbTracks2take; i++){
				if (G->players[0].tracks2take[i].city1 == G->chemin[k] && G->players[0].tracks2take[i].city2 == obj.city1){
					oui =1;
				}
			}
			if (oui == 0){
				G->players[0].tracks2take[G->players[0].nbTracks2take].city1 = G->chemin[k];
				G->players[0].tracks2take[G->players[0].nbTracks2take].city2 = obj.city1;
				G->players[0].tracks2take[G->players[0].nbTracks2take].length = G->tabTracks2D[G->chemin[k]][obj.city1].length;
				G->players[0].tracks2take[G->players[0].nbTracks2take].color1 = G->tabTracks2D[G->chemin[k]][obj.city1].color1;
				G->players[0].tracks2take[G->players[0].nbTracks2take].color2 = G->tabTracks2D[G->chemin[k]][obj.city1].color2;
				G->players[0].nbTracks2take+=1;
			}
		}
    }
	//printf("\n\n obj city 1 %d \n\n",obj.city1);
	//printf("\n\n obj k+1 1 %d \n\n",G->chemin[k]);

    for (int i=0; i<=100; i++){
         G->chemin[i]=69;
     }
}

/*!
* @brief return a filled t_track if we can place a track in our tracks2take array
* otherwise it return a t_track with city1 = -1
* it looks for colors, length, number of wagons ect
* maybe the hardest function to code because of all the conditions
* @param t_game *G
*/
t_track canPlaceTrack(t_game *G){
    int multi = G->players[0].cards[9],color,colormul;
    t_track retour;
	int speed = 5;
	if (G->players[0].nbWagons < 5){
		speed = 0;
	}
    for (int i=0; i<G->players[0].nbTracks2take; i++){
        if (G->tabTracks2D[G->players[0].tracks2take[i].city2][G->players[0].tracks2take[i].city1].taken == 1 || G->tabTracks2D[G->players[0].tracks2take[i].city1][G->players[0].tracks2take[i].city2].taken == 1 || (G->tabTracks2D[G->players[0].tracks2take[i].city1][G->players[0].tracks2take[i].city2].length >= G->players[0].nbWagons)){
            continue;
        }
        if (G->players[0].tracks2take[i].length <= (G->players[0].cards[G->players[0].tracks2take[i].color1] + multi) && G->players[0].tracks2take[i].color1 != 9){
            retour.city1=G->players[0].tracks2take[i].city1;
            retour.city2=G->players[0].tracks2take[i].city2;
            retour.length=G->players[0].tracks2take[i].length;
            colormul = G->players[0].tracks2take[i].length - G->players[0].cards[G->players[0].tracks2take[i].color1];
            if (colormul <0){
                colormul =0;
            }
            retour.color1 = G->players[0].tracks2take[i].color1;
            retour.multi = colormul;
			G->players[0].tracks2take[i].taken = 1;
            return retour;
        }
        else if (G->players[0].tracks2take[i].length <= (G->players[0].cards[G->players[0].tracks2take[i].color2] + multi) && G->players[0].tracks2take[i].color1 != 9 && G->players[0].tracks2take[i].color2 != 0 && G->players[0].tracks2take[i].color2 != 9){
            retour.city1=G->players[0].tracks2take[i].city1;
            retour.city2=G->players[0].tracks2take[i].city2;
            retour.length=G->players[0].tracks2take[i].length;
            colormul = G->players[0].tracks2take[i].length - G->players[0].cards[G->players[0].tracks2take[i].color2];
            if (colormul <0){
                colormul =0;
            }
            retour.color1 = G->players[0].tracks2take[i].color2;
            retour.multi = colormul;
			G->players[0].tracks2take[i].taken = 1;
            return retour;
        }
        for (int j=1; j<10; j++){
            if (G->players[0].tracks2take[i].color1 == 9 || G->players[0].tracks2take[i].color2 == 9){
                if (G->players[0].tracks2take[i].length <= G->players[0].cards[j] + multi){
                    retour.city1=G->players[0].tracks2take[i].city1;
                    retour.city2=G->players[0].tracks2take[i].city2;
                    retour.length=G->players[0].tracks2take[i].length;
                    color=j;
                    colormul=G->players[0].tracks2take[i].length - G->players[0].cards[j];
                    if (colormul <0){
                        colormul=0;
                    }
                    retour.multi = colormul;
                    retour.color1=color;
                    retour.color2=color;
					G->players[0].tracks2take[i].taken = 1;
                    return retour;
                }
            }
        }
    }
    retour.city1 = -1;
    return retour;
}

/*!
* @brief return a filled t_track if we can place a big track
* otherwise it return a t_track with city1 = -1
* it looks for colors, length, number of wagons ect
* @param t_game *G
*/
t_track randPlace(t_game *G){
	int multi = G->players[0].cards[9],color,colormul;
    t_track retour;
	int speed = 5;
	if (G->players[0].nbWagons < 5){
		speed = 0;
	}
    for (int i=0; i<G->nbCities; i++){
		for (int j=0; j<G->nbCities; j++){
	        if (G->tabTracks2D[i][j].taken == 1 || G->tabTracks2D[j][i].taken == 1 || i == j || (G->tabTracks2D[i][j].length >= G->players[0].nbWagons)){
	            continue;
	        }
	        if (G->tabTracks2D[i][j].length <= (G->players[0].cards[G->tabTracks2D[i][j].color1] + multi) && G->tabTracks2D[i][j].color1 != 9 && G->tabTracks2D[i][j].length >=speed){
	            retour.city1=i;
	            retour.city2=j;
	            retour.length=G->tabTracks2D[i][j].length;
	            colormul = G->tabTracks2D[i][j].length - G->players[0].cards[G->tabTracks2D[i][j].color1];
	            if (colormul <0){
	                colormul =0;
	            }
	            retour.color1 = G->tabTracks2D[i][j].color1;
	            retour.multi = colormul;
				//printf("1\n" );
	            return retour;
	        }
	        else if (G->tabTracks2D[i][j].length <= (G->players[0].cards[G->tabTracks2D[i][j].color2] + multi) && G->tabTracks2D[i][j].color1 != 9 && G->tabTracks2D[i][j].color2 != 0 && G->tabTracks2D[i][j].color2 != 9 && G->tabTracks2D[i][j].length >=speed){
	            retour.city1=i;
	            retour.city2=j;
	            retour.length=G->tabTracks2D[i][j].length;
	            colormul = G->tabTracks2D[i][j].length - G->players[0].cards[G->tabTracks2D[i][j].color2];
	            if (colormul <0){
	                colormul =0;
	            }
	            retour.color1 = G->tabTracks2D[i][j].color2;
	            retour.multi = colormul;
				//printf("2\n" );
	            return retour;
	        }
	        for (int l=1; l<10; l++){
				if (i==j || G->tabTracks2D[i][j].length > 100){
					continue;
				}
	            if ((G->tabTracks2D[i][j].color1 == 9 || G->tabTracks2D[i][j].color2 == 9) && G->tabTracks2D[i][j].length >=speed){
	                if (G->tabTracks2D[i][j].length <= G->players[0].cards[l] + multi){
	                    retour.city1=i;
	                    retour.city2=j;
	                    retour.length=G->tabTracks2D[i][j].length;
	                    color=l;
	                    colormul=G->tabTracks2D[i][j].length - G->players[0].cards[l];
	                    if (colormul <0){
	                        colormul=0;
	                    }
	                    retour.multi = colormul;
	                    retour.color1=color;
						//printf("3\n" );
	                    return retour;
	                }
	            }
	            else if (G->tabTracks2D[i][j].color2 == 9  && G->tabTracks2D[i][j].length >=speed){
	                if ((G->tabTracks2D[i][j].length <= G->players[0].cards[l] + multi)){
	                    retour.city1=i;
	                    retour.city2=j;
	                    retour.length=G->tabTracks2D[i][j].length;
	                    color=l;
	                    colormul=G->tabTracks2D[i][j].length - G->players[0].cards[l];
	                    if (colormul <0){
	                        colormul=0;
	                    }
	                    retour.multi = colormul;
	                    retour.color1=color;
						//printf("4\n" );
	                    return retour;
	                }
	            }
	        }
		}
    }
    retour.city1 = -1;
    return retour;
}

/*!
* @brief just a simple function to easily sort the tracks2take by their length
* @param t_game *G
*/
void triTabLength(t_game *G){
	t_track tmp;
	int i, j;

	for (i=0 ; i <= G->players[0].nbTracks2take; i++)
	{
		for (j=0 ; j <= G->players[0].nbTracks2take-i; j++)
		{
			/* Pour un ordre décroissant utiliser < */
			if (G->players[0].tracks2take[j].length < G->players[0].tracks2take[j+1].length)
			{
				tmp = G->players[0].tracks2take[j];
				G->players[0].tracks2take[j] = G->players[0].tracks2take[j+1];
				G->players[0].tracks2take[j+1] = tmp;
			}
		}
	}
}

/*!
* @brief the main brain of the strategie
* it returns a t_move filled of the data from the move we want to play
* it tells when to draw a card, take objectives, or to claim a track
* @param t_move *move
* @param t_game *G
* @param t_color *lastCard
*/
void bzhPlay(t_move* move, t_game *G, t_color* lastCard){
    int objSeq[2]={4,5};
	int dijsktra =0;
    t_track aplacer=canPlaceTrack(G);
	t_track random=randPlace(G);
	//t_track random;
	if (((G->players[0].nbWagons > 10 && dijsktra==12) || (!(G->players[0].nbWagons > 8) && dijsktra!=10)) || (G->players[0].nbWagons > 25)){
		aplacer.city1 = -1;
	}
    //printf("aplacer city1 %d city2 %d color %d multi %d\n",aplacer.city1,aplacer.city2,aplacer.color1,aplacer.multi );
	//printf("random city1 %d city2 %d color %d multi %d\n",random.city1,random.city2,random.color1,random.multi );

	/*
    if (G->indice > 5 && G->players[0].nbTracks2take < 3 ){
        G->indice = 0;
        tour = 1;
    }
	*/
    if (*lastCard != NONE){
        for (int i =0; i< G->players[0].nbTracks2take; i++){
            for (int j = 0; j<5; j++){
                if (G->faceUp[j] == G->players[0].tracks2take[i].color1 || G->faceUp[j] == G->players[0].tracks2take[i].color2){
                    move->type = 3;
                    while (G->faceUp[j] ==9) {
                        j = rand() %5;
                    }
                    move->drawCard.card = G->faceUp[j];
                    break;
                }
            }
        }
    }
    else if (G->indice <2){
        move->type = objSeq[G->indice];
    }

    else if (aplacer.city1 !=-1 && G->players[0].nbWagons >=2){
        move->type = 1;
		G->lastTrack = aplacer;
		move->claimRoute.city1=aplacer.city1;
        move->claimRoute.city2=aplacer.city2;
        move->claimRoute.color=aplacer.color1;
        move->claimRoute.nbLocomotives=aplacer.multi;
		dijsktra =0;
    }

	else if (((random.city1 !=-1 && G->lastTrack.length > 3 ) || (random.city1 !=-1 && G->players[1].nbWagons < G->players[1].nbWagons+3)) && G->players[0].nbWagons >=2){
        move->type = 1;
		G->lastTrack = random;
		move->claimRoute.city1=random.city1;
        move->claimRoute.city2=random.city2;
        move->claimRoute.color=random.color1;
        move->claimRoute.nbLocomotives=random.multi;
		dijsktra +=1;
	}
    else {
        for (int i =0; i< G->players[0].nbTracks2take; i++){
            for (int j = 0; j<5; j++){
                if (G->faceUp[j]==9){
                    move->type = 3;
                    move->drawCard.card = G->faceUp[j];
                    break;
                }
                else if (G->faceUp[j] == G->players[0].tracks2take[i].color1 || G->faceUp[j] == G->players[0].tracks2take[i].color2){
                    move->type = 3;
                    move->drawCard.card = G->faceUp[j];
                    break;
                }
            }
        }
        move->type = 2;
    }
	if (move->type == CHOOSE_OBJECTIVES){
		int un=0,deux=0,trois=0;
		//(A || B) && !(A && B)
		/*
		if (G->tamp[0].score <= G->tamp[2].score){
			un = 1;
			if (G->tamp[0].score <= G->tamp[1].score){
				deux = 1;
			}
		}
		if (G->tamp[1].score <= G->tamp[0].score){
			deux =1;
			if (G->tamp[1].score <= G->tamp[2].score){
				deux = 1;
			}
		}
		if (G->tamp[2].score <= G->tamp[1].score){
			trois = 1;
			if (G->tamp[2].score <= G->tamp[0].score){
				un = 1;
			}
		}
		move->chooseObjectives.chosen[0] = un;
		move->chooseObjectives.chosen[1] = deux;
		move->chooseObjectives.chosen[2] = trois;
		*/

		if (G->tamp[0].score < G->tamp[1].score){
			move->chooseObjectives.chosen[0] = 1;
			move->chooseObjectives.chosen[1] = 0;
			move->chooseObjectives.chosen[2] = 1;
		}
		else {
			move->chooseObjectives.chosen[0] = 1;
			move->chooseObjectives.chosen[1] = 1;
			move->chooseObjectives.chosen[2] = 0;
		}
		printf("score 1 : %d score 2 : %d score 3 : %d",G->tamp[0].score,G->tamp[1].score,G->tamp[2].score);
		printf("un : %d deux : %d trois : %d \n", un,deux, trois);

	}
    G->indice ++;
}

/*!
* @brief Initialisation of the game, fill all the fields we use with some default values
* to avoid seg fault and to gain some time for the following actions,
* it puts all the data into the t_game G
* @param t_game *G
* @param t_color ourCards[4]
* @param int *arrayTracks
*/
void initGame(t_game *G,t_color ourCards[4],int* arrayTracks){
    for (int i=0; i<=100; i++){
        G->chemin[i]=69;
        G->players[0].tracks2take[i].length = -1;
        G->players[0].tracks2take[i].city1 = -1;
        G->players[0].tracks2take[i].city2 = -1;
        G->players[0].tracks2take[i].color1 = -1;
        G->players[0].tracks2take[i].color2 = -1;
        G->players[0].tracks2take[i].taken = 0;
        G->players[0].tracks2take[i].me = 0;
    }
	t_track tracks[G->nbTracks];
	//cards & players init
	for (int k =0; k <= 10; k++){
		G->players[0].cards[k]=0;
	}
	G->players[0].nbObjectives=0;
    G->players[0].nbTracks2take=0;
	G->players[1].nbObjectives=0;
	G->players[0].nbWagons=45;
	G->players[1].nbWagons=45;
	G->players[0].nbCards=4;
	G->players[1].nbCards=4;
	G->indice=0;
	for (int i=0; i<36; i++){
		G->prec[i]=0;
	}
    G->players[0].cards[0]=-1;
	for (int i =0; i<4; i++){
		for (int j=1; j< 10; j++){
			if (ourCards[i] == j){
				G->players[0].cards[j] += 1;
			}
		}
	}
	//map init

	for (int i=0; i<G->nbTracks; i++){
		tracks[i].city1 = *arrayTracks++;
		tracks[i].city2 = *arrayTracks++;				/* id of the cities */
		tracks[i].length = *arrayTracks++;			/* length of the track */
		tracks[i].color1 = *arrayTracks++;
		tracks[i].color2 = *arrayTracks++;			/* colors */
		tracks[i].taken = 0;
	}

	for (int i =0; i<G->nbCities; i++){
		for (int j=0; j<= G->nbCities; j++){
			G->tabTracks2D[i][j].city1=-1;
			G->tabTracks2D[i][j].city2=-1;
			G->tabTracks2D[i][j].length =999;
			G->tabTracks2D[i][j].color1 =0;
			G->tabTracks2D[i][j].color2 =0;
			G->tabTracks2D[i][j].taken =0;
			G->tabTracks2D[i][j].me =0;
		}
	}
	for (int i=0; i< 78; i++){
		G->tabTracks2D[tracks[i].city1][tracks[i].city2].length = tracks[i].length;
		G->tabTracks2D[tracks[i].city1][tracks[i].city2].color1 = tracks[i].color1;
		G->tabTracks2D[tracks[i].city1][tracks[i].city2].color2 = tracks[i].color2;
		G->tabTracks2D[tracks[i].city1][tracks[i].city2].taken =0;
        G->tabTracks2D[tracks[i].city2][tracks[i].city1].me =0;
        G->tabTracks2D[tracks[i].city2][tracks[i].city1].length = tracks[i].length;
		G->tabTracks2D[tracks[i].city2][tracks[i].city1].color1 = tracks[i].color1;
		G->tabTracks2D[tracks[i].city2][tracks[i].city1].color2 = tracks[i].color2;
		G->tabTracks2D[tracks[i].city2][tracks[i].city1].taken =0;
        G->tabTracks2D[tracks[i].city2][tracks[i].city1].me =0;
	}
	/*
    G->players[0].tracks2take[0].city1 = 17;
    G->players[0].tracks2take[0].city2 = 18;
    G->players[0].tracks2take[0].length = 1;
    G->players[0].tracks2take[0].color1 = 9;
    G->players[0].tracks2take[0].color2 = 0;
    G->players[0].tracks2take[1].city1 = 0;
    G->players[0].tracks2take[1].city2 = 1;
    G->players[0].tracks2take[1].length = 1;
    G->players[0].tracks2take[1].color1 = 9;
    G->players[0].tracks2take[1].color2 = 0;
	G->players[0].tracks2take[2].city1 = 1;
    G->players[0].tracks2take[2].city2 = 2;
    G->players[0].tracks2take[2].length = 1;
    G->players[0].tracks2take[2].color1 = 9;
    G->players[0].tracks2take[2].color2 = 9;
	G->players[0].tracks2take[3].city1 = 14;
    G->players[0].tracks2take[3].city2 = 15;
    G->players[0].tracks2take[3].length = 1;
    G->players[0].tracks2take[3].color1 = 9;
    G->players[0].tracks2take[3].color2 = 0;
    G->players[0].nbTracks2take +=4;
	*/
}

/*!
* @brief update my data at every move I play, depending on the move I play it stores
* the data from the t_move where it needs to go and it recalculate the tracks to take also
* @param t_move *move
* @param t_game *G
*/
void updateMyData(t_move* move,t_game *G){
	int succes=0,r,t;

    if (G->players[0].objectives[0].city1 != -1 && G->players[0].objectives[0].city2 != -1 && G->indice < 7){
		r=G->players[0].objectives[0].score;
		t=G->players[0].objectives[1].score;
		/*
		if (r<t){
			succes=dijsktra(G->players[0].objectives[1].city1,G->players[0].objectives[1].city2,G);
			if (succes == 1){
				chemin2tracks(G,G->players[0].objectives[1]);
			}
		}
		else{
			succes=dijsktra(G->players[0].objectives[0].city1,G->players[0].objectives[0].city2,G);
			if (succes == 1){
				chemin2tracks(G,G->players[0].objectives[0]);
			}
		}
		*/
		succes=dijsktra(G->players[0].objectives[1].city1,G->players[0].objectives[1].city2,G);
		if (succes == 1){
			chemin2tracks(G,G->players[0].objectives[1]);
		}
		succes=dijsktra(G->players[0].objectives[0].city1,G->players[0].objectives[0].city2,G);
		if (succes == 1){
			chemin2tracks(G,G->players[0].objectives[0]);
		}

		triTabLength(G);
    }

    if (move->type == 1 /* claim route*/){
		//G->tabTracks2D[move->claimRoute.city1][move->claimRoute.city2].length = 0;
        G->tabTracks2D[move->claimRoute.city1][move->claimRoute.city2].taken = 1;
        G->tabTracks2D[move->claimRoute.city1][move->claimRoute.city2].me = 1;
        G->tabTracks2D[move->claimRoute.city2][move->claimRoute.city1].taken = 1;
        G->tabTracks2D[move->claimRoute.city2][move->claimRoute.city1].me = 1;
        int k = (G->tabTracks2D[move->claimRoute.city1][move->claimRoute.city2].length - move->claimRoute.nbLocomotives);
        for (int i=0; i< (G->tabTracks2D[move->claimRoute.city1][move->claimRoute.city2].length - move->claimRoute.nbLocomotives) ; i++){
                addCard(&G->players[0], move->claimRoute.color, 0);
				G->players[0].nbWagons -= 1;
        }
        k = move->claimRoute.nbLocomotives;
        for (int i=0; i< move->claimRoute.nbLocomotives ; i++){
                addCard(&G->players[0], 9, 0);
				G->players[0].nbWagons -= 1;
        }
        G->tabTracks2D[move->claimRoute.city1][move->claimRoute.city2].length = 0;
    }
    if (move->type == 2 /* draw blind */){
        //G->players[0].nbCards +=1;
        addCard(&G->players[0], move->drawBlindCard.card, 1);
    }
    if (move->type == 3 /* draw card */){
        //G->players[0].nbCards +=1;
		for (int i=0; i<5; i++){
			G->faceUp[i] = move->drawCard.faceUp[i];
		}
        addCard(&G->players[0], move->drawCard.card, 1);
    }
    if (move->type == 4 /* draw objs */){
        for (int i=0; i<3; i++){
            G->tamp[i]=move->drawObjectives.objectives[i];
        }
    }
    if (move->type == 5 /* choose objs */){
        for (int i=0; i<3; i++){
            if (move->chooseObjectives.chosen[i] == 1){
                G->players[0].objectives[G->players[0].nbObjectives]=G->tamp[i];
                G->players[0].nbObjectives++;
            }
        }
    }
}

/*!
* @brief update the ennemi data at every move he/her plays, depending on the move he/her plays it stores
* the data from the t_move where it needs to go
* @param t_move *move
* @param t_game *G
*/
void updateHisData(t_move* move,t_game *G){
    if (move->type == 1 /* claim route*/){
        G->tabTracks2D[move->claimRoute.city1][move->claimRoute.city2].taken = 1;
        G->tabTracks2D[move->claimRoute.city1][move->claimRoute.city2].length = 999;
        G->tabTracks2D[move->claimRoute.city2][move->claimRoute.city1].taken = 1;
        G->tabTracks2D[move->claimRoute.city2][move->claimRoute.city1].length = 999;
        for (int i=0; i< (G->tabTracks2D[move->claimRoute.city1][move->claimRoute.city2].length - move->claimRoute.nbLocomotives) ; i++){
            addCard(&G->players[1], move->claimRoute.color, 0);
			G->players[1].nbWagons -=1;
        }
        for (int i=0; i< move->claimRoute.nbLocomotives ; i++){
            addCard(&G->players[1], 9, 0);
			G->players[1].nbWagons -=1;
        }
    }
    if (move->type == 2 /* draw blind */){
        G->players[1].nbCards +=1;
    }
    if (move->type == 3 /* draw card */){
		for (int i=0; i<5; i++){
			G->faceUp[i] = move->drawCard.faceUp[i];
		}
        G->players[1].nbCards +=1;
    }
    if (move->type == 4 /* draw objs */){
        //printf("attention ça prend des objs \n");
    }
    if (move->type == 5 /* choose objs */){
        for (int i=0; i<3; i++){
            if (move->chooseObjectives.chosen[i] == 1){
                G->players[1].nbObjectives++;
            }
        }
    }
}

/*!
* @brief fucntion by T.Hilaire to ask for a move by a human
* @param t_move *move
*/
void askMove(t_move* move){
	/* ask for the type */
	printf("Choose a move\n");
	printf("1. claim a route\n");
	printf("2. draw a blind card\n");
	printf("3. draw a card\n");
	printf("4. draw objectives\n");
	printf("5. choose objectives\n");
	scanf("%d", &move->type);

	/* ask for details */
	if (move->type == CLAIM_ROUTE) {
		printf("Give city1, city2, color and nb of locomotives: ");
		scanf("%d %d %d %d", &move->claimRoute.city1, &move->claimRoute.city2, &move->claimRoute.color,
			  &move->claimRoute.nbLocomotives);
	}
	else if (move->type == DRAW_CARD) {
		printf("Give the color: ");
		scanf("%d", &move->drawCard.card);
	}
	else if (move->type == CHOOSE_OBJECTIVES){
		printf("For each objective, give 0 or 1:");
		scanf("%d %d %d", &move->chooseObjectives.chosen[0], &move->chooseObjectives.chosen[1], &move->chooseObjectives.chosen[2]);
	}
}

/*!
* @brief fucntion by T.Hilaire to play the move and return the t_return_code
* @param t_move *move
* @param t_color* lastCard
*/
t_return_code playOurMove(t_move* move, t_color* lastCard){
	t_return_code ret;

	switch (move->type) {
		case 1:
			ret = claimRoute(move->claimRoute.city1, move->claimRoute.city2, move->claimRoute.color, move->claimRoute.nbLocomotives);
			*lastCard = NONE;
			break;
		case 3:
			ret = drawCard(move->drawCard.card, move->drawCard.faceUp);
			*lastCard = (*lastCard==NONE && move->drawCard.card!= MULTICOLOR) ? move->drawCard.card : NONE;
			break;
		case 2:
			ret = drawBlindCard(&move->drawBlindCard.card);
			*lastCard = (*lastCard==NONE) ? move->drawBlindCard.card : NONE;
			break;
		case 4:
			ret = drawObjectives(move->drawObjectives.objectives);
			for(int i=0; i<3; i++){
				printf("%d. %d (", i, move->drawObjectives.objectives[i].city1);
				printCity(move->drawObjectives.objectives[i].city1);
				printf(") -> (");
				printCity(move->drawObjectives.objectives[i].city2);
				printf(") %d (%d pts)\n", move->drawObjectives.objectives[i].city2, move->drawObjectives.objectives[i].score);
				printf("\n");
			}
			*lastCard = NONE;
			break;
		case 5:
			ret = chooseObjectives(move->chooseObjectives.chosen);
			*lastCard = NONE;
			break;
	}
	return ret;
}

/*!
* @brief fucntion by T.Hilaire to know if we need to replay
* @param t_move *move
* @param t_color lastCard
*/
int needReplay(t_move* move, t_color lastCard){
	int replay = 0;

	if (move->type == DRAW_OBJECTIVES)
		replay = 1;
	else if (move->type == DRAW_BLIND_CARD && lastCard == NONE)
		replay = 1;
	else if (move->type == DRAW_CARD && lastCard == NONE && move->drawCard.card != MULTICOLOR)
		replay = 1;

	return replay;
}
