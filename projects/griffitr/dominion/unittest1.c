#include "test_suite.h"

#define NUMCARDS	27

int main(){
	int i;

	// List of game card enums currently represented.
	int allGameCards[NUMCARDS] =	{curse, estate, duchy, province, copper, silver, gold,
					adventurer, council_room, feast, gardens, mine, remodel,
					smithy, village, baron, great_hall, minion, steward,
					tribute, ambassador, cutpurse, embargo, outpost, salvager,
					sea_hag, treasure_map};

	// List of card costs created from www.wiki.dominionstrategy.com.
	int cardCosts[NUMCARDS] =	{0,	// curse
					2,	// estate
					5,	// duchy
					8,	// province
					0,	// copper
					3,	// silver
					6,	// gold
					6,	// adventurer
					5,	// council room
					4,	// feast
					4,	// gardens
					5,	// mine
					4,	// remodel
					4,	// smithy
					3,	// village
					4,	// baron
					3,	// great hall
					5,	// minion
					3,	// steward
					5,	// tribute
					3,	// ambassador
					4,	// cutpurse
					2,	// embargo
					5,	// outpost
					4,	// salvager
					4,	// sea hag
					4};	// treasure map
	
	printf("-----------------------------------------------------------------\n");
	printf("---------- Unit Test: GetCost -----------------------------------\n");
	printf("-----------------------------------------------------------------\n");
	
	// Test cost values for all known game cards.
	for (i = 0; i < NUMCARDS; i++){
		printf("Testing card #%d..... ", i+1);
		assertTrue( (getCost(allGameCards[i])  == cardCosts[i]), "Incorrect cost.");
		printf("  test cost = %d | true cost = %d\n", getCost(allGameCards[i]), cardCosts[i]);
	}

	// Test handling of false card value = -1
	printf("Testing false card..... ");
	assertTrue( (getCost(-1) == -1), "Expected error value.");
	printf("\n");

	return 0;
}
