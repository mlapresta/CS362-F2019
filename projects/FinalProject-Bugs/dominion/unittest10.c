#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

void testAssert(_Bool pass, char *testName){
  if (pass){
    printf("Test %s: Passed\n", testName);
  }
  else{
    printf("Test %s: Failed\n", testName);
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
      state->supplyCount[curse] = 10;
  }
  else if (numPlayers == 3)
  {
      state->supplyCount[curse] = 20;
  }
  else
  {
      state->supplyCount[curse] = 30;
  }

  //set number of Victory cards
  if (numPlayers == 2)
  {
      state->supplyCount[estate] = 8;
      state->supplyCount[duchy] = 8;
      state->supplyCount[province] = 8;
  }
  else
  {
      state->supplyCount[estate] = 12;
      state->supplyCount[duchy] = 12;
      state->supplyCount[province] = 12;
  }

  //set number of Treasure cards
  state->supplyCount[copper] = 60 - (7 * numPlayers);
  state->supplyCount[silver] = 40;
  state->supplyCount[gold] = 30;

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
                      state->supplyCount[i] = 8;
                  }
                  else {
                      state->supplyCount[i] = 12;
                  }
              }
              else
              {
                  state->supplyCount[i] = 10;
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
      state->deckCount[i] = 0;
      for (j = 0; j < 3; j++)
      {
          state->deck[i][j] = estate;
          state->deckCount[i]++;
      }
      for (j = 3; j < 10; j++)
      {
          state->deck[i][j] = copper;
          state->deckCount[i]++;
      }
  }

  //shuffle player decks
  for (i = 0; i < numPlayers; i++)
  {
      if ( shuffle(i, state) < 0 )
      {
          return -1;
      }
  }

  //draw player hands
  for (i = 0; i < numPlayers; i++)
  {
      //initialize hand size to zero
      state->handCount[i] = 0;
      state->discardCount[i] = 0;
      //draw 5 cards
      // for (j = 0; j < 5; j++)
      //	{
      //	  drawCard(i, state);
      //	}
  }

  //set embargo tokens to 0 for all supply piles
  for (i = 0; i <= treasure_map; i++)
  {
      state->embargoTokens[i] = 0;
  }

  //initialize first player's turn
  state->outpostPlayed = 0;
  state->phase = 0;
  state->numActions = 1;
  state->numBuys = 1;
  state->playedCardCount = 0;
  state->whoseTurn = 0;
  state->handCount[state->whoseTurn] = 0;
  //int it; move to top

//Removed the draw cards so that I could manually set the user's cards
//and the counts would be correct
/*
  //Moved draw cards to here, only drawing at the start of a turn
  for (it = 0; it < 5; it++) {
      drawCard(state->whoseTurn, state);
  }
*/
  updateCoins(state->whoseTurn, state, 0);

  return 0;
}
void runUnitTest(){
  int i;
  int seed = 100;
  int numPlayer = 3;
  int maxBonus = 10;
  int p, r, handCount;
  int bonus;
  int k[10] = {ambassador, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
  int buySave, coinSave;
  struct gameState G;
  struct gameState Gsaved;
  memset(&G, 23, sizeof(struct gameState));
  r = myInitializeGame(numPlayer, k, seed, &G); // initialize a new game

  G.handCount[0] = 3;

  G.hand[0][0] = silver;
  G.hand[0][1] = silver;
  G.hand[0][2] = ambassador;

  memcpy(&Gsaved, &G, sizeof(struct gameState));
  //int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)
  cardEffect(ambassador, 0, 2, 0, &G, 2, 0);
  char message[] = "test that cards removed from hand";
  testAssert(G.handCount[0] == Gsaved.handCount[0] - 2, message);
  char message[] = "test that cards added to supply";
  testAssert(G.supplyCount[silver] == Gsaved.supplyCount[silver] + 2, message);
}

int main(int argc, char *argv[]){

  runUnitTest();
  return 0;
}
