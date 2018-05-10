#include "test_suite.h"

int checkAdventurer(int p, int handPos, struct gameState *pre, struct gameState *post){
	int i;
	int expDrawn = 0, fails = 0;

	// Set PRE state -------------------------------------------------
	
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

/*	// Discard adventurer
	pre->discard[p][pre->discardCount[p]] = adventurer;
	pre->discardCount[p] += 1;
	pre->hand[p][handPos] = pre->hand[p][pre->handCount[p] - 1];
	pre->handCount[p] -= 1;
*/

	//Place adventurer into played cards, remove from hand.
	pre->playedCards[pre->playedCardCount] = pre->hand[p][handPos];
	pre->playedCardCount++;

	if (pre->handCount[p] == handPos + 1) { pre->handCount[p]--; } //If played card is last in hand, decrement.
	else if (pre->handCount[p] == 1) { pre->handCount[p]--; } //Or, if played card is only card in hand, decrement.
	else {
		pre->hand[p][handPos] = pre->hand[p][pre->handCount[p]-1]; //Otherwise, replace played card w/last card in hand.
		pre->handCount[p]--;
	}


	// Count relevant cards: treasures and adventurer
	int preT = 0, postT = 0;
	int preHandAdv = 0, postHandAdv = 0;
	int preDiscardAdv = 0, postDiscardAdv = 0;

	for (i = 0; i < expDrawn; i++){
		for (i = 0; i < pre->handCount[p]; i++){
			if (	pre->hand[p][i] == copper ||
				pre->hand[p][i] == silver ||
				pre->hand[p][i] == gold		) { preT++; }
			else if ( pre->hand[p][i] == adventurer ) { preHandAdv++; }
		}

		for (i = 0; i < post->handCount[p]; i++){
			if (	post->hand[p][i] == copper ||
				post->hand[p][i] == silver ||
				post->hand[p][i] == gold	) { postT++; }
			else if ( post->hand[p][i] == adventurer ) { postHandAdv++; }
		}
	}

	for(i = 0; i < pre->playedCardCount; i++){
		if (pre->playedCards[i] == adventurer) { preDiscardAdv++; }
	}

	for(i = 0; i < post->playedCardCount; i++){
		if (post->playedCards[i] == adventurer) { postDiscardAdv++; }
	}

	// Test PRE/POST States --------------------------------------------------------------------------------

//	fails += assertFail( pre->handCount[p] == post->handCount[p], "Hand count mismatch.");
	fails += assertFail(preT == postT, "Incorrect number of treasures drawn into hand.");
//	fails += assertFail( preHandAdv == postHandAdv, "Adventurer improperly discarded from hand.");
//	fails += assertFail( preDiscardAdv == postDiscardAdv, "Adventurer improperly placed into played cards.");

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
	
	for (i = 0; i < 1000; i++){
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
