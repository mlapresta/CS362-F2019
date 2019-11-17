#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

int AssertPrintFail(_Bool pass, char *testName){
  if (!pass){
    printf("Test %s: Failed\n", testName);
    return 1;
  }
  return 0;
}

int randomCard(){
  return rand()%27;
}

void compareFunction(struct gameState *GBefore,struct gameState *GAfter){
  int i,j;
  int testfailed=0;
  testfailed+=AssertPrintFail(GBefore->numPlayers==GAfter->numPlayers, "Same # players");
    //check all supply counts (except for estate) the same
  for (i=0; i<27; i++)
    testfailed+=AssertPrintFail(GBefore->supplyCount[i]==GAfter->supplyCount[i] || i==1, "Same # supply count (minus estate card)");

  for (i=0; i<27; i++)
    testfailed+=AssertPrintFail(GBefore->embargoTokens[i]==GAfter->embargoTokens[i], "Same # embargo Tokens");

  testfailed+=AssertPrintFail(GBefore->outpostPlayed==GAfter->outpostPlayed, "Same # outpost Played");
  testfailed+=AssertPrintFail(GBefore->outpostTurn==GAfter->outpostTurn, "Same outpost Turn");
  testfailed+=AssertPrintFail(GBefore->whoseTurn==GAfter->whoseTurn, "Next turn not started yet");
  testfailed+=AssertPrintFail(GBefore->phase==GAfter->phase, "Same phase");
  testfailed+=AssertPrintFail(GBefore->numActions==GAfter->numActions, "Same numActions");
  testfailed+=AssertPrintFail(GBefore->numBuys+1==GAfter->numBuys, "buys increased by 1");
  //Check other player's hands unchanged
  for (i=1; i<GAfter->numPlayers; i++)
    testfailed+=AssertPrintFail(GBefore->handCount[i]==GAfter->handCount[i], "Opponents handcount unchanged");
  for (i=1; i<GAfter->numPlayers; i++){
    for (j=0; j<5; j++)
      testfailed+=AssertPrintFail(GBefore->hand[i][j]==GAfter->hand[i][j], "Opponents hands unchanged");
  }

  //Check other player's decks unchanged
  for (i=1; i<GAfter->numPlayers; i++)
    testfailed+=AssertPrintFail(GBefore->deckCount[i]==GAfter->deckCount[i], "Opponents deckcount unchanged");
  for (i=1; i<GAfter->numPlayers; i++){
    for (j=0; j<GAfter->deckCount[i]; j++)
      testfailed+=AssertPrintFail(GBefore->deck[i][j]==GAfter->deck[i][j], "Opponents deck unchanged");
  }

  //Check other player's discard unchanged
  for (i=1; i<GAfter->numPlayers; i++)
    testfailed+=AssertPrintFail(GBefore->discardCount[i]==GAfter->discardCount[i], "Opponents discardcount unchanged");
  for (i=1; i<GAfter->numPlayers; i++){
    for (j=0; j<GAfter->discardCount[i]; j++)
      testfailed+=AssertPrintFail(GBefore->discard[i][j]==GAfter->discard[i][j], "Opponents discard unchanged");
  }

  //Check other player's played cards increased

  testfailed+=AssertPrintFail(GBefore->playedCardCount<GAfter->playedCardCount, "Playedcardcount increased by 1");


  testfailed+=AssertPrintFail(GBefore->coins+4==GAfter->coins ^ GBefore->supplyCount[1]==GAfter->supplyCount[1]-1, "Either coin+4 XOR estate gained");

  if (testfailed>0){
    printf("Before function run\n");
    printState(GBefore);
    printf("After function run\n");
    printState(GAfter);
    for (i=0; i<GAfter->numPlayers; i++){
      printf("Player %i hand before:\n", i);
      printHand(i, GBefore);
      printf("Player %i hand after:\n", i);
      printHand(i, GAfter);
    }

    printf("PrintPlayed before function run\n");
    printPlayed(0, GBefore);
    printf("Print Played after function run\n");
    printPlayed(0, GAfter);
    printf("******************************************************\n");
  }

}

int myInitializeGame(int numPlayers, int kingdomCards[10], int randomSeed,
                   struct gameState *state){
  int i;
  int j;
  int it;

  //set up random number generator
  SelectStream(1);
  PutSeed((long)randomSeed);

  //check number of players
  if (numPlayers > MAX_PLAYERS || numPlayers < 2)
  {
      return -1;
  }

  //set number of players
  state->numPlayers = numPlayers;

  //check selected kingdom cards are different
  for (i = 0; i < 10; i++)
  {
      for (j = 0; j < 10; j++)
      {
          if (j != i && kingdomCards[j] == kingdomCards[i])
          {
              return -1;
          }
      }
  }


  //initialize supply
  ///////////////////////////////

  //set number of Curse cards
  if (numPlayers == 2)
  {
      state->supplyCount[curse] = 1+rand()%10;
  }
  else if (numPlayers == 3)
  {
      state->supplyCount[curse] = 1+rand()%20;
  }
  else
  {
      state->supplyCount[curse] = 1+rand()%30;
  }

  //set number of Victory cards
  if (numPlayers == 2)
  {
      state->supplyCount[estate] = 1+rand()%8;
      state->supplyCount[duchy] = 1+rand()%8;
      state->supplyCount[province] = 1+rand()%8;
  }
  else
  {
      state->supplyCount[estate] = 1+rand()%8;
      state->supplyCount[duchy] = 1+rand()%8;
      state->supplyCount[province] = 1+rand()%8;
  }

  //set number of Treasure cards
  state->supplyCount[copper] = 1+rand()%(60 - (7 * numPlayers));
  state->supplyCount[silver] = 1+rand()%40;
  state->supplyCount[gold] = 1+rand()%30;

  //set number of Kingdom cards
  for (i = adventurer; i <= treasure_map; i++)       	//loop all cards
  {
      for (j = 0; j < 10; j++)           		//loop chosen cards
      {
          if (kingdomCards[j] == i)
          {
              //check if card is a 'Victory' Kingdom card
              if (kingdomCards[j] == great_hall || kingdomCards[j] == gardens)
              {
                  if (numPlayers == 2) {
                      state->supplyCount[i] = 1+rand()%10;
                  }
                  else {
                      state->supplyCount[i] = 1+rand()%10;
                  }
              }
              else
              {
                  state->supplyCount[i] = 1+rand()%10;
              }
              break;
          }
          else    //card is not in the set choosen for the game
          {
              state->supplyCount[i] = -1;
          }
      }

  }

  ////////////////////////
  //supply intilization complete

  //set player decks
  for (i = 0; i < numPlayers; i++)
  {
      state->deckCount[i] = rand()%51;
      for (j = 0; j < state->deckCount[i]; j++)
        state->deck[i][j] = randomCard();

  }

  //shuffle player decks
  for (i = 0; i < numPlayers; i++)
  {
      if ( shuffle(i, state) < 0 )
      {
          return -1;
      }
  }

  //set player discard piles
  for (i = 0; i < numPlayers; i++)
  {

      state->discardCount[i] = rand()%51;
      for (j = 0; j < state->discardCount[i]; j++)
        state->discard[i][j] = randomCard();
      //draw 5 cards
      // for (j = 0; j < 5; j++)
      //	{
      //	  drawCard(i, state);
      //	}
  }

  //set embargo tokens to 0 for all supply piles
  for (i = 0; i <= treasure_map; i++)
  {
      state->embargoTokens[i] = rand()%10;
  }

  //initialize first player's turn
  state->outpostPlayed = rand()%100;
  state->phase = rand()%100;
  state->numActions = rand()%100;
  state->numBuys = rand()%100;
  state->playedCardCount = rand()%10;
  state->whoseTurn = rand()%state->numPlayers;
  state->coins = rand()%100;
  //randomize num of cards in player's hands, and the cards

  //int it; move to top
  for (j=0; j<state->numPlayers; j++){
     state->handCount[j]=1+rand()%(5);
     //assign how many
     for (i=0; i<state->handCount[j]; i++)
       state->hand[j][i] = randomCard();
     for (i=state->handCount[j]; i<5; i++)
       state->hand[j][i]=-1;

  }

  for (i=0; i<state->playedCardCount; i++)
    state->playedCards[i]=randomCard();


  return 0;
}

void barontest(){
   int i, j;
   int seed = 100;
   int numPlayer = 2;
   int maxBonus = 10;
   int p, r, handCount;
   int bonus;
   int playerDiscard;
   int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
   int buySave, coinSave;
   int gameLoop;
   int emptyhandcards;

   for (gameLoop=0; gameLoop<5000; gameLoop++){
       struct gameState G;
       struct gameState GSave;
       memset(&G, 0, sizeof(struct gameState));
       //set a random number of players between [2-4]
       numPlayer=2+rand()%3;

       r = myInitializeGame(numPlayer, k, seed, &G); // initialize a new game
       G.hand[G.whoseTurn][0]=baron;
       //assign random player's choice for the baron card, [0,1]
       playerDiscard=rand()%2;
       memcpy(&GSave, &G, sizeof(struct gameState));
       BaronAction(playerDiscard, &G, G.whoseTurn, 0);
       compareFunction(&GSave, &G);
    }
}

int main(int argc, char *argv[]){
  clock_t t;

  srand(time(NULL));
  t = clock();
  barontest();
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
  printf("fun() took %f seconds to execute \n", time_taken);
}
