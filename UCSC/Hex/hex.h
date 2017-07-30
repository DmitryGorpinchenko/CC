//*************************************************************
// class representing engine that controls the game progress
//
// author: Dmitry Gorpinchenko
//
//*************************************************************

#ifndef HEX_H
#define HEX_H

#include <memory>

class HexEngine {
public:
    HexEngine(int game_size);
    ~HexEngine();
    
    //runs the game main loop
    //NOTE: in this implementation human player always goes first
    void Run();

private:
    struct hex_impl;
    std::unique_ptr<hex_impl> pimpl_;    
};

#endif
