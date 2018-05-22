#include "test_suite.h"

void assertPlayerCardsUnchanged(int player, struct gameState before, struct gameState after){
	int i;

	printf("\tAnalyzing cards in player deck...\n");
	for (i = 0; i < before.deckCount[player]; i++){
		printf("Card #%d..... ", i+1);
		assertTrue(before.deck[player][i] == after.deck[player][i], "Wrong card!");
	}

	printf("\tAnalyzing cards in player hand...\n");
	for (i = 0; i < before.handCount[player]; i++){
		printf("Card #%d..... ", i+1);
		assertTrue(before.hand[player][i] == after.hand[player][i], "Wrong card!");
	}

	printf("\tAnalyzing cards in player discard...\n");
	for (i = 0; i < before.discardCount[player]; i++){
		printf("Card #%d..... ", i+1);
		assertTrue(before.discard[player][i] == after.discard[player][i], "Wrong card!");
	}
}

void testInstanceSeaHag(int player, int curseSupply, struct gameState before, struct gameState after){
	int i;

	//Run Relevant Tests on Sea Hag Card
	printf("\n\t1.  Testing Current Player's Cards\n\n");
	testFullDeckCount(player, 0, 0, 1, before, after);

	printf("\n\t2.  Testing Game State ( Before vs. After )\n\n");	
	int cursesBefore = supplyCount(curse, &before);
	int cursesAfter = supplyCount(curse, &after);

	// If there are enough curses for all players...
	if (cursesBefore >= before.numPlayers) {
		printf("Curse supply count: Expected: %d | Actual: %d ... ", cursesBefore - before.numPlayers + 1, cursesAfter);
		assertTrue(cursesBefore - before.numPlayers + 1 == cursesAfter, "");
	}
	// If there are not enough curses...
	else {
		printf("Curse supply count: Expected: %d | Actual: %d ... ", 0, cursesAfter);
		assertTrue(0 == cursesAfter, "");
	}

	int cursesRemaining = cursesBefore;
	//Assert correct deck/hand/discard numbers for all other players
	for (i = 0; i < before.numPlayers; i++){
		if (i == player) { continue; }
	
		// CHECK PLAYER CARD COUNTS...	
		printf("\n\t Checking Player %d Card Counts\n", i+1);
		//If there were enough curse cards for this player...
		if (cursesRemaining > 0){
			printf("Deck count: Expected: %d | Actual: %d ... ", before.deckCount[i], after.deckCount[i]);
			assertTrue(before.deckCount[i] == after.deckCount[i], "Deck count should not change.");
		}	
		else {
			printf("Deck count: Expected: %d | Actual: %d ... ", before.deckCount[i]-1, after.deckCount[i]);
			assertTrue(before.deckCount[i]-1 == after.deckCount[i], "Deck count should decrease by 1.");	
		}
		printf("Hand count: Expected: %d | Actual: %d ... ", before.handCount[i], after.handCount[i]);
		assertTrue(before.handCount[i] == after.handCount[i], "Hand count should not change.");

		printf("Discard count: Expected: %d | Actual: %d ... ", before.discardCount[i]+1, after.discardCount[i]);
		assertTrue(before.discardCount[i]+1 == after.discardCount[i], "Discard count should increase by 1.");

		// CHECK INDIVIDUAL CARDS

		// Check deck top for curse or second card
		int prevDeckTop;
		int lowerDeckTop;
		int postDeckTop;
		int postDiscardTop;

		if (before.deckCount[i] > 0) { prevDeckTop = before.deck[i][before.deckCount[i] - 1]; }
		else { prevDeckTop = -1; }
		if (before.deckCount[i] > 1) { lowerDeckTop = before.deck[i][before.deckCount[i] - 2]; }
		else { lowerDeckTop = -1; }
		if (after.deckCount[i] > 0) { postDeckTop = after.deck[i][after.deckCount[i] - 1]; }
		else { postDeckTop = -1; }
		if (after.discardCount[i] > 0) { postDiscardTop = after.discard[i][after.discardCount[i] - 1]; }
		else { postDiscardTop = -1; }

		if (cursesRemaining > 0){
			printf("Checking deck top for curse: Expected: %d | Actual: %d ... ", curse, postDeckTop);
			assertTrue(curse == postDeckTop, "Expected curse card.");
			cursesRemaining--;
		}
		else {
			printf("Checking deck top for second card: Expected: %d | Actual: %d ... ", lowerDeckTop, postDeckTop);
			assertTrue(lowerDeckTop == postDeckTop, "Expected card beneath prev deck top.");	
		}

		// Check discard top for old deck top
		printf("Checking discard top for prev deck top: Expected: %d | Actual: %d ... ", prevDeckTop, postDiscardTop);
		assertTrue(prevDeckTop == postDiscardTop, "Expected prev deck top.");
	}

	printf("\n");
}

int main(){

	int cardToTest = sea_hag;
	printf("\n************************* TESTING: SEA_HAG CARD *************************\n\n");

	//Game State Setup
	int seed = 1000;
	int numPlayers = 4;
	int player = 0;

	struct gameState gameBefore, gameAfter;
	int k[10] =	{adventurer, gardens, embargo, village, minion, mine,
			cutpurse, sea_hag, tribute, smithy};
	
	//Initialize Game
	initializeGame(numPlayers, k, seed, &gameBefore);

	//---------------- BEGIN TESTS -------------------------------------------//

	int i;
	int choice1 = 0, choice2 = 0, choice3 = 0, handPos = 0, bonus = 0;
	int curseSupply;
	
	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 1 :: Full Curses Deck | All players receive curse ------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 5 [estate][duchy][province][copper][silver]\n");
	printf("----- Hand = 5 [sea_hag][gold][adventurer][council_room][feast]\n");
	printf("----- Discard = 5 [gardens][mine][remodel][smithy][village]\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	int t1Deck[] = {estate, duchy, province, copper, silver};
	int t1Hand[] = {sea_hag, gold, adventurer, council_room, feast};
	int t1Discard[] = {gardens, mine, remodel, smithy, village};
	curseSupply = 8;

	//Set card test instances for each player.
	for (i = 0; i < numPlayers; i++){
		setInstance(			t1Deck, 5, t1Hand, 5, t1Discard, 5,
						i, &gameBefore);
	}
	//Set curse card supply.
	gameBefore.supplyCount[curse] = curseSupply;

	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceSeaHag(		player, curseSupply, gameBefore, gameAfter);

	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 2 :: Partial Curses Deck | One player receives curse ---------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 5 [estate][duchy][province][copper][silver]\n");
	printf("----- Hand = 5 [sea_hag][gold][adventurer][council_room][feast]\n");
	printf("----- Discard = 5 [gardens][mine][remodel][smithy][village]\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	curseSupply = 1;

	//Set card test instances for each player.
	for (i = 0; i < numPlayers; i++){
		setInstance(			t1Deck, 5, t1Hand, 5, t1Discard, 5,
						i, &gameBefore);
	}
	//Set curse card supply.
	gameBefore.supplyCount[curse] = curseSupply;

	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceSeaHag(		player, curseSupply, gameBefore, gameAfter);

	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 3 :: Empty Curses Deck | No players receive curse ------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 5 [estate][duchy][province][copper][silver]\n");
	printf("----- Hand = 5 [sea_hag][gold][adventurer][council_room][feast]\n");
	printf("----- Discard = 5 [gardens][mine][remodel][smithy][village]\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	curseSupply = 0;

	//Set card test instances for each player.
	for (i = 0; i < numPlayers; i++){
		setInstance(			t1Deck, 5, t1Hand, 5, t1Discard, 5,
						i, &gameBefore);
	}
	//Set curse card supply.
	gameBefore.supplyCount[curse] = curseSupply;

	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceSeaHag(		player, curseSupply, gameBefore, gameAfter);

	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 4 :: One Card in Decks, No Discards | 0 Curse ----------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 1 [estate]\n");
	printf("----- Hand = 5 [sea_hag][gold][adventurer][council_room][feast]\n");
	printf("----- Discard = 0 []\n");
	printf("-------------------------------------------------------------------------\n\n");
	
	int t3Deck[] = {estate};
	int t3Discard[] = {};
	curseSupply = 0;

	//Set card test instances for each player.
	for (i = 0; i < numPlayers; i++){
		setInstance(			t3Deck, 1, t1Hand, 5, t3Discard, 0,
						i, &gameBefore);
	}
	//Set curse card supply.
	gameBefore.supplyCount[curse] = curseSupply;

	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceSeaHag(		player, curseSupply, gameBefore, gameAfter);

	printf("\n-------------------------------------------------------------------------\n");
	printf("----- TEST 5 :: Empty Decks | Full Curse Supply = 3 ---------------------\n");
	printf("-----                                                               -----\n");
	printf("----- Deck = 5 []\n");
	printf("----- Hand = 5 [sea_hag][gold][adventurer][council_room][feast]\n");
	printf("----- Discard = 1 [gardens]\n");
	printf("-------------------------------------------------------------------------\n\n");

	int t4Deck[] = {};
	int t4Discard[] = {gardens};
	curseSupply = 3;

	//Set card test instances for each player.
	for (i = 0; i < numPlayers; i++){
		setInstance(			t4Deck, 0, t1Hand, 5, t4Discard, 1,
						i, &gameBefore);
	}
	//Set curse card supply.
	gameBefore.supplyCount[curse] = curseSupply;

	runInstance(			choice1, choice2, choice3, handPos, &bonus, cardToTest, &gameBefore, &gameAfter);
	testInstanceSeaHag(		player, curseSupply, gameBefore, gameAfter);



	return 0;
}
