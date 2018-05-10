#ifndef _TEST_SUITE_H
#define _TEST_SUITE_H

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Assert Statement (PASS/FAIL: Non-Abortive)
int assertTrue(int statement, char errormsg[]);

// Set/Check Current Kingdom Card Supply Counts
void setKCards(int count, int cards[], int numKCards, struct gameState *state);
void checkKCards(int count, int cards[], int numKCards, struct gameState state);

// Direct Manipulation of Player Deck, Hand & Discard Cards
void addToDeck(int player, int card, struct gameState *state);
void addToHand(int player, int card, struct gameState *state);
void addToDiscard(int player, int card, struct gameState *state);
void emptyPlayerCards(int player, struct gameState *state);

// Card Effect Test Functions

// Sets a player's deck/hand/discard count and card values.
void setInstance(		int deckArr[], int deckSize,
				int handArr[], int handSize,
				int discardArr[], int discardSize,
				int player, struct gameState *before);

// Runs the specified card effect on a game state.
void runInstance(		int choice1, int choice2, int choice3,
				int handPos, int *bonus, int cardToTest,
				struct gameState *before, struct gameState *after);

// Asserts that the deck/hand/discard states of all other players remain unchanged.
//void testState(int currentPlayer, struct gameState before, struct gameState after);

void testActionCount(int expectedDif, struct gameState before, struct gameState after);

void testBuyCount(int expectedDif, struct gameState before, struct gameState after);

void assertOtherPlayerCardsUnchanged(int currentPlayer, struct gameState before, struct gameState after);

// Asserts that all cards in the draw list were drawn from a player's deck into that player's hand in correct order.
//void testDrawnCards(		int player, int draws[], int lengthDraws,
//				struct gameState before, struct gameState after);

void testCardsDrawn(int player, int draws[], int expectedDraws, struct gameState before, struct gameState after);

void testFullDeckCount(int player, int expectedDraws, int expectedDiscards, int expectedPlayed, struct gameState before, struct gameState after);	

int assertFail(int statement, char errMsg[]);

void randomize(int *p, int *handPos, int cardToTest, struct gameState *state);


#endif
