#include "test_suite.h"

//----- Non-Abortive Assertion ----------------------------------------//
int assertTrue(int statement, char errormsg[]){
	if (!statement) { printf("FAILED. %s\n", errormsg); return 0; }
	printf("PASSED.\n");
	return 1;
}

//----- Set / Check Supply Counts for Kingdom Cards in Play -----------------------------//
void setKCards(int count, int cards[], int numKCards, struct gameState *state){
	int i;
	for (i = 0; i < numKCards; i++){
		state->supplyCount[cards[i]] = count;
	}
}

void checkKCards(int count, int cards[], int numKCards, struct gameState state){
	int i;
	for (i = 0; i < numKCards; i++){
		printf("test count = %d | expected: %d....", supplyCount(cards[i], &state), count);
		assertTrue(supplyCount( cards[i], &state) == count, "Supply count incorrect.");
	}
}

//----- Direct DECK / HAND / DISCARD Manipulation ---------------------//
void addToDeck(int player, int card, struct gameState *state){
	state->deck[player][state->deckCount[player]] = card;
	state->deckCount[player]++;
}

void addToHand(int player, int card, struct gameState *state){
	state->hand[player][state->handCount[player]] = card;
	state->handCount[player]++;
}

void addToDiscard(int player, int card, struct gameState *state){
	state->discard[player][state->discardCount[player]] = card;
	state->discardCount[player]++;
}

void emptyPlayerCards(int player, struct gameState *state){
	state->deckCount[player] = 0;
	state->handCount[player] = 0;
	state->discardCount[player] = 0;
}


//----- CARD EFFECT TEST FUNCTIONS ------------------------------------------------//

// ----- SET INSTANCE -------------------------------------------------------------//
// Set an instance of dominion for card effect testing purposes.
// Allows direct setting of:
// 	Current player's deck/hand/discard cards
// 	Desired Card Effect to test
//
// --------------------------------------------------------------------------------//
void setInstance(		int deckArr[], int deckSize,
				int handArr[], int handSize,
				int discardArr[], int discardSize,
				int player, struct gameState *before){

	int i;

	//Setup Game
	emptyPlayerCards(player, before);
	//Add cards to player deck
	for (i = 0; i < deckSize; i++){
		addToDeck(player, deckArr[i], before);
	}
	//Add cards to player hand
	for (i = 0; i < handSize; i++){
		addToHand(player, handArr[i], before);
	}
	//Add cards to player discard
	for (i = 0; i < discardSize; i++){
		addToDiscard(player, discardArr[i], before);
	}


	return;
}

// ----- RUN INSTANCE -------------------------------------------------------------//
// Run card effect on a set test instance.
//---------------------------------------------------------------------------------//
void runInstance(	int choice1, int choice2, int choice3, int handPos, int *bonus,
			int cardToTest, struct gameState *before, struct gameState *after){
	
	//Copy Game State and run Play Card
	memcpy( after, before, sizeof(struct gameState) );
	cardEffect(cardToTest, choice1, choice2, choice3, after, handPos, bonus);
}


// Determines whether or not the current buy count is correct.
void testBuyCount(int expectedDif, struct gameState before, struct gameState after){
	printf("Checking buy count.....\nExpected %d | Actual: %d ... ", before.numBuys + expectedDif, after.numBuys);
	assertTrue( before.numBuys + expectedDif == after.numBuys, "Wrong buy count.");
}

// Determines whether or not the current action count is correct.
void testActionCount(int expectedDif, struct gameState before, struct gameState after){
	printf("Checking action count.....\nExpected %d | Actual: %d ... ", before.numActions + expectedDif, after.numActions);
	assertTrue( before.numActions + expectedDif == after.numActions, "Wrong action count.");
}

// Determines whether or not the full decks of any other players (besides the current one) have been modified,
// with the expection that they have not.
void assertOtherPlayerCardsUnchanged(int currentPlayer, struct gameState before, struct gameState after ){
	int i,j;

	printf("\nInspecting Deck/Hand/Discard cards of other players.\n");
	//Make sure other player hands/decks are not altered.
	for(i = 0; i < before.numPlayers; i++){
		if ( i != currentPlayer ){ // For all players besides current...
			int passed = 0;

			printf("\tPlayer %d hand..... ", i+1);
			passed += assertTrue( before.handCount[i] == after.handCount[i], "Altered hand count.");
			printf("\tPlayer %d deck..... ", i+1);
			passed += assertTrue( before.deckCount[i] == after.deckCount[i], "Altered deck count.");
			printf("\tPlayer %d discard..... ", i+1);
			passed += assertTrue( before.discardCount[i] == after.discardCount[i], "Altered discard count.");

			if ( passed != 3 ) { printf("\n\tSkipping Card Analysis due to incorrect card count(s)...\n\n");
				return;
			}

			printf("\nAnalyzing Player %d's Cards...\n", i+1);
			printf(	"handCount = %d, deckCount = %d, discardCount = %d\n", 
				before.handCount[i], before.deckCount[i], before.discardCount[i]);
			for (j = 0; j < after.handCount[i]; j++){ // For all cards in player i's hand...
				printf("Hand: Card #%d..... ", j+1);
				assertTrue( before.hand[i][j] == after.hand[i][j], "Wrong card.");
			}
			for (j = 0; j < after.deckCount[i]; j++){ // For all cards in player i's deck...
				printf("Deck: Card #%d..... ", j+1);
				assertTrue( before.deck[i][j] == after.deck[i][j], "Wrong card.");
			}
			for (j = 0; j < after.discardCount[i]; j++){
				printf("Discard: Card #%d..... ", j+1);
				assertTrue( before.discard[i][j] == after.discard[i][j], "Wrong card.");	
			}
		}
	}
}


//----- TEST CARDS DRAWN ----------------------------------------------------------//
// Looks at player's hand for instances of each card listed within the draws array.
// If any instance is found, the assert is true.
// ** Will not account for multiple cards, ie. if copper is included twice within
// the draw array, this function will assert true even if there is only one instance
// of copper in the hand.
// --------------------------------------------------------------------------------//
void testCardsDrawn(		int player, int draws[], int expectedDraws,
				struct gameState before, struct gameState after){
	int i, j;

	//Verify that cards in draw[] exist in player's hand.
	for (i = 0; i < expectedDraws; i++){
		printf("Looking for expected draw #%d..... ", i+1);
		int count = 0;
		for (j = 0; j < after.handCount[player]; j++){
			if ( after.hand[player][j] == draws[i] ) { count++; }
		}
		assertTrue( count > 0, "Draw card not found in player hand.");
	}
}

//----- TEST FULL DECK COUNT ------------------------------------------------------//
// Asserts that the before/after states of a player's full deck [hand, deck, discard]
// and the current played card count conforms to expected draw, discard and played
// card counts.
//---------------------------------------------------------------------------------//
void testFullDeckCount(int player, int expectedDraws, int expectedDiscards, int expectedPlayed, struct gameState before, struct gameState after){
	printf("Testing Player %d hand count:", player + 1);
	printf("Expected hand count:    %d  | Actual: %d..... ",	before.handCount[player]
									+ expectedDraws
									- expectedDiscards
									- expectedPlayed,
									after.handCount[player]);
	assertTrue(before.handCount[player] + expectedDraws - expectedDiscards - expectedPlayed == after.handCount[player], "Incorrect hand count.");

	if (before.deckCount[player] < expectedDraws) {
		printf("Testing Player %d deck count:", player + 1);
		printf("Expected deck count:    %d  | Actual: %d..... ",	before.deckCount[player]
										+ before.discardCount[player]
										- expectedDraws,	
										after.deckCount[player]);
		assertTrue(before.deckCount[player] + before.discardCount[player] - expectedDraws
			== after.deckCount[player], "Incorrect deck count.");

		printf("Testing Player %d discard count:", player + 1);
		printf("Expected discard count: %d  | Actual: %d..... ", expectedDiscards, after.discardCount[player]);
		assertTrue(expectedDiscards  == after.discardCount[player], "Incorrect discard count.");
	}
	else {
		printf("Testing Player %d deck count:", player + 1);
		printf("Expected deck count:    %d  | Actual: %d..... ", before.deckCount[player] - expectedDraws, after.deckCount[player]);
		assertTrue(before.deckCount[player] - expectedDraws == after.deckCount[player], "Incorrect deck count.");

		printf("Testing Player %d discard count:", player + 1);	
		printf("Expected discard count: %d  | Actual: %d..... ", before.discardCount[player] + expectedDiscards, after.discardCount[player]);
		assertTrue(before.discardCount[player] + expectedDiscards  == after.discardCount[player], "Incorrect discard count.");
	}
	if (expectedPlayed > 0){
		printf("Testing Played Cards:");
		printf("Expected played count: %d  | Actual: %d..... ", before.playedCardCount + expectedPlayed, after.playedCardCount);
		assertTrue(before.playedCardCount + expectedPlayed == after.playedCardCount, "Incorrect played count.");
	}
}

// Random Testing ------------------------------- //

int assertFail(int statement, char errMsg[]){
	if (!statement) { printf("FAILED: %s\n", errMsg); return 1; }
	return 0;
}

void randomize(int *p, int *handPos, int cardToTest, struct gameState *state){

	int i;

	//Random bit assignment
	for (i = 0; i < sizeof(struct gameState); i++){
		((char*)state)[i] = floor(Random() * 256);
	}

	*p = floor(Random() * 2);
	state->whoseTurn = *p;
	state->deckCount[*p] = floor(Random() * MAX_DECK);
	state->handCount[*p] = floor(Random() * MAX_DECK);
	state->discardCount[*p] = floor(Random() * MAX_DECK);
	state->playedCardCount = floor(Random() * MAX_DECK);

	//Assign real cards to deck
	//Deck:
	for (i = 0; i < state->deckCount[*p]; i++){
		state->deck[*p][i] = floor(Random() * 27);
	//	printf("deck card %d: %d\n", i+1, state->deck[*p][i]);
	}

	//Hand:
	for (i = 0; i < state->handCount[*p]; i++){
		state->hand[*p][i] = floor(Random() * 27);
	//	printf("hand card %d: %d\n", i+1, state->hand[*p][i]);
	
	}

	//Discard:
	for (i = 0; i < state->discardCount[*p]; i++){
		state->discard[*p][i] = floor(Random() * 27);
	}
	
	//Played:
	for (i = 0; i < state->playedCardCount; i++){
		state->playedCards[i] = floor(Random() * 27);
	}

	//Place test card in hand:
	*handPos = floor(Random() * state->handCount[*p]);
	state->hand[*p][*handPos] = cardToTest;

	//printf("Adventure card: %d| handpos: %d\n", state->hand[*p][*handPos], *handPos);
	//printf("Deck: %d, Hand: %d, Discard: %d\n", state->deckCount[*p], state->handCount[*p], state->discardCount[*p]);
}
