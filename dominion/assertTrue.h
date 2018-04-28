
#include <stdio.h>

void assertTrue(int statement, char errormsg[]){
	if (!statement) { printf("FAILED. %s\n", errormsg); return; }
	printf("PASSED.\n");
	return;
}
