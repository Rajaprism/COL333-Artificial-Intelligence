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
            std::cout << move_to_str(m) << "raja "<<std::endl;
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
// #include<bits/stdc++.h>
// #include <algorithm>
// #include <random>
// #include <iostream>
// #include <set>
// using namespace std;;

// #include "board.hpp"
// #include "engine.hpp"

// int eval(const Board& b){

//     int val=0;

//     if(b.data.b_rook_ws  == DEAD)val+=3;
//     if(b.data.b_rook_bs  == DEAD)val+=3;
//     if(b.data.b_king     == DEAD)val+=100;
//     if(b.data.b_bishop   == DEAD)val+=5;
//     if(b.data.b_pawn_ws  == DEAD)val+=1;
//     if(b.data.b_pawn_bs  == DEAD)val+=1;

//     if(b.data.w_rook_ws  == DEAD)val-=3;
//     if(b.data.w_rook_bs  == DEAD)val-=3;
//     if(b.data.w_king     == DEAD)val-=100;
//     if(b.data.w_bishop   == DEAD)val-=5;
//     if(b.data.w_pawn_ws  == DEAD)val-=1;
//     if(b.data.w_pawn_bs  == DEAD)val-=1;

//     if(b.data.player_to_play == WHITE)val*=-1;

//     return val;

// }

// int minimax(const Board& b, int depth, int alpha, int beta, bool maximizingPlayer, U16& bestMove, set<Board*>& st, set<Board*>& st_real) {
//     if (b.in_check()) {
//         if (maximizingPlayer) return INT_MIN;
//         else return INT_MAX;
//     }
//     if (depth == 0) {
//         return eval(b);
//     }

//     if (maximizingPlayer) {
//         int maxEval = INT_MIN;
//         auto moveset = b.get_legal_moves();
//         Board* best_board = nullptr;

//         for (auto move : moveset) {
//             Board* c = b.copy();
//             c->do_move(move);
//             if (st.find(c) != st.end() || st_real.find(c) != st_real.end()){
//                 delete c;
//                 continue;
//             }
//             int eval = minimax(*c, depth - 1, alpha, beta, false, bestMove, st, st_real);
//             if (eval > maxEval) {
//                 maxEval = eval;
//                 bestMove = move;
//                 best_board = c;
//             }
//             st.insert(best_board);
//             delete c;

//             alpha = std::max(alpha, eval);
//             if (beta <= alpha) break; 
//         }
//         return maxEval;
//     } 
//     else {
//         int minEval = INT_MAX;
//         auto moveset = b.get_legal_moves();
//         Board* best_board = nullptr;

//         for (auto move : moveset) {
//             Board* c = b.copy();
//             c->do_move(move);
//             if (st.find(c) != st.end() || st_real.find(c) != st_real.end()){
//                 delete c;
//                 continue;
//             }
//             int eval = minimax(*c, depth - 1, alpha, beta, true, bestMove, st, st_real);
//             if (eval < minEval) {
//                 minEval = eval;
//                 bestMove = move;
//                 best_board = c;
//             }
//             st.insert(best_board);
//             delete c;

//             beta = std::min(beta, eval);
//             if (beta <= alpha) break; 
//         }
//         return minEval;
//     }
// }


// void Engine::find_best_move(const Board& b) {

//     auto moveset = b.get_legal_moves();
//     if (moveset.size() == 0) {
//         this->best_move = 0;
//         return;
//     }

//     else {
//         std::vector<U16> moves;
//         std::cout << all_boards_to_str(b) << std::endl;
//         for (auto m : moveset) {
//             std::cout << move_to_str(m) << " ";
//         }
//         std::cout << std::endl;
//         std::sample(
//             moveset.begin(),
//             moveset.end(),
//             std::back_inserter(moves),
//             1,
//             std::mt19937{std::random_device{}()}
//         );

//         U16 bestmove=moves[0];
//         int best_val=INT_MAX;
//         set<Board*> st;
//         set<Board*> st_real;
       
//         minimax(b,8,true,INT_MIN,INT_MAX,bestmove,st,st_real);
//         Board* c = b.copy();
//         c->do_move(bestmove);
//         st_real.insert(c);
//         delete c;

//         this->best_move = bestmove;
//     }
// }


