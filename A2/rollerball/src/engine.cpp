#include <algorithm>
#include<queue>
#include <random>
#include <iostream>
#include <set>
using namespace std;;

#include "board.hpp"
// #include "board.cpp"
#include "engine.hpp"


int MaxVal(Board*b,int alpha,int beta,int depth);
int MinVal(Board*b,int alpha,int beta,int depth);

#define kingval 1000
#define o_rookval 120
#define i_rookval 100
#define bishopval 70
#define pawnval 50
#define checkval 10000
#define depthlevel 2
#define promote 3
#define dist 10


int manhatten(U8 pos,int a,int b){
    int x=getx(pos),y=gety(pos);
    return abs(x-a)+abs(y-b);
}

int evalpawn(Board *b,bool t){
    // t-1 for white else 0;
    int val=0;
    if(!t){
        val=dist;
        if(b->data.b_pawn_bs!=DEAD){
            U8 pos=b->data.b_pawn_bs;
            int x=getx(pos),y=gety(pos);
            val-=min(manhatten(pos,2,1),manhatten(pos,2,0));
            if(x==4 && y==6)val-=2;
        }
        if(b->data.b_pawn_ws!=DEAD){
            U8 pos=b->data.b_pawn_ws;
            int x=getx(pos),y=gety(pos);
            val-=min(manhatten(pos,2,1),manhatten(pos,2,0));
            if(x==4 && y==5)val-=2;
        }

    }else{
        val=dist;
        if(b->data.w_pawn_bs!=DEAD){
            U8 pos=b->data.w_pawn_bs;
            int x=getx(pos),y=gety(pos);
            val-=min(manhatten(pos,4,5),manhatten(pos,4,6));
            if(x==2 && y==0)val-=2;
        }
        if(b->data.w_pawn_ws!=DEAD){
            U8 pos=b->data.w_pawn_ws;
            int x=getx(pos),y=gety(pos);
            val-=min(manhatten(pos,4,5),manhatten(pos,4,6));
            if(x==2 && y==1)val-=2;
        }
    }
    return val;
}
int getrookval(U8 pos){
    int val=0;
    int x=getx(pos),y=gety(pos);
    if(x==0 || x==6 || y==0 || y==6)val+=o_rookval;
    else val+=i_rookval;
    return val;
}
int evalrook(Board* b,bool t){
    int val=0;
    if(t){
        if(b->data.w_rook_bs!=DEAD)val+=getrookval(b->data.w_rook_bs);
        if(b->data.w_rook_ws!=DEAD)val+=getrookval(b->data.w_rook_ws);
    }else{
        if(b->data.b_rook_bs!=DEAD)val+=getrookval(b->data.b_rook_bs);
        if(b->data.b_rook_ws!=DEAD)val+=getrookval(b->data.b_rook_ws);
    }
    return val;
}

// int checkmate(Board* b,bool t){

// }

int evaluate(Board* b,bool t){
    int val=0;
    if(t){
        if(b->data.w_king!=DEAD)val+=kingval;
        if(b->data.w_bishop!=DEAD)val+=bishopval;
        if(b->data.w_pawn_bs!=DEAD)val+=pawnval;
        if(b->data.w_pawn_ws!=DEAD)val+=pawnval;
        if(b->in_check())val-=checkval;
        // if(checkmate(b,t))val+=checkval;
    }else{
        if(b->data.b_king!=DEAD)val+=kingval;
        if(b->data.b_bishop!=DEAD)val+=bishopval;
        if(b->data.b_pawn_bs!=DEAD)val+=pawnval;
        if(b->data.b_pawn_ws!=DEAD)val+=pawnval;
        if(b->in_check())val-=checkval;
        // if(checkmate(b,t))val+=checkval;
    }
    val+=evalrook(b,t);
    val+=promote*evalpawn(b,t);
    return val;
}

int MinVal(Board* b,int alpha,int beta,int depth){

    auto moveset=b->get_legal_moves();
    if(depth==0 || moveset.size()==0){
        cout<<"Depth reached"<<endl;
        if(b->data.player_to_play==WHITE)
        return -evaluate(b,true)+evaluate(b,false);
        else
        return -evaluate(b,false)+evaluate(b,true);
    }
    int val=INT_MAX;
    for(auto move: moveset){
        auto copyb=b->copy();
        copyb->do_move(move);
        val=min(val,MaxVal(copyb,alpha,beta,depth-1));
        beta=min(beta,val);
        delete copyb;
        if(alpha>=beta)return val;
    }
    return val;
}

int MaxVal(Board* b,int alpha,int beta,int depth){
    auto moveset=b->get_legal_moves();
    if(depth==0 || moveset.size()==0){
        if(b->data.player_to_play==WHITE)
        return evaluate(b,true)-evaluate(b,false);
        else 
        return evaluate(b,false)-evaluate(b,true);
    }
    int val=INT_MIN;
    for(auto move : moveset){
        auto copyb=b->copy();
        copyb->do_move(move);
        val=max(val,MinVal(copyb,alpha,beta,depth-1));
        alpha=max(alpha,val);
        delete copyb;
        if(alpha>=beta) return val;
    }
    return val;
}

void Engine::find_best_move(const Board& b) {

    // auto startTime = std::chrono::high_resolution_clock::now();
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        this->best_move = 0;
        return;
    }

    else {
        // priority_queue<pair<int,U16>> bestmove;
        U16 best_move=*moveset.begin();
        int mx=INT_MIN;
        for(auto move : moveset){
            Board* c = b.copy();
            c->do_move(move);
            int val=MinVal(c,INT_MIN,INT_MAX,depthlevel);
            if(val>mx){
                best_move=move;
                mx=val;
            }
            delete c;
        }
        Board * c=b.copy();
        c->do_move(best_move);
        delete c;

        this->best_move=best_move;
        
    }
}





// #include <algorithm>
// #include <random>
// #include <iostream>
// #include <set>
// #include <map>
// using namespace std;;

// #include "board.hpp"
// #include "engine.hpp"
// int abtak=INT_MIN;
// vector<Board*> vec_dfs;
// vector<Board*> vec_overall;
// set<string> mp;

// void flip_player(Board* b) {
//     b->data.player_to_play = (PlayerColor)(b->data.player_to_play ^ (WHITE | BLACK));
// }

// bool areBoardDataEqual(Board& bd1,Board& bd2) {
//     return (
//         bd1.data.b_rook_ws == bd2.data.b_rook_ws &&
//         bd1.data.b_rook_bs == bd2.data.b_rook_bs &&
//         bd1.data.b_king == bd2.data.b_king &&
//         bd1.data.b_bishop == bd2.data.b_bishop &&
//         bd1.data.b_pawn_ws == bd2.data.b_pawn_ws &&
//         bd1.data.b_pawn_bs == bd2.data.b_pawn_bs &&
//         bd1.data.w_rook_ws == bd2.data.w_rook_ws &&
//         bd1.data.w_rook_bs == bd2.data.w_rook_bs &&
//         bd1.data.w_king == bd2.data.w_king &&
//         bd1.data.w_bishop == bd2.data.w_bishop &&
//         bd1.data.w_pawn_ws == bd2.data.w_pawn_ws &&
//         bd1.data.w_pawn_bs == bd2.data.w_pawn_bs &&
//         bd1.data.player_to_play == bd2.data.player_to_play &&
//         bd1.data.last_killed_piece == bd2.data.last_killed_piece &&
//         bd1.data.last_killed_piece_idx == bd2.data.last_killed_piece_idx
//     );
// }

// bool is_present(Board* b){
//     bool val=true;
//     for(auto x: vec_dfs){
//         val&=areBoardDataEqual(*b,*x);
//     }
//     return val;
// }

// bool only_king(Board *b){

//     int ct=0;

//     if(b->data.b_rook_ws  != DEAD)ct++;
//     if(b->data.b_rook_bs  != DEAD)ct++;
//     if(b->data.b_bishop   != DEAD)ct++;
//     if(b->data.b_pawn_ws  != DEAD)ct++;
//     if(b->data.b_pawn_bs  != DEAD)ct++;

//     if(ct==0)return true;
//     return false;

// }

// int is_bishop_rook(Board *b){

//     int ct=0;

//     if(b->data.b_rook_ws  != DEAD)ct++;
//     if(b->data.b_rook_bs  != DEAD)ct++;
//     if(b->data.b_bishop   != DEAD)ct++;

//     return ct;

// }

// bool is_checkmate(Board* b) {
//     flip_player(b);
//     bool ok = (b->in_check() && b->get_legal_moves().size()==0);
//     flip_player(b);

//     return ok;
// }

// int clockwiseDistance(Board* b,bool ok,bool ok2) {
//     U8 blackKingPos = b->data.b_king;
//     U8 whiteBishopPos = b->data.w_bishop;
//     U8 rook_ws = b->data.w_rook_ws;
//     U8 rook_bs = b->data.w_rook_bs;

//     int w_x,w_y;
//     int king_x = getx(blackKingPos);
//     int king_y = gety(blackKingPos);
//     if(ok){
//         w_x = getx(whiteBishopPos);
//         w_y = gety(whiteBishopPos);
//     }
//     else if(ok && ok2){
//         w_x = getx(rook_ws);
//         w_y = gety(rook_ws);
//     }
//     else{
//         w_x = getx(rook_bs);
//         w_y = gety(rook_bs);
//     }

//     int dis=0;

//     if((king_y==5 || king_y==6) && (w_y==5 || w_y==6)){
//         dis += abs(king_x-w_x);
//     }
//     if((king_y==0 || king_y==1) && (w_y==0 || w_y==1)){
//         dis += w_x+6+6-king_x;
//     }
//     else{
//         dis += king_x+w_x;
//     }

//     if((king_x==5 || king_x==6)){
//         dis+=(6-w_y)+(6-king_y);
//     }
//     else{
//         dis+=abs(king_y-w_y);
//     }

//     return dis;
// }

// int pawnPromotionDis(Board* b,bool ok){

//     U8 pawn_ws = b->data.w_pawn_ws;
//     U8 pawn_bs = b->data.w_pawn_bs;
//     int x=getx(pawn_ws);
//     int y=gety(pawn_ws);
//     if(!ok){
//         x=getx(pawn_bs);
//         y=gety(pawn_bs);
//     }

//     int dis=0;

//     if(y==1 || y==2){
//         dis+=x-1;
//     }
//     else{
//         dis+=4-x;
//     }
//     dis+=7-y;

//     return dis;
// }

// int evaluation(Board* b) {
//     int val = 0;

//     if (b->in_check()) {
//         val -= 1000000; 
//     }
    
//     if (is_checkmate(b)) {     // opposite
//         val += 5000000; 
//     }
//     flip_player(b);

//     if (b->data.w_rook_ws != DEAD){
//         val += 500;
//         int dis=clockwiseDistance(b,false,true);
//         if(!(dis<=2))val+=12000/(dis+2);

//     } 
//     if (b->data.w_rook_bs != DEAD){
//         val += 500;
//         int dis=clockwiseDistance(b,false,false);
//         if(!(dis<=2))val+=12000/(dis);
        
//     } 
//     if (b->data.w_bishop != DEAD){
//         val += 300;
//         int dis=clockwiseDistance(b,true,false);
//         val+=10000/(dis);

//     } 
   
//     if (b->data.w_pawn_ws != DEAD) {
        
//         int dis = pawnPromotionDis(b,true);
//         dis++;
//         val+=5000/dis;
//         val+=100;
        
//     }
//     if (b->data.w_pawn_bs != DEAD) {
//         int dis = pawnPromotionDis(b,false);
//         dis++;
//         val+=5000/dis;
//         val+=100;
        
//     }

//     if (b->data.b_rook_ws != DEAD) val -= 500;
//     if (b->data.b_rook_bs != DEAD) val -= 500;
//     if (b->data.b_bishop != DEAD) val -= 300;

//     if (b->data.b_pawn_ws != DEAD) {
//         int y=gety(b->data.b_pawn_ws);
//         if(y==0 || y==1 || y==2)val-=700;
//         val-=100;
        
//     }
//     if (b->data.b_pawn_bs != DEAD) {
//         int y=gety(b->data.b_pawn_bs);
//         if(y==0 || y==1 || y==2)val-=700;
//         val-=100;
//     }

//     return val;
// }

// int minimax(Board* b, int depth, int alpha, int beta, bool maximizingPlayer, U16& bestMove,bool ok,int deep,int &visit) {
//     visit++;
    
//     if(is_checkmate(b)){
//         return evaluation(b);
//     }

//     if (depth == 0) { 
//         return evaluation(b);
//     }

//     vec_dfs.push_back(b);
//     auto moveset = b->get_legal_moves();
//     Board* best_board = nullptr;
//     if (moveset.size() == 0) {
//         return evaluation(b);
//     }

//     if (maximizingPlayer) {
//         int maxEval = INT_MIN;

//         for (auto move : moveset) {
//             Board* c = b->copy();
//             c->do_move(move);
//             string brd_str = all_boards_to_str(*c);
//             if (is_present(c) || mp.find(brd_str)!=mp.end()) {
//                 delete c;
//                 continue;
//             }

//             int eval = minimax(c, depth - 1, alpha, beta, false, bestMove,ok,deep,visit);
//             delete c;

//             if (eval > maxEval) {
//                 maxEval = eval;
//                 if (depth == deep){
//                     bestMove = move;
//                     abtak=maxEval;
//                 }
//             }

//             alpha = std::max(alpha, eval);
//             if(depth != deep && beta <= alpha){
//                 break;
//             }

//         }

//         vec_dfs.pop_back();
//         return maxEval;
//     } 
//     else {
//         int minEval = INT_MAX;

//         for (auto move : moveset) {
//             Board* c = b->copy();
//             c->do_move(move);
//             string brd_str = all_boards_to_str(*c);
//             if (is_present(c) || mp.find(brd_str)!=mp.end()) {
//                 delete c;
//                 continue;
//             }


//             int eval = minimax(c, depth - 1, alpha, beta, true, bestMove,ok,deep,visit);
//             delete c;

//             if (eval < minEval) {
//                 minEval = eval;
//                 if (depth == deep) bestMove = move;
//             }

//             beta = std::min(beta, eval);
//             if (depth != deep && beta <= alpha) {
//                 return eval;
//             }
//         }
//         vec_dfs.pop_back();
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

//         if(b.data.player_to_play==BLACK){
//             this->best_move = moves[0];
//             return ;
//         }


//         string brd_str;
//         U16 bestmove=moves[0];
//         int best_val=INT_MAX;
//         Board* c = b.copy();
//         int  deep = 4;

//         brd_str = all_boards_to_str(b);
//         mp.insert(brd_str);

        

//         // if(only_king(c)){

//         //     deep = 4 ;
//         //     minimax(c,4,true,INT_MIN,INT_MAX,bestmove,true,deep);
//         //     c->do_move(bestmove);
//         //     vec_overall.push_back(c);
//         //     delete c;  
//         //     this->best_move = bestmove;

//         //     return;
            
//         // }
//         int visit=0;
//         minimax(c,4,INT_MIN,INT_MAX,true,bestmove,true,deep,visit);
//         cout<< visit <<" : No. of nodes visited in this step"<<endl;
        
//         c->do_move(bestmove);
//         brd_str = all_boards_to_str(*c);
//         mp.insert(brd_str);
        
//         this->best_move = bestmove;
//     }
// }