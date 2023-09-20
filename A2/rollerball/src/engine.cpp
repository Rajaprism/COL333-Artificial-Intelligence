// #include <algorithm>
// #include<queue>
// #include <random>
// #include <iostream>
// #include <set>
// using namespace std;;

// #include "board.hpp"
// // #include "board.cpp"
// #include "engine.hpp"


// int MaxVal(Board*b,int alpha,int beta,int depth);
// int MinVal(Board*b,int alpha,int beta,int depth);

// #define kingval 10000
// #define rookval 250
// #define bishopval 100
// #define pawnval 50
// #define depthlevel 1
// #define promote 2


// int manhatten(U8 pos,int a,int b){
//     int x=getx(pos),y=gety(pos);
//     return abs(x-a)+abs(y-b);
// }

// int evalpawn(Board *b,bool t){
//     // t-1 for white else 0;
//     int val=0;
//     if(!t){
//         val=20;
//         if(b->data.b_pawn_bs!=DEAD){
//             U8 pos=b->data.b_pawn_bs;
//             int x=getx(pos),y=gety(pos);
//             val-=min(manhatten(pos,2,1),manhatten(pos,2,0));
//             if(x==4 && y==6)val-=2;
//         }
//         if(b->data.b_pawn_ws!=DEAD){
//             U8 pos=b->data.b_pawn_ws;
//             int x=getx(pos),y=gety(pos);
//             val-=min(manhatten(pos,2,1),manhatten(pos,2,0));
//             if(x==4 && y==5)val-=2;
//         }

//     }else{
//         val=20;
//         if(b->data.w_pawn_bs!=DEAD){
//             U8 pos=b->data.w_pawn_bs;
//             int x=getx(pos),y=gety(pos);
//             val-=min(manhatten(pos,4,5),manhatten(pos,4,6));
//             if(x==2 && y==0)val-=2;
//         }
//         if(b->data.w_pawn_ws!=DEAD){
//             U8 pos=b->data.w_pawn_ws;
//             int x=getx(pos),y=gety(pos);
//             val-=min(manhatten(pos,4,5),manhatten(pos,4,6));
//             if(x==2 && y==1)val-=2;
//         }
//     }
//     return val;
// }

// int evaluate(Board* b,bool t){
//     int val=0;
//     if(t){
//         if(b->data.w_king!=DEAD)val+=kingval;
//         if(b->data.w_rook_bs!=DEAD)val+=rookval;
//         if(b->data.w_rook_ws!=DEAD)val+=rookval;
//         if(b->data.w_bishop!=DEAD)val+=bishopval;
//         if(b->data.w_pawn_bs!=DEAD)val+=pawnval;
//         if(b->data.w_pawn_ws!=DEAD)val+=pawnval;
//         val+=promote*evalpawn(b,t);
//     }else{
//         if(b->data.b_king!=DEAD)val+=kingval;
//         if(b->data.b_rook_bs!=DEAD)val+=rookval;
//         if(b->data.b_rook_ws!=DEAD)val+=rookval;
//         if(b->data.b_bishop!=DEAD)val+=bishopval;
//         if(b->data.b_pawn_bs!=DEAD)val+=pawnval;
//         if(b->data.b_pawn_ws!=DEAD)val+=pawnval;
//         val+=promote*evalpawn(b,t);
//     }
//     return val;
// }

// int MinVal(Board* b,int alpha,int beta,int depth){

//     auto moveset=b->get_legal_moves();
//     if(depth==0 || moveset.size()==0){
//         if(b->data.player_to_play==WHITE)
//         return evaluate(b,true)-evaluate(b,false);
//         else
//         return evaluate(b,false)-evaluate(b,true);
//     }
//     int val=INT_MAX;
//     for(auto move: moveset){
//         auto copyb=b->copy();
//         copyb->do_move(move);
//         val=min(val,MaxVal(copyb,alpha,beta,depth-1));
//         beta=min(beta,val);
//         delete copyb;
//         if(alpha>=beta)return val;
//     }
//     return val;
// }

// int MaxVal(Board* b,int alpha,int beta,int depth){
//     auto moveset=b->get_legal_moves();
//     if(depth==0 || moveset.size()==0){
//         if(b->data.player_to_play==WHITE)
//         return evaluate(b,true)-evaluate(b,false);
//         else 
//         return evaluate(b,false)-evaluate(b,true);
//     }
//     int val=INT_MIN;
//     for(auto move : moveset){
//         auto copyb=b->copy();
//         copyb->do_move(move);
//         val=max(val,MinVal(copyb,alpha,beta,depth-1));
//         alpha=max(alpha,val);
//         delete copyb;
//         if(alpha>=beta) return val;
//     }
//     return val;
// }

// void Engine::find_best_move(const Board& b) {

//     auto startTime = std::chrono::high_resolution_clock::now();
//     auto moveset = b.get_legal_moves();
//     if (moveset.size() == 0) {
//         this->best_move = 0;
//         return;
//     }

//     else {
//         priority_queue<pair<int,U16>> bestmove;
//         U16 best_move=*moveset.begin();
//         for(auto move : moveset){
//             Board* c = b.copy();
//             c->do_move(move);
//             int val=MinVal(c,INT_MIN,INT_MAX,depthlevel);
//             bestmove.push({val,move});
//             delete c;
//         }
//         Board * c=b.copy();
//         best_move=bestmove.top().second;
//         c->do_move(best_move);
//         delete c;

//         auto endTime = std::chrono::high_resolution_clock::now();
//         std::chrono::duration<double> elapsedTime = endTime - startTime;
//         // Convert elapsed time to seconds
//         double seconds = elapsedTime.count();
//         cout<<"time to calculate"<<seconds;

//         this->best_move=best_move;
        
//     }
// }



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

    if(b->data.b_rook_ws  != DEAD)ct++;;
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
    if (is_checkmate(b)) {
        val -=5000; // Increase the value for checks by the white player.
    }
    b->_flip_player();
    if (b->in_check()) {
        val +=1000; // Increase the value for checks by the white player.
    }
    if (is_checkmate(b)) {
        val +=5000; // Increase the value for checks by the white player.
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
    } 
    else {
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