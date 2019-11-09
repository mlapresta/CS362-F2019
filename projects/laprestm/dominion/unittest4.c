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

void testTributeCard(){
  int i;
  int seed = 100;
  int numPlayer = 2;
  int maxBonus = 10;
  int p, r, handCount;
  int bonus;
  int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
  int deckSaveP1, deckSaveP2, coinSave, supplySave, actionSave, handCountSaveP1;
  int discardSaveP1, discardSaveP2;
  struct gameState G;
  memset(&G, 23, sizeof(struct gameState));

  r = myInitializeGame(numPlayer, k, seed, &G); // initialize a new game
  G.handCount[0]=5;
  G.handCount[1]=5;
  printf("\n\n##############################################################\n");
  printf("Tribute Test Case:\nPlayer to the left reveals 2 different action card\n");
  printf("##############################################################\n");

  //Set first player's hand
  G.hand[0][0] = tribute;
  G.supplyCount[tribute]--;
  G.hand[0][1] = copper;
  G.supplyCount[copper]--;
  G.hand[0][2] = copper;
  G.supplyCount[copper]--;
  G.hand[0][3] = copper;
  G.supplyCount[copper]--;
  G.hand[0][4] = copper;
  G.supplyCount[copper]--;

  //Set second player's hand
  G.hand[1][0] = silver;
  G.supplyCount[silver]--;
  G.hand[1][1] = silver;
  G.supplyCount[silver]--;
  G.hand[1][2] = silver;
  G.supplyCount[silver]--;
  G.hand[1][3] = silver;
  G.supplyCount[silver]--;
  G.hand[1][4] = silver;
  G.supplyCount[silver]--;

  //set second player's deck:
  G.deckCount[1] = 2;
  G.deck[1][0] = minion;
  G.deck[1][1] = mine;

  actionSave=G.numActions;

  TributeAction(&G, 0);
  //original number of actions is 1, playing card makes it zero, so an increase is delta=3 actions
  testAssert((actionSave+4==G.numActions), "Number of actions increased by 4");
  testAssert((G.hand[0][0]!=tribute), "Tribute discarded from hand");
  testAssert((G.hand[0][0]==copper)&&(G.hand[0][1]==copper)&&(G.hand[0][2]==copper)&&(G.hand[0][3]==copper)&&(G.hand[0][4]==-1), "After Tribute removed from hand, rest of hand unchanged");
  testAssert((G.hand[1][0]==silver)&&(G.hand[1][1]==silver)&&(G.hand[1][2]==silver)&&(G.hand[1][3]==silver)&&(G.hand[1][4]==silver), "Opponent's hand unchanged");


  r = myInitializeGame(numPlayer, k, seed, &G); // initialize a new game
  G.handCount[0]=5;
  G.handCount[1]=5;
  printf("\n\n##############################################################\n");
  printf("Tribute Test Case:\nPlayer to the left reveals 2 different treasure cards\n");
  printf("##############################################################\n");

  //Set first player's hand
  G.hand[0][0] = tribute;
  G.supplyCount[tribute]--;
  G.hand[0][1] = copper;
  G.supplyCount[copper]--;
  G.hand[0][2] = copper;
  G.supplyCount[copper]--;
  G.hand[0][3] = copper;
  G.supplyCount[copper]--;
  G.hand[0][4] = copper;
  G.supplyCount[copper]--;

  //Set second player's hand
  G.hand[1][0] = silver;
  G.supplyCount[silver]--;
  G.hand[1][1] = silver;
  G.supplyCount[silver]--;
  G.hand[1][2] = silver;
  G.supplyCount[silver]--;
  G.hand[1][3] = silver;
  G.supplyCount[silver]--;
  G.hand[1][4] = silver;
  G.supplyCount[silver]--;

  //set second player's deck:
  G.deckCount[1] = 2;
  G.deck[1][0] = silver;
  G.deck[1][1] = gold;

  coinSave=G.coins;

  TributeAction(&G, 0);
  testAssert((coinSave+4==G.coins), "Number of coins increased by 4");
  testAssert((G.hand[0][0]!=tribute), "After Tribute discarded from hand");
  testAssert((G.hand[0][0]==copper)&&(G.hand[0][1]==copper)&&(G.hand[0][2]==copper)&&(G.hand[0][3]==copper)&&(G.hand[0][4]==-1), "Tribute removed from hand, rest of hand unchanged");
  testAssert((G.hand[1][0]==silver)&&(G.hand[1][1]==silver)&&(G.hand[1][2]==silver)&&(G.hand[1][3]==silver)&&(G.hand[1][4]==silver), "Opponent's hand unchanged");

  r = myInitializeGame(numPlayer, k, seed, &G); // initialize a new game
  G.handCount[0]=5;
  G.handCount[1]=5;
  printf("\n\n##############################################################\n");
  printf("Tribute Test Case:\nPlayer to the left reveals 2 different victory cards\n");
  printf("##############################################################\n");

  //Set first player's hand
  G.hand[0][0] = tribute;
  G.supplyCount[tribute]--;
  G.hand[0][1] = copper;
  G.supplyCount[copper]--;
  G.hand[0][2] = copper;
  G.supplyCount[copper]--;
  G.hand[0][3] = copper;
  G.supplyCount[copper]--;
  G.hand[0][4] = copper;
  G.supplyCount[copper]--;

  //Set second player's hand
  G.hand[1][0] = silver;
  G.supplyCount[silver]--;
  G.hand[1][1] = silver;
  G.supplyCount[silver]--;
  G.hand[1][2] = silver;
  G.supplyCount[silver]--;
  G.hand[1][3] = silver;
  G.supplyCount[silver]--;
  G.hand[1][4] = silver;
  G.supplyCount[silver]--;

  //set second player's deck:
  G.deckCount[1] = 2;
  G.deck[1][0] = estate;
  G.deck[1][1] = great_hall;

  handCountSaveP1=G.handCount[0];

  TributeAction(&G, 0);
  testAssert((handCountSaveP1+3==G.handCount[0]), "Number of cards in hand increased by 4"); //4 cards-1 used tribute cards = 3 cards
  testAssert((G.hand[0][0]!=tribute), "Tribute discarded from hand");
  testAssert((G.hand[0][1]==copper)&&(G.hand[0][2]==copper)&&(G.hand[0][3]==copper)&&(G.hand[0][4]==copper), "rest of hand unchanged (minus the added cards)");
  testAssert((G.hand[1][0]==silver)&&(G.hand[1][1]==silver)&&(G.hand[1][2]==silver)&&(G.hand[1][3]==silver)&&(G.hand[1][4]==silver), "Opponent's hand unchanged");

  memset(&G, 23, sizeof(struct gameState));

  r = myInitializeGame(numPlayer, k, seed, &G); // initialize a new game
  G.handCount[0]=5;
  G.handCount[1]=5;
  printf("\n\n##############################################################\n");
  printf("Tribute Test Case:\nPlayer to the left reveals 2 nonunique action card\n");
  printf("##############################################################\n");

  //Set first player's hand
  G.hand[0][0] = tribute;
  G.supplyCount[tribute]--;
  G.hand[0][1] = copper;
  G.supplyCount[copper]--;
  G.hand[0][2] = copper;
  G.supplyCount[copper]--;
  G.hand[0][3] = copper;
  G.supplyCount[copper]--;
  G.hand[0][4] = copper;
  G.supplyCount[copper]--;

  //Set second player's hand
  G.hand[1][0] = silver;
  G.supplyCount[silver]--;
  G.hand[1][1] = silver;
  G.supplyCount[silver]--;
  G.hand[1][2] = silver;
  G.supplyCount[silver]--;
  G.hand[1][3] = silver;
  G.supplyCount[silver]--;
  G.hand[1][4] = silver;
  G.supplyCount[silver]--;

  //set second player's deck:
  G.deckCount[1] = 2;
  G.deck[1][0] = minion;
  G.deck[1][1] = minion;

  actionSave=G.numActions;

  TributeAction(&G, 0);
  testAssert((actionSave+2==G.numActions), "Number of actions increased by 2");
  testAssert((G.hand[0][0]!=tribute), "Tribute discarded from hand");
  testAssert((G.hand[0][0]==copper)&&(G.hand[0][1]==copper)&&(G.hand[0][2]==copper)&&(G.hand[0][3]==copper)&&(G.hand[0][4]==-1), "After Tribute removed from hand, rest of hand unchanged");
  testAssert((G.hand[1][0]==silver)&&(G.hand[1][1]==silver)&&(G.hand[1][2]==silver)&&(G.hand[1][3]==silver)&&(G.hand[1][4]==silver), "Opponent's hand unchanged");


  r = myInitializeGame(numPlayer, k, seed, &G); // initialize a new game
  G.handCount[0]=5;
  G.handCount[1]=5;
  printf("\n\n##############################################################\n");
  printf("Tribute Test Case:\nPlayer to the left reveals 2 nonunique treasure cards\n");
  printf("##############################################################\n");

  //Set first player's hand
  G.hand[0][0] = tribute;
  G.supplyCount[tribute]--;
  G.hand[0][1] = copper;
  G.supplyCount[copper]--;
  G.hand[0][2] = copper;
  G.supplyCount[copper]--;
  G.hand[0][3] = copper;
  G.supplyCount[copper]--;
  G.hand[0][4] = copper;
  G.supplyCount[copper]--;

  //Set second player's hand
  G.hand[1][0] = silver;
  G.supplyCount[silver]--;
  G.hand[1][1] = silver;
  G.supplyCount[silver]--;
  G.hand[1][2] = silver;
  G.supplyCount[silver]--;
  G.hand[1][3] = silver;
  G.supplyCount[silver]--;
  G.hand[1][4] = silver;
  G.supplyCount[silver]--;

  //set second player's deck:
  G.deckCount[1] = 2;
  G.deck[1][0] = silver;
  G.deck[1][1] = silver;

  coinSave=G.coins;

  TributeAction(&G, 0);
  testAssert((coinSave+2==G.coins), "Number of coins increased by 2");
  testAssert((G.hand[0][0]!=tribute), "Tribute discarded from hand");
  testAssert((G.hand[0][0]==copper)&&(G.hand[0][1]==copper)&&(G.hand[0][2]==copper)&&(G.hand[0][3]==copper)&&(G.hand[0][4]==-1), "After Tribute removed from hand, rest of hand unchanged");
  testAssert((G.hand[1][0]==silver)&&(G.hand[1][1]==silver)&&(G.hand[1][2]==silver)&&(G.hand[1][3]==silver)&&(G.hand[1][4]==silver), "Opponent's hand unchanged");

  r = myInitializeGame(numPlayer, k, seed, &G); // initialize a new game
  G.handCount[0]=5;
  G.handCount[1]=5;
  printf("\n\n##############################################################\n");
  printf("Tribute Test Case:\nPlayer to the left reveals 2 nonunique victory cards\n");
  printf("##############################################################\n");

  //Set first player's hand
  G.hand[0][0] = tribute;
  G.supplyCount[tribute]--;
  G.hand[0][1] = copper;
  G.supplyCount[copper]--;
  G.hand[0][2] = copper;
  G.supplyCount[copper]--;
  G.hand[0][3] = copper;
  G.supplyCount[copper]--;
  G.hand[0][4] = copper;
  G.supplyCount[copper]--;

  //Set second player's hand
  G.hand[1][0] = silver;
  G.supplyCount[silver]--;
  G.hand[1][1] = silver;
  G.supplyCount[silver]--;
  G.hand[1][2] = silver;
  G.supplyCount[silver]--;
  G.hand[1][3] = silver;
  G.supplyCount[silver]--;
  G.hand[1][4] = silver;
  G.supplyCount[silver]--;

  //set second player's deck:
  G.deckCount[1] = 2;
  G.deck[1][0] = estate;
  G.deck[1][1] = estate;

  handCountSaveP1=G.handCount[0];

  TributeAction(&G, 0);
  testAssert((handCountSaveP1+1==G.handCount[0]), "Number of cards in hand increased by 2"); //After discarding the tribute card, we have 4 cards, and then gain 2 for a delta of +1
  testAssert((G.hand[0][0]!=tribute), "Tribute discarded from hand");
  testAssert((G.hand[0][0]==copper)&&(G.hand[0][1]==copper)&&(G.hand[0][2]==copper)&&(G.hand[0][3]==copper), "rest of hand unchanged (minus the added cards)");
  testAssert((G.hand[1][0]==silver)&&(G.hand[1][1]==silver)&&(G.hand[1][2]==silver)&&(G.hand[1][3]==silver)&&(G.hand[1][4]==silver), "Opponent's hand unchanged");

  r = myInitializeGame(numPlayer, k, seed, &G); // initialize a new game
  G.handCount[0]=5;
  G.handCount[1]=5;
  printf("\n\n##############################################################\n");
  printf("Tribute Test Case:\nPlayer to the left reveals 1 action and 1 treasure card\n");
  printf("##############################################################\n");

  //Set first player's hand
  G.hand[0][0] = tribute;
  G.supplyCount[tribute]--;
  G.hand[0][1] = copper;
  G.supplyCount[copper]--;
  G.hand[0][2] = copper;
  G.supplyCount[copper]--;
  G.hand[0][3] = copper;
  G.supplyCount[copper]--;
  G.hand[0][4] = copper;
  G.supplyCount[copper]--;

  //Set second player's hand
  G.hand[1][0] = silver;
  G.supplyCount[silver]--;
  G.hand[1][1] = silver;
  G.supplyCount[silver]--;
  G.hand[1][2] = silver;
  G.supplyCount[silver]--;
  G.hand[1][3] = silver;
  G.supplyCount[silver]--;
  G.hand[1][4] = silver;
  G.supplyCount[silver]--;

  //set second player's deck:
  G.deckCount[1] = 2;
  G.deck[1][0] = mine;
  G.deck[1][1] = silver;

  coinSave=G.coins;
  actionSave=G.numActions;

  TributeAction(&G, 0);
  testAssert((coinSave+2==G.coins), "Number of coins increased by 2");
  testAssert((actionSave+2==G.numActions), "Number of actions increased by 2"); //We use one action playing the tribute card, so the total gained actions vs the original is 2-1
  testAssert((G.hand[0][0]!=tribute), "Tribute discarded from hand");
  testAssert((G.hand[0][0]==copper)&&(G.hand[0][1]==copper)&&(G.hand[0][2]==copper)&&(G.hand[0][3]==copper)&&(G.hand[0][4]==-1), "After Tribute removed from hand, rest of hand unchanged");
  testAssert((G.hand[1][0]==silver)&&(G.hand[1][1]==silver)&&(G.hand[1][2]==silver)&&(G.hand[1][3]==silver)&&(G.hand[1][4]==silver), "Opponent's hand unchanged");

  r = myInitializeGame(numPlayer, k, seed, &G); // initialize a new game
  G.handCount[0]=5;
  G.handCount[1]=5;
  printf("\n\n##############################################################\n");
  printf("Tribute Test Case:\nPlayer to the left reveals 1 treasure card and 1 victory card\n");
  printf("##############################################################\n");

  //Set first player's hand
  G.hand[0][0] = tribute;
  G.supplyCount[tribute]--;
  G.hand[0][1] = copper;
  G.supplyCount[copper]--;
  G.hand[0][2] = copper;
  G.supplyCount[copper]--;
  G.hand[0][3] = copper;
  G.supplyCount[copper]--;
  G.hand[0][4] = copper;
  G.supplyCount[copper]--;

  //Set second player's hand
  G.hand[1][0] = silver;
  G.supplyCount[silver]--;
  G.hand[1][1] = silver;
  G.supplyCount[silver]--;
  G.hand[1][2] = silver;
  G.supplyCount[silver]--;
  G.hand[1][3] = silver;
  G.supplyCount[silver]--;
  G.hand[1][4] = silver;
  G.supplyCount[silver]--;

  //set second player's deck:
  G.deckCount[1] = 2;
  G.deck[1][0] = silver;
  G.deck[1][1] = estate;

  coinSave=G.coins;
  handCountSaveP1=G.handCount[0];

  TributeAction(&G, 0);
  testAssert((coinSave+2==G.coins), "Number of cards in hand increased by 2");
  testAssert((handCountSaveP1+1==G.handCount[0]), "Number of cards in hand increased by 2"); //After discarding the tribute card, we have 4 cards, and then gain 2 for a delta of +1
  testAssert((G.hand[0][0]!=tribute), "Tribute discarded from hand");
  testAssert((G.hand[0][1]==copper)&&(G.hand[0][2]==copper)&&(G.hand[0][3]==copper)&&(G.hand[0][4]==copper), "rest of hand unchanged (minus the added cards)");
  testAssert((G.hand[1][0]==silver)&&(G.hand[1][1]==silver)&&(G.hand[1][2]==silver)&&(G.hand[1][3]==silver)&&(G.hand[1][4]==silver), "Opponent's hand unchanged");

  r = myInitializeGame(numPlayer, k, seed, &G); // initialize a new game
  G.handCount[0]=5;
  G.handCount[1]=5;
  printf("\n\n##############################################################\n");
  printf("Tribute Test Case:\nPlayer to the left does not have cards to reveal\n");
  printf("##############################################################\n");

  //Set first player's hand
  G.hand[0][0] = tribute;
  G.supplyCount[tribute]--;
  G.hand[0][1] = copper;
  G.supplyCount[copper]--;
  G.hand[0][2] = copper;
  G.supplyCount[copper]--;
  G.hand[0][3] = copper;
  G.supplyCount[copper]--;
  G.hand[0][4] = copper;
  G.supplyCount[copper]--;

  //Set second player's hand
  G.hand[1][0] = silver;
  G.supplyCount[silver]--;
  G.hand[1][1] = silver;
  G.supplyCount[silver]--;
  G.hand[1][2] = silver;
  G.supplyCount[silver]--;
  G.hand[1][3] = silver;
  G.supplyCount[silver]--;
  G.hand[1][4] = silver;
  G.supplyCount[silver]--;

  //set second player's deck:
  G.deckCount[1] = 0;
  G.discardCount[1]=0;
  coinSave=G.coins;
  handCountSaveP1=G.handCount[0];

  TributeAction(&G, 0);
  testAssert((G.hand[0][0]!=tribute), "Tribute discarded from hand");
  testAssert((G.hand[0][0]==tribute)&&(G.hand[0][1]==copper)&&(G.hand[0][2]==copper)&&(G.hand[0][3]==copper)&&(G.hand[0][4]==copper), "rest of hand unchanged (minus the added cards)");
  testAssert((G.hand[1][0]==silver)&&(G.hand[1][1]==silver)&&(G.hand[1][2]==silver)&&(G.hand[1][3]==silver)&&(G.hand[1][4]==silver), "Opponent's hand unchanged");

}


int main(int argc, char *argv[]){






  testTributeCard();

  return 0;
}
