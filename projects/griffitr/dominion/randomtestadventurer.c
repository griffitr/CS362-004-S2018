#include "test_suite.h"

int checkAdventurer(int p, int handPos, struct gameState *pre, struct gameState *post){
	int i;
	int expDrawn = 0, fails = 0;

	for (i = 0; i < pre->deckCount[p]; i++){
		if (	pre->deck[p][i] == copper ||
			pre->deck[p][i] == silver ||
			pre->deck[p][i] == gold	)	{ expDrawn++; }
	}
	for (i = 0; i < pre->discardCount[p]; i++){
		if (	pre->discard[p][i] == copper ||
			pre->discard[p][i] == silver ||
			pre->discard[p][i] == gold )	{ expDrawn++; }
	}

	if (expDrawn > 1) { expDrawn = 2; }



	int drawnT = 0;
	int tempDraws[MAX_DECK];
	int idx = -1;

	while (drawnT < expDrawn){

		//Draw 1 card
		//If pre->deck > 0... take top card from deck, place at end of hand
		if (pre->deckCount[p] > 0){
			if (	pre->deck[p][pre->deckCount[p]-1] == copper ||
				pre->deck[p][pre->deckCount[p]-1] == silver ||
				pre->deck[p][pre->deckCount[p]-1] == gold	)
			{
				pre->hand[p][pre->handCount[p]] = pre->deck[p][pre->deckCount[p]-1];
				pre->deckCount[p]--;
				pre->handCount[p]++;
				
				drawnT++;
			}
			else
			{
				idx++;
				tempDraws[idx] = pre->deck[p][pre->deckCount[p]-1];
				pre->deckCount[p]--;
			}
		}
		
		//Else, if pre->discard > 0... : copy discard into deck and set discard to 0.
		else if (pre->discardCount[p] > 0){
			memcpy(pre->deck[p], pre->discard[p], sizeof(int) * pre->discardCount[p]);
			pre->deckCount[p] = pre->discardCount[p];
			pre->discardCount[p] = 0;
		}
	}

	//Put unused drawn cards into discard.
	while (idx > -1){
		pre->discard[p][pre->discardCount[p]] = tempDraws[idx];
		pre->discardCount[p]++;
		idx--;
	}

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

	for(i = 0; i < pre->playedCardCount; i++){
		if (pre->playedCards[i] == adventurer) { preDiscardAdv++; }
	}

	for(i = 0; i < post->playedCardCount; i++){
		if (post->playedCards[i] == adventurer) { postDiscardAdv++; }
	}

if (preT != postT){
	printf("Pre discard: %d   | post: %d\n", pre->discardCount[p], post->discardCount[p]);
	printf("Pre deckcount: %d | post: %d\n", pre->deckCount[p], post->deckCount[p]);
	printf("Pre handcount: %d | post: %d\n", pre->handCount[p], post->handCount[p]);
	printf("expDrawn: %d | drawnT: %d\n", expDrawn, drawnT);
	printf("Pre tres: %d | Post tres: %d\n", preT, postT);
}


	// Test PRE/POST States --------------------------------------------------------------------------------

	fails += assertFail( pre->handCount[p] == post->handCount[p], "Hand count mismatch.");
	fails += assertFail(preT == postT, "Incorrect number of treasures drawn into hand.");
	fails += assertFail( preHandAdv == postHandAdv, "Adventurer improperly discarded from hand.");
	fails += assertFail( preDiscardAdv == postDiscardAdv, "Adventurer improperly placed into played cards.");

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
	
	for (i = 0; i < 100000; i++){

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
