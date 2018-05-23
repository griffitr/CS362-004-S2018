#include "test_suite.h"



void testInstanceVillage(	int player, int draws[], int expDraws, int expDiscards, int expPlays, struct gameState before, struct gameState after){

	//Run Relevant Tests on Smithy Card
	printf("\n\t1.  Testing Current Player's Hand/Drawn Cards\n\n");
//	testDrawnCards(player, draws, lengthDraws, before, after);
	testFullDeckCount(player, expDraws, expDiscards, expPlays, before, after);
	testCardsDrawn(player, draws, expDraws, before, after);
	printf("\n\t2.  Testing Game State ( Before vs. After )\n\n");	
	//Assert actions +2
	testActionCount(2, before, after);
	testBuyCount(0, before, after);

	assertOtherPlayerCardsUnchanged(player, before, after);
	printf("\n");
}

int main(){

	int cardToTest = village;
	printf("\n************************* TESTING: VILLAGE CARD *************************\n\n");

	//Game State Setup
	int seed = 1000;
	int numPlayers = 2;
	int player = 0;

	struct gameState gameBefore, gameAfter;
	int k[10] =	{adventurer, gardens, embargo, village, minion, mine,
			cutpurse, sea_hag, tribute, smithy};
	
	//Initialize Game
	initializeGame(numPlayers, k, seed, &gameBefore);

	//---------------- BEGIN TESTS -------------------------------------------//

	int choice1 = 0, choice2 = 0, choice3 = 0, handPos = 0, bonus = 0;
	int draws[1] = {-1};
	int expDraws = 1, expDiscards = 0, expPlays = 1;
	
	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 1 :: Full Deck | NO Shuffle Required -------------------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 5 [estate][estate][copper][silver][gold]\n");
	printf("----- Hand = 5 [village][estate][estate][estate][estate]\n");
	printf("----- Discard = 5 [estate][estate][estate][estate][estate]\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	int t1Deck[] = {estate, estate, copper, silver, gold};
	int t1Hand[] = {village, estate, estate, estate, estate};
	int t1Discard[] = {estate, estate, estate, estate, estate};
	draws[0] = gold;

	setInstance(			t1Deck, 5, t1Hand, 5, t1Discard, 5,
					player, &gameBefore);
	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceVillage(		player, draws, expDraws, expDiscards, expPlays, gameBefore, gameAfter);

	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 2 :: Partial Deck, One in Hand | No Shuffle Required ---------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 1 [gold]\n");
	printf("----- Hand = 1 [village]\n");
	printf("----- Discard = 5 [estate][estate][estate][estate][estate]\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	int t2Deck[] = {gold};
	int t2Hand[] = {village};
	int t2Discard[] = {estate, estate, estate, estate, estate};

	setInstance(			t2Deck, 1, t2Hand, 1, t2Discard, 5,
					player, &gameBefore);
	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
//	draws[0] = gameBefore.deck[player][ gameBefore.deckCount[player] ];

	testInstanceVillage(		player, draws, expDraws, expDiscards, expPlays, gameBefore, gameAfter);

	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 3 :: Empty Deck, Partial Hand | Shuffle Required -------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 0 []\n");
	printf("----- Hand = 1 [village]\n");
	printf("----- Discard = 5 [estate][estate][estate][estate][estate]\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	int t3Deck[] = {};
	int t3Hand[] = {village};
	int t3Discard[] = {estate, estate, estate, estate, estate};
	draws[0] = estate;

	setInstance(			t3Deck, 0, t3Hand, 1, t3Discard, 5,
					player, &gameBefore);
	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);

	testInstanceVillage(		player, draws, expDraws, expDiscards, expPlays, gameBefore, gameAfter);


	return 0;
}
