#include "test_suite.h"

#define MAX		500
#define NUMPLAYERS	  2

int main(){
	int i;
	struct gameState state;
	int k[10] =			{adventurer, gardens, embargo, village, minion, mine,
					cutpurse, sea_hag, tribute, smithy};
	int player;
	int testCard = adventurer;
	int invalidCard = steward;

	initializeGame(NUMPLAYERS, k, 1, &state);

	printf("-----------------------------------------------------------------\n");
	printf("---------- Unit Test: fullDeckCount -----------------------------\n");
	printf("-----------------------------------------------------------------\n");
	

	for(i = 0; i < NUMPLAYERS; i++){
		int j;
		player = i;
		printf("TESTING PLAYER POSITION %d -----------------------------\n", i+1);

		// Player has cards but 0 instances of card in deck, hand, discard.
		printf("Searching for non-existant card in player deck....\n\t");
		assertTrue( fullDeckCount(player, testCard, &state) == 0, "Non-existant card found.");
		
		// Player has 1 instance in deck.
		printf("Searching for instance in deck....\n\t");
		addToDeck(player, testCard, &state);
		assertTrue( fullDeckCount(player, testCard, &state) == 1, "Incorrect count; expected 1 in deck.");
		state.deckCount[player]--;	
	
		// 1 instance in hand.
		printf("Searching for instance in hand....\n\t");
		addToHand(player, testCard, &state);
		assertTrue( fullDeckCount(player, testCard, &state) == 1, "Incorrect count; expected 1 in hand.");
		state.handCount[player]--;
	
		// 1 instance in discard.
		printf("Searching for instance in discard....\n\t");
		addToDiscard(player, testCard, &state);
		assertTrue( fullDeckCount(player, testCard, &state) == 1, "Incorrect count; expected 1 in discard.");
		
		// 1 instance in all.
		printf("Searching for instances in deck, hand & discard....\n\t");
		addToDeck(player, testCard, &state);
		addToHand(player, testCard, &state);
		assertTrue( fullDeckCount(player, testCard, &state) == 3, "Incorrect count; expected 3.");
	
		// 2/Multiple instances in all.
		
		// Max instances in all.
		emptyPlayerCards(player, &state);
		printf("Searching for max instances....\n\t");
		for (j = 0; j < MAX; j++){
			addToDeck(player, testCard, &state);
		}
		for (j = 0; j < MAX; j++){
			addToHand(player, testCard, &state);
		}
		for (j = 0; j < MAX; j++){
			addToDiscard(player, testCard, &state);
		}
		assertTrue( fullDeckCount(player, testCard, &state) == (MAX * 3),
			"Incorrect count; expected max value.");
	
		printf("Searching for instance of card not in play.... \n\t");
		assertTrue( fullDeckCount(player, invalidCard, &state) == 0, "Expected 0 count.");

		// Player has no cards.
		printf("Searching for instances in empty deck/hand/discard....\n\t");
		emptyPlayerCards(player, &state);
		assertTrue( fullDeckCount(player, testCard, &state) == 0, "Invalid count in empty deck." );
	}
	
	printf("TESTING INVALID PLAYER POSITION -----------------------------\n");
	printf("Searching for instance in non-existant player deck....\n\t");
	assertTrue( fullDeckCount(NUMPLAYERS, testCard, &state) == -1, "Expected error value.");
	
	return 0;
}
