//*************************************************************
// implementation details of hex game engine
//
// author: Dmitry Gorpinchenko
//
//*************************************************************

#include <unordered_map>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm> 
#include <random> 
#include <chrono>
#include <memory>
#include "hex.h"
#include "board.h"
#include "uf.h"

//each player is associated with 2 fake nodes
//to speed up S-N or E-W path connectivity checking
static const int c_fake_nodes_per_player = 2;

static const std::string c_win_msg      = "Congrats! You win :) I'm really stupid machine :(";
static const std::string c_loose_msg    = "Ha-ha, you loose :))) It's OK because I'm very smart AI!";
static const std::string c_prompt_msg   = "Enter your next move coords i j: ";
static const std::string c_think_msg    = "AI is thinking on perfect move to beat you asap ...";
static const std::string c_selected_msg = "AI selected the best move she can!";

enum class Player : int {
    HUMAN,
    AI
};

enum class Turn : int {
    FIRST,
    SECOND
};

enum class FakeNode : int {
    TOP,
    RIGHT,
    BOTTOM,
    LEFT
};

//functor for hashing enum types
template<typename EnumType>
struct enum_hash {
    int operator() (EnumType val) const {
        return static_cast<int>(val);
    }
};

using FakeIdMap     = std::unordered_map<FakeNode, int, enum_hash<FakeNode> >;
using StateUFMap    = std::unordered_map<NodeState, UF, enum_hash<NodeState> >;
using TurnStateMap  = std::unordered_map<Turn, NodeState, enum_hash<Turn> >;
using StateFakeMap  = std::unordered_map<NodeState, std::vector<FakeNode>, enum_hash<NodeState> >;
using PlayerTurnMap = std::unordered_map<Player, Turn, enum_hash<Player> >;

const PlayerTurnMap& PlayerTurn() {
    static const PlayerTurnMap map {
        { Player::HUMAN, Turn::FIRST  },
        { Player::AI,    Turn::SECOND }
    };
    return map;
}

const TurnStateMap& State() {
    static const TurnStateMap map {
        { Turn::FIRST,  NodeState::BLUE },
        { Turn::SECOND, NodeState::RED  }
    };
    return map;
}

const StateFakeMap& Fakes() {
    static const StateFakeMap map {
        { NodeState::BLUE, { FakeNode::LEFT, FakeNode::RIGHT  } },
        { NodeState::RED,  { FakeNode::TOP,  FakeNode::BOTTOM } }
    };
    return map;
}

struct HexEngine::hex_impl {
    HexBoard board;
    StateUFMap ufs;
    int game_size;
    FakeIdMap fake_id;
    std::array<Player, 2> players;

    hex_impl(int game_size) 
        : board(game_size)
        , game_size(game_size)
        , ufs {
            { NodeState::BLUE, UF(game_size * game_size + c_fake_nodes_per_player) },
            { NodeState::RED,  UF(game_size * game_size + c_fake_nodes_per_player) } 
        }
        , fake_id {
            { FakeNode::LEFT,   game_size * game_size     },
            { FakeNode::TOP,    game_size * game_size     },
            { FakeNode::RIGHT,  game_size * game_size + 1 },
            { FakeNode::BOTTOM, game_size * game_size + 1 }
        } 
    { 
        if (PlayerTurn().at(Player::HUMAN) == Turn::FIRST) {
            players = { Player::HUMAN, Player::AI };
        } else {
            players = { Player::AI, Player::HUMAN };
        }
    } 

    bool is_win(Player p) const {
        auto& fakes = Fakes().at(State().at(PlayerTurn().at(p))); 
        return ufs.at(State().at(PlayerTurn().at(p))).IsConnected(fake_id.at(fakes[0]), fake_id.at(fakes[1])); 
    }   
    
    void congratulate(Player p) const {
        board.Draw();
        std::cout << std::endl << (p == Player::HUMAN ? c_win_msg : c_loose_msg) << std::endl;
    }
    
    void move(Player p) {
        int i, j;
        auto state = State().at(PlayerTurn().at(p));
        do {
            board.Draw();
            if (p == Player::HUMAN) {
                std::cout << std::endl << c_prompt_msg;  
                std::cin >> i >> j;
            } else if (p == Player::AI) {
                std::cout << std::endl << c_think_msg << std::flush;
                ai(&i, &j);
                std::cout << std::endl << c_selected_msg << std::endl;
            }
        } while (!board.SetNodeState(i, j, state));
        for (auto& n : neigh(i, j)) {
            if (board.IsSameState(i, j, n.first, n.second)) {
                ufs.at(state).Union(n.first * game_size + n.second, i * game_size + j);
            }
        }
        if (state == NodeState::BLUE) {
            if (j == 0) {
                ufs.at(state).Union(i * game_size + j, fake_id.at(FakeNode::LEFT));
            } else if (j == game_size - 1) {
                ufs.at(state).Union(i * game_size + j, fake_id.at(FakeNode::RIGHT));
            }   
        } else if (state == NodeState::RED) {
            if (i == 0) {
                ufs.at(state).Union(i * game_size + j, fake_id.at(FakeNode::TOP));
            } else if (i == game_size - 1) {
                ufs.at(state).Union(i * game_size + j, fake_id.at(FakeNode::BOTTOM));
            }
        }
    }
    
    void ai_helper(int i, int j, const HexBoard& mental_board, UF* ai_uf) const {
        for (auto& n : neigh(i, j)) {  
            if (mental_board.IsSameState(i, j, n.first, n.second)) {
                ai_uf->Union(n.first * game_size + n.second, i * game_size + j);
            }
        }
        if (i == 0) {
            ai_uf->Union(i * game_size + j, fake_id.at(FakeNode::TOP));
        } else if (i == game_size - 1) {
            ai_uf->Union(i * game_size + j, fake_id.at(FakeNode::BOTTOM));
        }
    }
    
    void ai(int* i, int* j) const {
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        auto ai_state = State().at(PlayerTurn().at(Player::AI));
        auto  h_state = State().at(PlayerTurn().at(Player::HUMAN));
        std::vector<int> empty_ids;
        for (int i = 0; i < game_size * game_size; ++i) {
            if (board.IsNodeEmpty(i / game_size, i % game_size)) {
                empty_ids.push_back(i);
            }
        }
        //-1 because first mental ai's move we make by hand below
        std::vector<NodeState> mental_states(empty_ids.size() - 1);
        for (int i = 0; i < mental_states.size(); ++i) {
            //take turns so that right amount of h_states and ai_states appear at the end 
            mental_states[i] = i % 2 == 0 ? h_state : ai_state;
        }
        int n_trials = 1000;
        int best_wins_num = -1; 
        //try every empty slot
        for (auto id : empty_ids) {
            int n_wins = 0;
            int cur_i = id / game_size;
            int cur_j = id % game_size;
            for (int t = 0; t < n_trials; ++t) {
                auto mental_board = board;
                auto ai_uf = ufs.at(ai_state);
                mental_board.SetNodeState(cur_i, cur_j, ai_state);
                ai_helper(cur_i, cur_j, mental_board, &ai_uf);
                std::shuffle(mental_states.begin(), mental_states.end(), std::default_random_engine(seed));
                int ptr = 0;
                for (auto iid : empty_ids) {
                    if (iid != id) {
                        mental_board.SetNodeState(iid / game_size, iid % game_size, mental_states[ptr]);
                        if (mental_states[ptr++] == ai_state) {
                            ai_helper(iid / game_size, iid % game_size, mental_board, &ai_uf);
                        }
                    }
                }
                auto& fakes = Fakes().at(ai_state);  
                if (ai_uf.IsConnected(fake_id.at(fakes[0]), fake_id.at(fakes[1]))) {
                    ++n_wins;
                }
            }
            if (n_wins > best_wins_num) {
                best_wins_num = n_wins;
                *i = cur_i; 
                *j = cur_j;
            }
        }
    }
    
    std::vector<std::pair<int, int> > neigh(int i, int j) const {
        return {
            { i    , j - 1 },
            { i    , j + 1 },
            { i - 1, j     },
            { i + 1, j     },
            { i - 1, j + 1 },
            { i + 1, j - 1 },    
        };
    }
};

HexEngine::HexEngine(int game_size) 
    : pimpl_(new hex_impl(game_size))
{}

HexEngine::~HexEngine() = default;
    
void HexEngine::Run() {
    while (true) {
        for (auto player : pimpl_->players) {
            pimpl_->move(player);
            if (pimpl_->is_win(player)) {
                pimpl_->congratulate(player);
                return;
            }
        }
    }
}
