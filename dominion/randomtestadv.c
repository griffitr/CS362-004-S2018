#include "test_suite.h"
#include <math.h>

int assertFail(int statement, char errMsg[]){
	if (!statement) { printf("FAILED: %s\n", errMsg); return 1; }
	return 0;
}

void randomize(int *p, int *handPos, int cardToTest, struct gameState *state){

	int i;

	for (i = 0; i < sizeof(struct gameState); i++){
		((char*)state)[i] = floor(Random() * 256);
	}

	*p = floor(Random() * 2);
	state->whoseTurn = *p;
	state->deckCount[*p] = floor(Random() * MAX_DECK);
	state->handCount[*p] = floor(Random() * MAX_DECK);
	state->discardCount[*p] = floor(Random() * MAX_DECK);

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

	//Place adventurer card in hand:
	*handPos = floor(Random() * state->handCount[*p]);
	state->hand[*p][*handPos] = cardToTest;

	//printf("Adventure card: %d| handpos: %d\n", state->hand[*p][*handPos], *handPos);
	//printf("Deck: %d, Hand: %d, Discard: %d\n", state->deckCount[*p], state->handCount[*p], state->discardCount[*p]);
}

int checkAdventurer(int p, int handPos, struct gameState *pre, struct gameState *post){
	int i;
	int expDrawn = 0, fails = 0;

	// Set PRE state -------------------------------------------------

	//If there are two treasures in the deck, no shuffle required. expDrawn = 2
	
	//If there are less than 2 treasures in deck, shuffle required - copy/paste
		//If there are less than 2 treasures in discard, expDrawn = num

		//If there are >1 treasures in discard, expDrawn = 2
	





	// Player should have at least 2 cards drawn if deck + discard > 1
	if (pre->deckCount[p] + pre->discardCount[p] > 1) {
		for (i = 0; i < 2; i++) {
			pre->hand[p][pre->handCount[p]] = copper;
			pre->handCount[p] += 1;
		}
		expDrawn = 2;
	}

	// If deck + discard == 1, player should have 1 drawn card
	else if (pre->deckCount[p] + pre->discardCount[p] == 1){
		pre->hand[p][pre->handCount[p]] = copper;
		pre->handCount[p] += 1;
		expDrawn = 1;
	}
	// If deck + discard == 0, player should have 0 drawn cards
		// DO NOTHING.

	// Discard adventurer
	pre->discard[p][pre->discardCount[p]] = adventurer;
	pre->discardCount[p] += 1;
	pre->hand[p][handPos] = pre->hand[p][pre->handCount[p] - 1];
	pre->handCount[p] -= 1;

	// Test PRE/POST States ------------------------------------------
	fails += assertFail( pre->handCount[p] == post->handCount[p], "Hand count mismatch.");

	// Drawn cards should be treasures
	int preT = 0, postT = 0;
	int preAdv = 0, postAdv = 0;

	for (i = 0; i < expDrawn; i++){
		for (i = 0; i < pre->handCount[p]; i++){
			if (	pre->hand[p][i] == copper ||
				pre->hand[p][i] == silver ||
				pre->hand[p][i] == gold		) { preT++; }
			else if ( pre->hand[p][i] == adventurer ) { preAdv++; }
		}

		for (i = 0; i < post->handCount[p]; i++){
			if (	post->hand[p][i] == copper ||
				post->hand[p][i] == silver ||
				post->hand[p][i] == gold	) { postT++; }
			else if ( post->hand[p][i] == adventurer ) {postAdv++; }
		}
	}

	fails += assertFail(preT == postT, "Incorrect number of treasures drawn into hand.");
	
	// Adventurer card should be discarded
//	fails += assertFail( post->discard[p][post->discardCount[p] - 1] == adventurer, "Adventurer not on top of discard.");
	fails += assertFail( preAdv == postAdv, "Adventurer improperly discarded from hand.");

	return fails;
}

int main(){
	int seed = 3;
	int failed = 0, passed = 0;

	//Create randomized game state
	struct gameState pre, post;
	int p, bonus;
	int handPos = 0;

	int i;

	SelectStream(2);
	PutSeed(seed);
	
	for (i = 0; i < 2000; i++){
		int result = 0;

		//Create random gamestate
		randomize(&p, &handPos, adventurer, &pre);
		//Test game state
		memcpy( &post, &pre, sizeof(struct gameState ));
		cardEffect(adventurer, 0, 0, 0, &post, handPos, &bonus);

		result = checkAdventurer(p, handPos, &pre, &post);
	
		//If test fails, print seed and iteration
		if (result) { printf("Test Instance FAILED %d tests. Seed: %d | Iteration: %d\n\n", result, seed, i); failed++; }
		else { passed++; }
	}

	printf("Test Summary: \tPassed = %d | Failed = %d\n", passed, failed);
	
	return 0;
}

