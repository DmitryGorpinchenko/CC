//*********************************************************************************
// implementation details of HexBoard data type
//
// author: Dmitry Gorpinchenko
//
//*********************************************************************************

#include <iostream>
#include <unordered_map>
#include <vector>
#include "board.h"

//functor for hashing enum types
template<typename EnumType>
struct enum_hash {
    int operator() (EnumType val) const {
        return static_cast<int>(val);
    }
};

using ColorMap = std::unordered_map<NodeState, std::string, enum_hash<NodeState> >;

//maps node state to color code for more creative visualization
const ColorMap& Color() {
    static const ColorMap color_map {
        { NodeState::EMPTY, "\033[37m" },
        { NodeState::RED,   "\033[31m" },
        { NodeState::BLUE,  "\033[34m" }
    };
    return color_map;
}

struct HexBoard::board_impl {
    int board_size;
    //represent 2D board with 1D array
    std::vector<NodeState> nodes;
    
    board_impl(int board_size) 
        : board_size(board_size)
        , nodes(board_size * board_size, NodeState::EMPTY) 
    {}

//useful macros to avoid extensive typing of pimpl_->nodes[.....] etc.    
#define HEX_SIZE        (pimpl_->board_size)
#define HEX_STATE(i, j) (pimpl_->nodes[(i) * HEX_SIZE + (j)])
};

HexBoard::HexBoard(int board_size) 
    : pimpl_(new board_impl(board_size)) 
{}
    
HexBoard::HexBoard(const HexBoard& other) 
    : pimpl_(new board_impl(*other.pimpl_)) 
{}   

HexBoard::~HexBoard() = default;
   
HexBoard& HexBoard::operator= (const HexBoard& that) {
    if (this != &that) {
        *this->pimpl_ = *that.pimpl_;      
    }
    return *this;
}  
   
bool HexBoard::IsNodeEmpty(int i, int j) const {
    if (i < 0 || i >= HEX_SIZE || j < 0 || j >= HEX_SIZE) {
        throw std::out_of_range("illegal node coords");
    }
    return HEX_STATE(i, j) == NodeState::EMPTY;
}   
    
bool HexBoard::IsSameState(int i1, int j1, int i2, int j2) const {
    if (i1 < 0 || i1 >= HEX_SIZE || j1 < 0 || j1 >= HEX_SIZE 
    ||  i2 < 0 || i2 >= HEX_SIZE || j2 < 0 || j2 >= HEX_SIZE) {
        return false;
    }        
    return HEX_STATE(i1, j1) == HEX_STATE(i2, j2);
}    
    
bool HexBoard::SetNodeState(int i, int j, NodeState state) {
    if (i < 0 || i >= HEX_SIZE || j < 0 || j >= HEX_SIZE || HEX_STATE(i, j) != NodeState::EMPTY) {
        return false;
    }
    HEX_STATE(i, j) = state;
    return true;
}    
    
void HexBoard::Draw() const {
    std::cout << std::endl;
    for (int i = 0; i < HEX_SIZE - 1; ++i) {
        std::cout << std::string(i,  ' ');
        for (int j = 0; j < HEX_SIZE - 1; ++j) {
            std::cout << Color().at(HEX_STATE(i, j)) << '*';
            std::cout << Color().at(NodeState::EMPTY) << " - ";
        }
        std::cout << Color().at(HEX_STATE(i, HEX_SIZE - 1)) << '*' << std::endl;
        std::cout << Color().at(NodeState::EMPTY) << std::string(i,  ' ');
        for (int j = 0; j < HEX_SIZE - 1; ++j) {
            std::cout << "\\ / ";
        }
        std::cout << "\\" << std::endl;
    }
    std::cout << std::string(HEX_SIZE - 1,  ' ');
    for (int j = 0; j < HEX_SIZE - 1; ++j) {
        std::cout << Color().at(HEX_STATE(HEX_SIZE - 1, j)) << '*';
        std::cout << Color().at(NodeState::EMPTY) << " - ";
    }
    std::cout << Color().at(HEX_STATE(HEX_SIZE - 1, HEX_SIZE - 1)) << '*' << std::endl;
    std::cout << Color().at(NodeState::EMPTY);
}
