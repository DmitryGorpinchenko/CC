//*********************************************************************
// class representing hex board
//
// author: Dmitry Gorpinchenko
//
//*********************************************************************

#ifndef BOARD_H
#define BOARD_H

#include <memory>

enum class NodeState : int {
    EMPTY,
    RED,
    BLUE
};

class HexBoard {
public:
    HexBoard(int board_size);
    HexBoard(const HexBoard& other);
    ~HexBoard();
    
    HexBoard& operator= (const HexBoard& that);
    
    //checks whether nodes (i1, j1) and (i2, j2) have the same state
    bool IsSameState(int i1, int j1, int i2, int j2) const;
    
    //tests whether node (i, j) is empty
    bool IsNodeEmpty(int i, int j) const;
    
    //sets node's with correct coords (i, j) state to 'state' if it is empty
    bool SetNodeState(int i, int j, NodeState state);
    
    //displays the board
    //NOTE: for proper display terminal must support color output
    void Draw() const;
    
private:  
    struct board_impl;
    std::unique_ptr<board_impl> pimpl_;
};

#endif
