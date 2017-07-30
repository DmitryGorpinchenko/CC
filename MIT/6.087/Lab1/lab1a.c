/*
 * lab1a.c
 *
 *  Created on:
 *      Author:
 */

/* include helper functions for game */
#include "lifegame.h"

/* add whatever other includes here */

/* number of generations to evolve the world */
#define NUM_GENERATIONS 50

int main(void)
{
	/* TODO: initialize the world */
    initialize_world();
    
    /*evolve the world for NUM_GENERATIONS generations*/
	evolve_for(NUM_GENERATIONS);

	/* TODO: output final world state */
    output_world();

	return 0;
}
