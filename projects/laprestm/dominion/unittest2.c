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

void testMinionCard(){
  int i;
  int seed = 100;
  int numPlayer = 2;
  int maxBonus = 10;
  int p, r, handCount;
  int bonus;
  int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
  int deckSaveP1, deckSaveP2, coinSave, actionSave;
  struct gameState G;
  memset(&G, 23, sizeof(struct gameState));
  r = myInitializeGame(numPlayer, k, seed, &G); // initialize a new game
  G.handCount[0]=5;
  G.handCount[1]=5;
  printf("\n\n##############################################################\n");
  printf("Minion Test Case:\n	Player has chosen to increase coins by 2\n");
  printf("##############################################################\n");

  //Set first player's hand

  G.hand[0][0] = minion;
  G.supplyCount[minion]--;
  G.hand[0][1] = gold;
  G.supplyCount[gold]--;
  G.hand[0][2] = gold;
  G.supplyCount[gold]--;
  G.hand[0][3] = gold;
  G.supplyCount[gold]--;
  G.hand[0][4] = gold;
  G.supplyCount[gold]--;

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

  deckSaveP1 = G.deckCount[0];
  deckSaveP2 = G.deckCount[1];
  coinSave = G.coins;
  actionSave=G.numActions;

  MinionAction(1, &G, 0);

  testAssert((actionSave+1==G.numActions), "Action count increased by 1");
  testAssert((coinSave+2==G.coins), "Coin count increased by 2");
  testAssert((G.hand[1][0]==silver)&&(G.hand[1][1]==silver)&&(G.hand[1][2]==silver)&&(G.hand[1][3]==silver)&&(G.hand[1][4]==silver), "Other Player's hand unchanged");
  testAssert((G.hand[0][0]!=minion)&&(G.playedCards[0]==minion)&&(G.playedCardCount==1), "Minion discarded from hand");
  testAssert((G.hand[0][0]==gold)&&(G.hand[0][1]==gold)&&(G.hand[0][2]==gold), "Player's remaining cards unchanged");
  testAssert((deckSaveP2=G.deckCount[1]), "Opponent's deck untouched");
  testAssert((deckSaveP1=G.deckCount[0]), "Player's deck untouched");

  memset(&G, 23, sizeof(struct gameState));
  r = myInitializeGame(numPlayer, k, seed, &G); // initialize a new game

  G.handCount[0]=5;
  G.handCount[1]=5;
  printf("\n\n##############################################################\n");
  printf("Minion Test Case:\nPlayer has chosen to discard their hand, other player has 5 cards in hand\n");
  printf("##############################################################\n");

  //Set first player's hand

  G.hand[0][0] = minion;
  G.supplyCount[minion]--;
  G.hand[0][1] = gold;
  G.supplyCount[gold]--;
  G.hand[0][2] = gold;
  G.supplyCount[gold]--;
  G.hand[0][3] = gold;
  G.supplyCount[gold]--;
  G.hand[0][4] = gold;
  G.supplyCount[gold]--;

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


  deckSaveP1 = G.deckCount[0];
  deckSaveP2 = G.deckCount[1];
  coinSave = G.coins;
  actionSave=G.numActions;

  MinionAction(0, &G, 0);

  testAssert((actionSave+1==G.numActions), "Action count increased by 1");
  testAssert((coinSave==G.coins), "Player's coin count the same");
  testAssert((G.handCount[1]==4), "Opponent's hand now has 4");
  testAssert((G.deckCount[1]==deckSaveP2-4), "Opponent has 4 less cards in deck");
  testAssert((G.hand[1][0]!=silver)&&(G.hand[1][1]!=silver)&&(G.hand[1][2]!=silver)&&(G.hand[1][3]!=silver), "Opponent has new cards in hand");
  testAssert((G.handCount[0]==4), "Player's hand now has 4");
  testAssert((G.deckCount[0]==deckSaveP2-4), "Player has 4 less cards in deck");
  testAssert((G.hand[0][0]!=gold)&&(G.hand[0][1]!=gold)&&(G.hand[0][2]!=gold)&&(G.hand[0][3]!=gold), "Player has new cards in hand");


  memset(&G, 23, sizeof(struct gameState));
  r = myInitializeGame(numPlayer, k, seed, &G); // initialize a new game
  G.handCount[0]=5;

  G.handCount[0]=5;
  G.handCount[1]=4;
  printf("\n\n##############################################################\n");
  printf("Minion Test Case:\nPlayer has chosen to discard their hand, other player has 4 cards in hand\n");
  printf("##############################################################\n");

  //Set first player's hand
  G.hand[0][0] = minion;
  G.supplyCount[minion]--;
  G.hand[0][1] = gold;
  G.supplyCount[gold]--;
  G.hand[0][2] = gold;
  G.supplyCount[gold]--;
  G.hand[0][3] = gold;
  G.supplyCount[gold]--;
  G.hand[0][4] = gold;
  G.supplyCount[gold]--;

  //Set second player's hand
  G.hand[1][0] = silver;
  G.supplyCount[silver]--;
  G.hand[1][1] = silver;
  G.supplyCount[silver]--;
  G.hand[1][2] = silver;
  G.supplyCount[silver]--;
  G.hand[1][3] = silver;
  G.supplyCount[silver]--;
  G.hand[1][4] = -1;

  deckSaveP1 = G.deckCount[0];
  deckSaveP2 = G.deckCount[1];
  coinSave = G.coins;
  actionSave=G.numActions;

  MinionAction(0, &G, 0);

  testAssert((actionSave+1==G.numActions), "Action count increased by 1");
  testAssert((coinSave==G.coins), "Player's coin count the same");
  testAssert((G.hand[1][0]==silver)&&(G.hand[1][1]==silver)&&(G.hand[1][2]==silver)&&(G.hand[1][3]==silver)&&(G.hand[1][4]==-1), "Other Player's hand unchanged");
  testAssert((G.deckCount[1]==deckSaveP2), "Opponent deck count untouched");
  testAssert((G.handCount[0]==4), "Player's hand now has 4");
  testAssert((G.deckCount[0]==deckSaveP2-4), "Player has 4 less cards in deck");
  testAssert((G.hand[0][0]!=gold)&&(G.hand[0][1]!=gold)&&(G.hand[0][2]!=gold)&&(G.hand[0][3]!=gold), "Player has new cards in hand");

}


int main(int argc, char *argv[]){


  testMinionCard();

  return 0;
}
