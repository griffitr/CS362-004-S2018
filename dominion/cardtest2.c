#include "test_suite.h"


void testInstanceSmithy(	int player, int draws[], int expDraws, int expDiscards, int expPlayed, struct gameState before, struct gameState after){

	//Run Relevant Tests on Smithy Card
	printf("\n\t1.  Testing Current Player's Hand/Drawn Cards\n\n");
//	testDrawnCards(player, draws, lengthDraws, before, after);
	testFullDeckCount(player, expDraws, expDiscards, expPlayed, before, after);
	testCardsDrawn(player, draws, expDraws, before, after);

	printf("\n\t2.  Testing Game State ( Before vs. After )\n\n");	
	
	testActionCount(0, before, after);
	testBuyCount(0, before, after);

	assertOtherPlayerCardsUnchanged(player, before, after);
	
//	testState(player, before, after);
	printf("\n");
}

int main(){

	int cardToTest = smithy;
	printf("\n************************* TESTING: SMITHY CARD *************************\n\n");

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
	int draws[3] = {-1, -1, -1};
	int expDraws = 0, expDiscards = 0, expPlayed = 1;
	
	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 1 :: Full Deck | NO Shuffle Required -------------------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 5 [estate][estate][copper][silver][gold]\n");
	printf("----- Hand = 5 [smithy][estate][estate][estate][estate]\n");
	printf("----- Discard = 5 [estate][estate][estate][estate][estate]\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	int t1Deck[] = {estate, estate, copper, silver, gold};
	int t1Hand[] = {smithy, estate, estate, estate, estate};
	int t1Discard[] = {estate, estate, estate, estate, estate};
	draws[0] = gold;
	draws[1] = silver;
	draws[2] = copper;
	expDraws = 3;

	setInstance(			t1Deck, 5, t1Hand, 5, t1Discard, 5,
					player, &gameBefore);
	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceSmithy(		player, draws, expDraws, expDiscards, expPlayed, gameBefore, gameAfter);

	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 2 :: Partial Deck | Shuffle Required -------------------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 1 [copper]\n");
	printf("----- Hand = 5 [smithy][estate][estate][estate][estate]\n");
	printf("----- Discard = 2 [silver][gold]\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	int t2Deck[] = {copper};
	int t2Hand[] = {smithy, estate, estate, estate, estate};
	int t2Discard[] = {silver, gold};
	draws[0] = copper;
	draws[1] = silver;
	draws[2] = gold;

	setInstance(			t2Deck, 1, t2Hand, 5, t2Discard, 2,
					player, &gameBefore);
	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceSmithy(		player, draws, expDraws, expDiscards, expPlayed, gameBefore, gameAfter);

	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 3 :: Empty Deck | Shuffle Required -------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 0 []\n");
	printf("----- Hand = 5 [smithy][estate][estate][estate][estate]\n");
	printf("----- Discard = 1 [copper]\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	int t3Deck[] = {};
	int t3Hand[] = {smithy, estate, estate, estate, estate};
	int t3Discard[] = {copper};
	draws[0] = copper;
	draws[1] = -1;
	draws[2] = -1;
	expDraws = 1;

	setInstance(			t3Deck, 0, t3Hand, 5, t3Discard, 1,
					player, &gameBefore);
	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceSmithy(		player, draws, expDraws, expDiscards, expPlayed, gameBefore, gameAfter);

	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 4 :: Empty Deck/Discard | No Draws ---------------------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 0 []\n");
	printf("----- Hand = 5 [smithy][estate][estate][estate][estate]\n");
	printf("----- Discard = 0 []\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	int t4Deck[] = {};
	int t4Hand[] = {smithy, estate, estate, estate, estate};
	int t4Discard[] = {};
	draws[0] = -1;
	draws[1] = -1;
	draws[2] = -1;
	expDraws = 0;

	setInstance(			t4Deck, 0, t4Hand, 5, t4Discard, 0,
					player, &gameBefore);
	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceSmithy(		player, draws, expDraws, expDiscards, expPlayed, gameBefore, gameAfter);

	return 0;
}
