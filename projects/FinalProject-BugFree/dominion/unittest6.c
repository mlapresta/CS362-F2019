
//Name: Maryum Shabazz
//Date: 11/30/19
//Description: Test for Bug 6


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
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, minion};
    struct gameState G;
    int card=9, choice1=0,choice2=0,choice3=0, handPos=0; 
    int bonus=0;

    printf("Testing bug 6\n");  

    
    //initialize game 
   	initializeGame(numPlayer,k,seed,&G);
    
  //set discard pile number at 0 
  G.discardCount[0]=0;

  //set player to first player
  int player=0;
  //counts coins pre feast card played
  int coinsPre=G.coins;

  //note: when calling the feast card (number 9) choice2
  //choice3 and bonus serve no purpose 
  cardEffect(card, choice1,choice2,choice3,&G,handPos,&bonus);

  //asserts if coins before and after feast card the same. 
  if(assertValues(coinsPre,G.coins))
  {
    printf("Pass: Coins for player one the same after playing feast card \n");
  }
  else
  {
    printf("Fail: Coins for player one not the same after playing feast card\n");
  }
  
  




    	
printf("END TEST bug 6\n");
printf("******************************************************************************************************\n");
return 0;   

}
