#include "test_suite.h"
#include <math.h>

int checkVillage(int p, int handPos, struct gameState *pre, struct gameState *post){
	int fail = 0;
	int i;

	//Set PRE game state --------------------------------------------------------------------

	//Draw 1 card
	//If pre->deck > 0... take top card from deck, place at end of hand
	if (pre->deckCount[p] > 0){
		pre->hand[p][pre->handCount[p]] = pre->deck[p][pre->deckCount[p]-1];
		pre->deckCount[p] -= 1;
		pre->handCount[p] += 1;
	}
	
	//Else, if pre->discard > 0... : copy post deck & discard into pre
	else if (pre->discardCount[p] > 0){
		memcpy(pre->deck[p], post->deck[p], sizeof(int) * pre->discardCount[p]);
		memcpy(pre->discard[p], post->discard[p], sizeof(int) * pre->discardCount[p]);

		pre->hand[p][pre->handCount[p]] = post->hand[p][post->handCount[p]-1];
		pre->handCount[p]++;
		pre->deckCount[p] = pre->discardCount[p]-1;
		pre->discardCount[p] = 0;
	}
	//Increase action count +2
	pre->numActions += 2;

	//Place village into discard

	pre->playedCards[pre->playedCardCount] = pre->hand[p][handPos];
	pre->playedCardCount++;

	if (pre->handCount[p] == handPos + 1) { pre->handCount[p]--; } //If played card is last in hand, decrement.
	else if (pre->handCount[p] == 1) { pre->handCount[p]--; } //Or, if played card is only card in hand, decrement.
	else {
		pre->hand[p][handPos] = pre->hand[p][pre->handCount[p]-1]; //Otherwise, replace played card w/last card in hand.
		pre->handCount[p]--;
	}

	//Count village cards in hand
	int preHandVil= 0, postHandVil = 0;
	int preDiscardVil = 0, postDiscardVil = 0;

	for(i = 0; i < pre->handCount[p]; i++){
		if (pre->hand[p][i] == village) { preHandVil++; }
	}

	for(i = 0; i < post->handCount[p]; i++){
		if (post->hand[p][i] == village) { postHandVil++; }
	}

	for(i = 0; i < pre->playedCardCount; i++){
		if (pre->playedCards[i] == village) { preDiscardVil++; }
	}

	for(i = 0; i < post->playedCardCount; i++){
		if (post->playedCards[i] == village) { postDiscardVil++; }
	}


	//Check POST game state -----------------------------------------------------------------

	//Check that card was drawn
	fail += assertFail(pre->handCount[p] == post->handCount[p], "Hand count mismatch.");
	fail += assertFail(pre->deckCount[p] == post->deckCount[p], "Deck count mismatch.");

	//Check action count
	fail += assertFail(pre->numActions == post->numActions, "Action num mismatch.");
	//Check that village was discarded
	fail += assertFail(preHandVil == postHandVil, "Incorrect num village cards in hand.");
	fail += assertFail(preDiscardVil == postDiscardVil, "Incorrect num village cards in played.");

	return fail;
}

int main(){
	int seed = 3;
	int failed = 0, passed = 0;

	//Create randomized game state
	struct gameState pre, post;
	int p, bonus;
	int handPos = 0;
	int testCard = village;

	int i;

	SelectStream(2);
	PutSeed(seed);
	
	for (i = 0; i < 100000; i++){
		int result = 0;

		//Create random gamestate
		randomize(&p, &handPos, testCard, &pre);
		pre.numActions = floor(Random() * 1000);
		//Test game state
		memcpy( &post, &pre, sizeof(struct gameState));
		cardEffect(testCard, 0, 0, 0, &post, handPos, &bonus);

		result = checkVillage(p, handPos, &pre, &post);
	
		//If test fails, print seed and iteration
		if (result) { printf("Test Instance FAILED %d tests. Seed: %d | Iteration: %d\n\n", result, seed, i); failed++; }
		else { passed++; }
	}

	printf("Test Summary: \tPassed = %d | Failed = %d\n", passed, failed);
	

	return 0;
}
