#include <algorithm>
#include <random>
#include <iostream>

#include "board.hpp"
#include "engine.hpp"

void Engine::find_best_move(const Board& b) {

    // pick a random move
    
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        this->best_move = 0;
    }
    else {
        std::vector<U16> moves;
        std::cout << all_boards_to_str(b) << std::endl;
        for (auto m : moveset) {
            std::cout << move_to_str(m) << " ";
        }
        std::cout << std::endl;
        std::sample(
            moveset.begin(),
            moveset.end(),
            std::back_inserter(moves),
            1,
            std::mt19937{std::random_device{}()}
        );
        this->best_move = moves[0];
    }
}
