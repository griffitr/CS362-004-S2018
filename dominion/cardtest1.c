#include "test_suite.h"


void testPlayerHand(int player, int expectedTreasure, struct gameState before, struct gameState after){
	int i;
	int result;

	//Make sure players new hand holds the correct amount of cards.
	int handCountBefore =  before.handCount[player];
	int handCountAfter = after.handCount[player];

	printf("Prev Hand Count = %d || Post Hand Count = %d\n", handCountBefore, handCountAfter);
	printf("Expected: %d ....... ", (handCountBefore + expectedTreasure));
	result = assertTrue(	handCountBefore + expectedTreasure == handCountAfter,
				"Incorrect hand count after card played." );

	if (result == 0) {
		printf("\n\tSkipping Player-Hand Analysis due to incorrect hand count...\n\n");
		return;
	}

	printf("\nAnalyzing Player's New Hand... Expecting %d new treasure cards...\n", expectedTreasure);
	//Make sure player's new hand holds <expectedTreasure> more treasure cards than before.
	for (i = 0; i < expectedTreasure; i++){

		printf("\tNew Card %d is treasure?  ", i+1);
		int testCard = ( after.hand[player][( (before.handCount[player]) + i )] );
		
		assertTrue(	(testCard == copper ||
				testCard == silver ||
				testCard == gold),
				"Expected treasure card (copper/silver/gold)." );
	}
	return;
}

void testInstanceAdventurer(	int player, int expectedTreasure, int totalTreasure,
				struct gameState before, struct gameState after){

	//Run Relevant Adventurer Tests
	printf("\n\t1.  Testing Current Player's Hand\n\n");
	testPlayerHand(player, expectedTreasure, before, after);
	printf("\n\t2.  Testing Game State ( Before vs. After )\n\n");	

	testActionCount(0, before, after);
	testBuyCount(0, before, after);

	assertOtherPlayerCardsUnchanged(player, before, after);
	
	printf("\n");
}

int main(){

	int cardToTest = adventurer;
	printf("\n************************* TESTING: ADVENTURE CARD ***********************\n\n");


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
	int expTreasure;
	int totalTreasure;

	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 1 :: 2 Treasures in Deck | NO Shuffle Required ---------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 5 [adventurer][gold][copper][province][estate]\n");
	printf("----- Hand = 5 [adventurer][estate][estate][estate][estate]\n");
	printf("----- Discard = 5 [estate][estate][estate][estate][estate]\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	int t1Deck[] = {adventurer, gold, copper, province, estate};
	int t1Hand[] = {adventurer, estate, estate, estate, estate};
	int t1Discard[] = {estate, estate, estate, estate, estate};
	expTreasure = 2;
	totalTreasure = 2;

	setInstance(			t1Deck, 5, t1Hand, 5, t1Discard, 5,
					player, &gameBefore);
	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceAdventurer(		player, expTreasure, totalTreasure, gameBefore, gameAfter);

	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 2 :: 3 Treasures in Deck | NO Shuffle Required ---------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 5 [adventurer][gold][copper][silver][estate]\n");
	printf("----- Hand = 5 [adventurer][estate][estate][estate][estate]\n");
	printf("----- Discard = 5 [estate][estate][estate][estate][estate]\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	int t2Deck[] = {adventurer, gold, copper, silver, estate};
	int t2Hand[] = {adventurer, estate, estate, estate, estate};
	int t2Discard[] = {estate, estate, estate, estate, estate};
	expTreasure = 2;
	totalTreasure = 3;

	setInstance(			t2Deck, 5, t2Hand, 5, t2Discard, 5,
					player, &gameBefore);
	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceAdventurer(		player, expTreasure, totalTreasure, gameBefore, gameAfter);

	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 3 :: 3 Treasures in Deck | Shuffle Required ------------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 5 [silver][estate][estate][estate][estate]\n");
	printf("----- Hand = 5 [adventurer][estate][estate][estate][estate]\n");
	printf("----- Discard = 5 [estate][estate][copper][estate][gold]\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	int t3Deck[] = {silver, estate, estate, estate, estate};
	int t3Hand[] = {adventurer, estate, estate, estate, estate};
	int t3Discard[] = {estate, estate, copper, estate, gold};
	expTreasure = 2;
	totalTreasure = 3;

	setInstance(			t3Deck, 5, t3Hand, 5, t3Discard, 5,
					player, &gameBefore);
	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceAdventurer(		player, expTreasure, totalTreasure, gameBefore, gameAfter);

	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 4 :: 1 Treasure in Deck | Shuffle Required ------------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 5 [estate][estate][estate][estate][estate]\n");
	printf("----- Hand = 5 [adventurer][estate][estate][estate][estate]\n");
	printf("----- Discard = 5 [estate][estate][estate][estate][gold]\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	int t4Deck[] = {estate, estate, estate, estate, estate};
	int t4Hand[] = {adventurer, estate, estate, estate, estate};
	int t4Discard[] = {estate, estate, estate, estate, gold};
	expTreasure = 1;
	totalTreasure = 1;

	setInstance(			t4Deck, 5, t4Hand, 5, t4Discard, 5,
					player, &gameBefore);
	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceAdventurer(		player, expTreasure, totalTreasure, gameBefore, gameAfter);

	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 5 :: 0 Treasures in Deck | Shuffle Required ------------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 5 [estate][estate][estate][estate][estate]\n");
	printf("----- Hand = 5 [adventurer][estate][estate][estate][estate]\n");
	printf("----- Discard = 5 [estate][estate][estate][estate][estate]\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	int t5Deck[] = {estate, estate, estate, estate, estate};
	int t5Hand[] = {adventurer, estate, estate, estate, estate};
	int t5Discard[] = {estate, estate, estate, estate, estate};
	expTreasure = 0;
	totalTreasure = 0;

	setInstance(			t5Deck, 5, t5Hand, 5, t5Discard, 5,
					player, &gameBefore);
	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceAdventurer(		player, expTreasure, totalTreasure, gameBefore, gameAfter);

	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 6 :: 0 Treasures in Deck | Empty Discard ---------------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 5 [estate][estate][estate][estate][estate]\n");
	printf("----- Hand = 5 [adventurer][estate][estate][estate][estate]\n");
	printf("----- Discard = 0 --\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	int t6Deck[] = {estate, estate, estate, estate, estate};
	int t6Hand[] = {adventurer, estate, estate, estate, estate};
	int t6Discard[] = {};
	expTreasure = 1;
	totalTreasure = 1;

	setInstance(			t6Deck, 5, t6Hand, 5, t6Discard, 5,
					player, &gameBefore);
	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceAdventurer(		player, expTreasure, totalTreasure, gameBefore, gameAfter);

	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 7 :: 0 Treasures | Empty Deck/Discard ------------------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 0 --\n");
	printf("----- Hand = 5 [adventurer][estate][estate][estate][estate]\n");
	printf("----- Discard = 0 --\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	int t7Deck[] = {};
	int t7Hand[] = {adventurer, estate, estate, estate, estate};
	int t7Discard[] = {};
	expTreasure = 0;
	totalTreasure = 0;

	setInstance(			t7Deck, 5, t7Hand, 5, t7Discard, 5,
					player, &gameBefore);
	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceAdventurer(		player, expTreasure, totalTreasure, gameBefore, gameAfter);

	
	return 0;
}
