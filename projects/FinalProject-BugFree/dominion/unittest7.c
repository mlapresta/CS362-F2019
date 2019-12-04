/*
Name: Maryum Shabazz
Date: 11/30/19
Description: Test for Bug 7
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
    int k[10] = {adventurer, council_room, tribute, gardens, mine
               , remodel, smithy, village, baron, minion};
    struct gameState G;
    int card=19, choice1=0,choice2=0,choice3=0, handPos=0;
    int bonus=0;
    int bonusSaved;
    printf("Testing bug 7\n");


    //initialize game
   	initializeGame(numPlayer,k,seed,&G);
    int currentPlayer=whoseTurn(&G);

    int nextPlayer=currentPlayer+1;
  G.handCount[0] = 2;
  //Setup player hand with 1 tribute and 1 copper
  G.hand[0][0] = tribute;
  G.supplyCount[tribute]--;
  G.hand[0][1] = copper;
  G.supplyCount[copper]--;

  G.handCount[1] = 3;

  //Set second player's hand, we set to 2 mines and 1 gold
  G.deck[nextPlayer][G.deckCount[nextPlayer]-1] = gold;
  G.deck[nextPlayer][G.deckCount[nextPlayer]-2] = mine;
  G.deck[nextPlayer][G.deckCount[nextPlayer]-3] = mine;

  //note: when calling the tribute card (number 19) choice1,choice2
  //choice3 and bonus serve no purpose
  int actionsPre=G.numActions;
  int coinsPre=G.coins;
  bonusSaved = bonus;
  cardEffect(card, choice1,choice2,choice3,&G,handPos,&bonus);

  //asserts if actions before and after playing tribute card are the same.
  //if third card is not looked at actions of player 1 should be increaed by 2 only.
  if((assertValues(G.numActions,(actionsPre+2)))&& (assertValues(coinsPre+2,G.coins)))
  {
    printf("Pass: Third card not look at \n");
  }
  else
  {
    printf("Fail: Third Card looked at \n");
  }

  printf("Added test due to bug #8 fix for how coins are handled \n");
  //Added this test due to the fix for bug 8
  //asserts if actions before and after playing tribute card are the same.
  //if third card is not looked at actions of player 1 should be increaed by 2 only.
  if((assertValues(G.numActions,(actionsPre+2)))&& (assertValues(bonusSaved+2,bonus)))
  {
    printf("Pass: Third card not look at \n");
  }
  else
  {
    printf("Fail: Third Card looked at \n");
  }









printf("END TEST bug 7\n");
printf("******************************************************************************************************\n");
return 0;

}
