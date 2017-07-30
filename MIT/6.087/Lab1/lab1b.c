/*
 * lab1b.c
 *
 *  Created on:
 *      Author:
 */

/* include helper functions for game */
#include "lifegame.h"

/* add whatever other includes here */

/* number of generations to evolve the world */
#define NUM_GENERATIONS 50

int main(int argc, char ** argv)
{
	/* TODO: initialize the world, from file argv[1]
	   if command line argument provided (argc > 1), or
	   using hard-coded pattern (use Part A) otherwise */
    if(argc > 1) {
        initialize_world_from_file(argv[1]);
    } else {
        initialize_world();
    }

	evolve_for(NUM_GENERATIONS);

	/* TODO: output final world state to console and save
	   world to file "world.txt" in current directory */
    output_world();
    save_world_to_file("world.txt");

	return 0;
}

