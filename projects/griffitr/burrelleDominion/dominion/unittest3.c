#include "test_suite.h"

//#define NUMCARDS	  17
#define MAX		5000
#define NUMPLAYERS	   2

int main(){
	struct gameState state;
	int k[10] =			{adventurer, gardens, embargo, village, minion, mine,
					cutpurse, sea_hag, tribute, smithy};
	
	int cardsInPlay[17] =		{adventurer, gardens, embargo, village, minion, mine,
					cutpurse, sea_hag, tribute, smithy, curse, estate, duchy,
					province, copper, silver, gold};
	int invalidCard = steward;
	int numCards = sizeof(cardsInPlay) / sizeof(typeof(cardsInPlay));

	initializeGame(NUMPLAYERS, k, 1, &state);

	printf("-----------------------------------------------------------------\n");
	printf("---------- Unit Test: supplyCount -------------------------------\n");
	printf("-----------------------------------------------------------------\n");
	

	// Check valid card supply values
	printf("-- Testing values for all cards in play -----\n");
	printf("Supply Count = 10: \n");
	setKCards(10, cardsInPlay, numCards, &state);
	checkKCards(10, cardsInPlay, numCards, state);

	printf("\nSupply Count = 1: \n");
	setKCards(1, cardsInPlay, numCards, &state);
	checkKCards(1, cardsInPlay, numCards, state);

	printf("\nSupply Count = 0/Empty: \n");
	setKCards(0, cardsInPlay, numCards, &state);
	checkKCards(0, cardsInPlay, numCards, state);

	printf("\nSupply Count = %d/Max: \n", MAX);
	setKCards(MAX, cardsInPlay, numCards, &state);
	checkKCards(MAX, cardsInPlay, numCards, state);
	
	// When supply card is invalid/does not exist.
	printf("\nTesting Invalid card value............");
	assertTrue(supplyCount(invalidCard, &state) == -1, "Invalid card did not return error value.");

	return 0;
}
