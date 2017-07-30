//*******************************************************************
// main program starting the game of Hex on 11x11 board
//
// program should be compiled on Linux with recent version of g++:
//
//   g++ -std=c++11 main.cpp uf.cpp board.cpp hex.cpp 
//
// and then run as follows:
//
//  ./a.out
//
// author: Dmitry Gorpinchenko
//
//*******************************************************************

#include "hex.h"

static const int game_size = 11;

int main() {
    HexEngine game(game_size);
    game.Run();
    return 0;
}
