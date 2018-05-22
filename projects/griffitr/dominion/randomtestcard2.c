#include "test_suite.h"
#include <math.h>

int checkGreatHall(int p, int handPos, struct gameState *pre, struct gameState *post){
	int fails = 0;
	int i;

	//Set PRE game state ---------------------------------------------------------//
	
	//Draw 1 card
	//If pre->deck > 0... take top card from deck, place at end of hand
	if (pre->deckCount[p] > 0){
		pre->hand[p][pre->handCount[p]] = pre->deck[p][pre->deckCount[p]-1];
		pre->deckCount[p] -= 1;
		pre->handCount[p] += 1;
	}
	
	//Else, if pre->discard > 0... : copy post deck & discard into pre state, then match last post hand card into pre.
	else if (pre->discardCount[p] > 0){
		memcpy(pre->deck[p], post->deck[p], sizeof(int) * pre->discardCount[p]);
		memcpy(pre->discard[p], post->discard[p], sizeof(int) * pre->discardCount[p]);

		pre->handCount[p]++;
		pre->hand[p][pre->handCount[p] - 1] = post->hand[p][post->handCount[p] - 1];

		pre->deckCount[p] = pre->discardCount[p] - 1;
		pre->discardCount[p] = 0;

	}

	//Action +1
	pre->numActions++;

	//Place great_hall into played cards
	pre->playedCards[pre->playedCardCount] = pre->hand[p][handPos];
	pre->playedCardCount++;

	if (pre->handCount[p] == handPos + 1) { pre->handCount[p]--; } //If played card is last in hand, decrement.
	else if (pre->handCount[p] == 1) { pre->handCount[p]--; } //Or, if played card is only card in hand, decrement.
	else {
		pre->hand[p][handPos] = pre->hand[p][pre->handCount[p]-1]; //Otherwise, replace played card w/last card in hand.
		pre->handCount[p]--;
	}

	//Count great_hall cards in hand
	int preHandGH= 0, postHandGH = 0;
	int preDiscardGH = 0, postDiscardGH = 0;

	for(i = 0; i < pre->handCount[p]; i++){
		if (pre->hand[p][i] == great_hall) { preHandGH++; }
	}

	for(i = 0; i < post->handCount[p]; i++){
		if (post->hand[p][i] == great_hall) { postHandGH++; }
	}

	for(i = 0; i < pre->playedCardCount; i++){
		if (pre->playedCards[i] == great_hall) { preDiscardGH++; }
	}

	for(i = 0; i < post->playedCardCount; i++){
		if (post->playedCards[i] == great_hall) { postDiscardGH++; }
	}

	if (preHandGH != postHandGH){
		printf("prehandGH: %d | posthandGH: %d\n", preHandGH, postHandGH);
		printf("handCount: %d\n", pre->handCount[p]);
	}

	//Check POST game state ------------------------------------------------------//
	
	//Check that card was drawn
	fails += assertFail(pre->handCount[p] == post->handCount[p], "Hand count mismatch.");
	fails += assertFail(pre->deckCount[p] == post->deckCount[p], "Deck count mismatch.");

	//Check action count
	fails += assertFail(pre->numActions == post->numActions, "Action num mismatch.");
	//Check that great hall was discarded
	fails += assertFail(preHandGH == postHandGH, "Incorrect num great hall cards in hand.");
	fails += assertFail(preDiscardGH == postDiscardGH, "Incorrect num great hall cards in played.");


	return fails;
}

int main(){
	int seed = 3;
	int failed = 0, passed = 0;

	//Create randomized game state
	struct gameState pre, post;
	int p, bonus;
	int handPos = 0;
	int testCard = great_hall;

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

		result = checkGreatHall(p, handPos, &pre, &post);
	
		//If test fails, print seed and iteration
		if (result) { printf("Test Instance FAILED %d tests. Seed: %d | Iteration: %d\n\n", result, seed, i); failed++; }
		else { passed++; }
/*
		if (i == 16269 || i == 17391 || i == 21679 || i == 26893 || i == 54427){

			printf("Pre handCount: %d | Post handCount: %d \n", pre.handCount[p], post.handCount[p]);
			
			int j;	
			int preHandGH= 0, postHandGH = 0;
			int preDiscardGH = 0, postDiscardGH = 0;

			for(j = 0; j < pre.handCount[p]; j++){
				if (pre.hand[p][j] == great_hall) { preHandGH++; }
			}

			for(j = 0; j < post.handCount[p]; j++){
			if (post.hand[p][j] == great_hall) { postHandGH++; }
			}
		
			for(j = 0; j < pre.playedCardCount; j++){
				if (pre.playedCards[j] == great_hall) { preDiscardGH++; }
			}

			for(j = 0; j < post.playedCardCount; j++){
				if (post.playedCards[j] == great_hall) { postDiscardGH++; }
			}

			printf("Pre GH in hand: %d    | Post GH in hand: %d\n", preHandGH, postHandGH);
			printf("Pre GH in played: %d  | Post GH in played: %d\n", preDiscardGH, postDiscardGH);
			printf("Pre/Post deckCount: %d | %d  || Pre/Post discardCount: %d | %d\n",
				pre.deckCount[p], post.deckCount[p], pre.discardCount[p], post.discardCount[p]);
			printf("HandPos= %d |  HandPos in hand... Pre: %d | Post: %d\n", handPos, pre.hand[p][handPos], post.hand[p][handPos]);
			printf("\n");
		}
*/
	}

	printf("Test Summary: \tPassed = %d | Failed = %d\n", passed, failed);


	return 0;
}
