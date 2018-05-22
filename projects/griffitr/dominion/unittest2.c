#include "test_suite.h"

int main(){
	
	struct gameState state;
	int k[10] =	{adventurer, gardens, embargo, village, minion, mine,
			cutpurse, sea_hag, tribute, smithy};
	initializeGame(2, k, 1, &state);

	printf("-----------------------------------------------------------------\n");
	printf("---------- Unit Test: isGameOver --------------------------------\n");
	printf("-----------------------------------------------------------------\n");
	

	// Test 1: Gameover when Province supply = 0.
	printf("---- Testing Province Supply -----------\n");
	printf("Counting down from full supply to empty:\n");

	while (state.supplyCount[province] > 0){
		printf("Province cards left: %d..... ", state.supplyCount[province]);
		assertTrue( (isGameOver(&state) == 0), "Game Over triggered.");
		state.supplyCount[province] -= 1;
	}
	
	printf("Province supply = 0, testing for Game Over.....");
	assertTrue( (isGameOver(&state) == 1), "Game Over failed to trigger.");
	
	// Test 2: Gameover when Empty supply count = 3.
	state.supplyCount[province] = 8;

	printf("---- Testing Kingdom Card Supply: 0, 1, 2, 3, 4 Empty Supplies -------\n");
	// supplies empty = 0
	printf("0 Empty Supply Decks..... ");
	assertTrue( (isGameOver(&state) == 0), "Game Over triggered at 0 empty supplies.");
	// supplies empty = 1
	printf("1 Empty Supply Decks..... ");
	state.supplyCount[adventurer] = 0;
	assertTrue( (isGameOver(&state) == 0), "Game Over triggered at 1 empty supply.");
	// supplies empty = 2
	printf("2 Empty Supply Decks..... ");
	state.supplyCount[gardens] = 0;
	assertTrue( (isGameOver(&state) == 0), "Game Over triggered at 2 empty supplies.");
	// supplies empty = 3
	printf("3 Empty Supply Decks..... ");
	state.supplyCount[smithy] = 0;
	assertTrue( (isGameOver(&state) == 1), "Game Over failed to trigger with 3 empty supplies.");
	// supplies empty = 4
	printf("4 Empty Supply Decks..... ");
	state.supplyCount[tribute] = 0;
	assertTrue( (isGameOver(&state) == 1), "Game Over failed to trigger with 4 empty supplies.");


	return 0;
}
