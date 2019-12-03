/*
Name: Maryum Shabazz
Date: 11/30/19
Description: Test for Bug 5
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <time.h>
#include <stdlib.h>


//Values to compare functions
int assertValues(int paramOne, int paramTwo)
{
    if(paramOne==paramTwo)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int main() {
    int seed = 100;
    int numPlayer=2;
	//seed random number generator
	srand(time(0));
    int choice1=0;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, minion};
    struct gameState G, testG;
    printf("Testing bug 5\n");  

    
    //initialize game 
   	initializeGame(numPlayer,k,seed,&G);
    G.handCount[0] = 2;
  //Setup player hand with 1 mine and 1 copper
  G.hand[0][0] = mine;
  G.supplyCount[mine]--;
  G.hand[0][1] = copper;
  G.supplyCount[copper]--;

  G.discardCount[0]=3;
  //setup player discard pile with 1 mine and 2 coppers 
  G.discard[0][0]=copper;
  G.supplyCount[copper]--;
  G.discard[0][1]=copper;
  G.supplyCount[copper]--;
  G.discard[0][2] = mine;
  G.supplyCount[mine]--;

  
  G.deckCount[0]=5;

  //set up player deck pile with 3 curses and 2 estates
  G.deck[0][0]=curse;
  G.supplyCount[curse]--;
  G.deck[0][1]=curse;
  G.supplyCount[curse]--;
  G.deck[0][2]=curse;
  G.supplyCount[curse]--;
  G.deck[0][3] = estate;
  G.supplyCount[estate]--;
  G.deck[0][4]=estate;
  G.supplyCount[estate]--;

  //set player to first player
  int player=0;
  int score=scoreFor(player,&G);


  if( assertValues(score,-1))
  {
  	printf("Pass: Score for player one  is correct\n");
  }
  else
  {
  	printf("Fail: Score for player one  is not correct\n");
  }

    	
printf("END TEST bug 5\n");
printf("******************************************************************************************************\n");
return 0;   

}
