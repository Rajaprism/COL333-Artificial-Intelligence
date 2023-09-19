#include <algorithm>
#include <random>
#include <iostream>
#include <set>
using namespace std;;

#include "board.hpp"
#include "engine.hpp"
int abtak=INT_MIN;

bool only_king(Board *b){

    int ct=0;

    if(b->data.b_rook_ws  != DEAD)ct++;
    if(b->data.b_rook_bs  != DEAD)ct++;
    if(b->data.b_bishop   != DEAD)ct++;
    if(b->data.b_pawn_ws  != DEAD)ct++;
    if(b->data.b_pawn_bs  != DEAD)ct++;

    if(ct==0)return true;
    return false;

}

bool is_checkmate(Board* b) {

    if (!b->in_check()) {
        return false; 
    }
    auto moveset = b->get_legal_moves();

    for (auto move : moveset) {
        Board* c = b->copy();
        c->do_move(move);
        
        if (!c->in_check()) {
            delete c;
            return false;
        }
        
        delete c;
    }
    return true;
}

int evaluation(Board* b){

    int val=0;

    if (b->in_check()) {
        val -=1000; // Increase the value for checks by the white player.
    }
    b->_flip_player();
    if (b->in_check()) {
        val +=1000; // Increase the value for checks by the white player.
    }
    b->_flip_player();
    
    if(b->data.w_rook_ws  != DEAD)val+=5;
    if(b->data.w_rook_bs  != DEAD)val+=5;
    if(b->data.w_bishop   != DEAD)val+=7;
    if(b->data.w_pawn_ws  != DEAD)val+=2;
    if(b->data.w_pawn_bs  != DEAD)val+=2;

    if(b->data.b_rook_ws  != DEAD)val-=5;
    if(b->data.b_rook_bs  != DEAD)val-=5;
    if(b->data.b_bishop   != DEAD)val-=7;
    if(b->data.b_pawn_ws  != DEAD)val-=2;
    if(b->data.b_pawn_bs  != DEAD)val-=2;

    return val;
}


int minimax(Board* b, int depth, int alpha, int beta, bool maximizingPlayer, U16& bestMove, set<Board*>& st, set<Board*>& st_real,bool ok,int deep) {


    if (depth == 0) {

        int eval=evaluation(b);

        if(eval<abtak-10 && ok==true){
            eval = minimax(b, 1, alpha, beta, true, bestMove, st, st_real,false,deep);
            return eval;
        }

        return evaluation(b);
    }

    
    st.insert(b);
    auto moveset = b->get_legal_moves();
    Board* best_board = nullptr;
    if (moveset.size() == 0) {
        return evaluation(b);
    }

    if (maximizingPlayer) {
        int maxEval = INT_MIN;

        for (auto move : moveset) {
            Board* c = b->copy();
            c->do_move(move);
            if (st.find(c) != st.end() || st_real.find(c) != st_real.end()) {
                delete c;
                continue;
            }

            int eval = minimax(c, depth - 1, alpha, beta, false, bestMove, st, st_real,ok,deep);
            delete c;

            if (eval > maxEval) {
                maxEval = eval;
                if (depth == deep){
                    bestMove = move;
                    abtak=maxEval;
                }
            }

            // alpha = std::max(alpha, eval);
            // if (beta <= alpha) {
            //     return eval;
            // }
        }

        st.erase(b);
        return maxEval;
    } else {
        int minEval = INT_MAX;

        for (auto move : moveset) {
            Board* c = b->copy();
            c->do_move(move);
            if (st.find(c) != st.end() || st_real.find(c) != st_real.end()) {
                delete c;
                continue;
            }

            int eval = minimax(c, depth - 1, alpha, beta, true, bestMove, st, st_real,ok,deep);
            delete c;

            if (eval < minEval) {
                minEval = eval;
                if (depth == deep) bestMove = move;
            }

            // beta = std::min(beta, eval);
            // if (beta <= alpha) {
            //     return eval;
            // }
        }
        st.erase(b);
        return minEval;
    }
}



void Engine::find_best_move(const Board& b) {

    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        this->best_move = 0;
        return;
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

        

        U16 bestmove=moves[0];
        int best_val=INT_MAX;
        set<Board*> st;
        set<Board*> st_real;
        Board* c = b.copy();
        int  deep = 3;
        st_real.insert(c);

        if(only_king(c)){

            deep = 5 ;
            minimax(c,5,true,INT_MIN,INT_MAX,bestmove,st,st_real,true,deep);
            c->do_move(bestmove);
            st_real.insert(c);
            delete c;
        
            this->best_move = bestmove;

            return;
            
        }
       
        minimax(c,3,true,INT_MIN,INT_MAX,bestmove,st,st_real,true,deep);
        
        c->do_move(bestmove);
        st_real.insert(c);
        delete c;
        
        this->best_move = bestmove;
    }
}