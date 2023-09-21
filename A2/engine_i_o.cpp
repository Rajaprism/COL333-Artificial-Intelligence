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
#define rookval 100
#define bishopval 70
#define pawnval 50
#define checkval 10000
#define depthlevel 1
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
    return rookval;
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

int checkpawn(Board* b, U8 pos){
    U8 pawn=b->data.board_0[pos];
    if(pawn & PAWN) return pawnval;
    if(pawn & PAWN_BISHOP) return bishopval;
    if(pawn & PAWN_ROOK) return getrookval(pos);
    return 0;
}

// int checkmate(Board* b,bool t){

// }

int evaluate(Board* b,bool t){
    int val=0;
    if(t){
        if(b->data.w_king!=DEAD)val+=kingval;
        if(b->data.w_bishop!=DEAD)val+=bishopval;
        if(b->data.w_pawn_bs!=DEAD)val+=checkpawn(b,b->data.w_pawn_bs);
        if(b->data.w_pawn_ws!=DEAD)val+=checkpawn(b,b->data.w_pawn_ws);;
        if(b->in_check())val+=checkval;
        // if(checkmate(b,t))val+=checkval;
    }else{
        if(b->data.b_king!=DEAD)val+=kingval;
        if(b->data.b_bishop!=DEAD)val+=bishopval;
        if(b->data.b_pawn_bs!=DEAD)val+=checkpawn(b,b->data.b_pawn_bs);
        if(b->data.b_pawn_ws!=DEAD)val+=checkpawn(b,b->data.b_pawn_ws);
        if(b->in_check())val+=checkval;
        // if(checkmate(b,t))val+=checkval;
    }
    val+=evalrook(b,t);
    val+=promote*evalpawn(b,t);
    return val;
}

int MinVal(Board* b,int alpha,int beta,int depth){

    auto moveset=b->get_legal_moves();
    if(depth==0 || moveset.size()==0){
        // cout<<"Depth reached"<<endl;
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

// #define kingval 1000
// #define o_rookval 120
// #define i_rookval 100
// #define bishopval 70
// #define pawnval 50
// #define checkval 10000
// #define depthlevel 2
// #define promote 3
// #define dist 10



// constexpr U8 cw_90[64] = {
//     48, 40, 32, 24, 16, 8,  0,  7,
//     49, 41, 33, 25, 17, 9,  1,  15,
//     50, 42, 18, 19, 20, 10, 2,  23,
//     51, 43, 26, 27, 28, 11, 3,  31,
//     52, 44, 34, 35, 36, 12, 4,  39,
//     53, 45, 37, 29, 21, 13, 5,  47,
//     54, 46, 38, 30, 22, 14, 6,  55,
//     56, 57, 58, 59, 60, 61, 62, 63
// };

// constexpr U8 acw_90[64] = {
//      6, 14, 22, 30, 38, 46, 54, 7,
//      5, 13, 21, 29, 37, 45, 53, 15,
//      4, 12, 18, 19, 20, 44, 52, 23,
//      3, 11, 26, 27, 28, 43, 51, 31,
//      2, 10, 34, 35, 36, 42, 50, 39,
//      1,  9, 17, 25, 33, 41, 49, 47,
//      0,  8, 16, 24, 32, 40, 48, 55,
//     56, 57, 58, 59, 60, 61, 62, 63
// };

// constexpr U8 cw_180[64] = {
//     54, 53, 52, 51, 50, 49, 48, 7,
//     46, 45, 44, 43, 42, 41, 40, 15,
//     38, 37, 18, 19, 20, 33, 32, 23,
//     30, 29, 26, 27, 28, 25, 24, 31,
//     22, 21, 34, 35, 36, 17, 16, 39,
//     14, 13, 12, 11, 10,  9,  8, 47,
//      6,  5,  4,  3,  2,  1,  0, 55,
//     56, 57, 58, 59, 60, 61, 62, 63
// };


// void _flip_player(Board *b) {
//     b->data.player_to_play = (PlayerColor)(b->data.player_to_play ^ (WHITE | BLACK));
// }


// void _undo_last_move(U16 move,Board* b) {

//     U8 p0 = getp0(move);
//     U8 p1 = getp1(move);
//     U8 promo = getpromo(move);

//     U8 piecetype = b->data.board_0[p1];
//     U8 deadpiece = b->data.last_killed_piece;
//     b->data.last_killed_piece = 0;

//     // scan and get piece from coord
//     U8 *pieces = (U8*)(&(b->data));
//     for (int i=0; i<12; i++) {
//         if (pieces[i] == p1) {
//             pieces[i] = p0;
//             break;
//         }
//     }
//     if (b->data.last_killed_piece_idx >= 0) {
//         pieces[b->data.last_killed_piece_idx] = p1;
//         b->data.last_killed_piece_idx = -1;
//     }

//     if (promo == PAWN_ROOK) {
//         piecetype = (piecetype & (WHITE | BLACK)) | ROOK;
//     }
//     else if (promo == PAWN_BISHOP) {
//         piecetype = (piecetype & (WHITE | BLACK)) | BISHOP;
//     }

//     b->data.board_0[p0]           = piecetype;
//     b->data.board_90[cw_90[p0]]   = piecetype;
//     b->data.board_180[cw_180[p0]] = piecetype;
//     b->data.board_270[acw_90[p0]] = piecetype;

//     b->data.board_0[p1]           = deadpiece;
//     b->data.board_90[cw_90[p1]]   = deadpiece;
//     b->data.board_180[cw_180[p1]] = deadpiece;
//     b->data.board_270[acw_90[p1]] = deadpiece;

// }


// int manhatten(U8 pos,int a,int b){
//     int x=getx(pos),y=gety(pos);
//     return abs(x-a)+abs(y-b);
// }

// int evalpawn(Board *b,bool t){
//     // t-1 for white else 0;
//     int val=0;
//     if(!t){
//         val=dist;
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
//         val=dist;
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
// int getrookval(U8 pos){
//     int val=0;
//     int x=getx(pos),y=gety(pos);
//     if(x==0 || x==6 || y==0 || y==6)val+=o_rookval;
//     else val+=i_rookval;
//     return val;
// }
// int evalrook(Board* b,bool t){
//     int val=0;
//     if(t){
//         if(b->data.w_rook_bs!=DEAD)val+=getrookval(b->data.w_rook_bs);
//         if(b->data.w_rook_ws!=DEAD)val+=getrookval(b->data.w_rook_ws);
//     }else{
//         if(b->data.b_rook_bs!=DEAD)val+=getrookval(b->data.b_rook_bs);
//         if(b->data.b_rook_ws!=DEAD)val+=getrookval(b->data.b_rook_ws);
//     }
//     return val;
// }


// int evaluate(Board* b,bool t){
//     int val=0;
//     if(t){
//         if(b->data.w_king!=DEAD)val+=kingval;
//         if(b->data.w_bishop!=DEAD)val+=bishopval;
//         if(b->data.w_pawn_bs!=DEAD)val+=pawnval;
//         if(b->data.w_pawn_ws!=DEAD)val+=pawnval;
//         if(b->in_check())val-=checkval;
//         // if(checkmate(b,t))val+=checkval;
//     }else{
//         if(b->data.b_king!=DEAD)val+=kingval;
//         if(b->data.b_bishop!=DEAD)val+=bishopval;
//         if(b->data.b_pawn_bs!=DEAD)val+=pawnval;
//         if(b->data.b_pawn_ws!=DEAD)val+=pawnval;
//         if(b->in_check())val-=checkval;
//         // if(checkmate(b,t))val+=checkval;
//     }
//     val+=evalrook(b,t);
//     val+=promote*evalpawn(b,t);
//     return val;
// }

// int MinVal(Board* b,int alpha,int beta,int depth){

//     auto moveset=b->get_legal_moves();
//     if(depth==0 || moveset.size()==0){
//         cout<<"Depth reached"<<endl;
//         if(b->data.player_to_play==WHITE)
//         return -evaluate(b,true)+evaluate(b,false);
//         else
//         return -evaluate(b,false)+evaluate(b,true);
//     }
//     int val=INT_MAX;
//     for(auto move: moveset){
//         auto copyb=b->copy();
//         copyb->do_move(move);
//         val=min(val,MaxVal(copyb,alpha,beta,depth-1));
//         beta=min(beta,val);
//         _undo_last_move(move,b);
//         _flip_player(b);
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
//         _undo_last_move(move,b);
//         _flip_player(b);
//         if(alpha>=beta) return val;
//     }
//     return val;
// }

// void Engine::find_best_move(const Board& b) {

//     // auto startTime = std::chrono::high_resolution_clock::now();
//     auto moveset = b.get_legal_moves();
//     if (moveset.size() == 0) {
//         this->best_move = 0;
//         return;
//     }

//     else {
//         // priority_queue<pair<int,U16>> bestmove;
//         U16 best_move=*moveset.begin();
//         int mx=INT_MIN;
//         Board* c=b.copy();
//         for(auto move : moveset){
//             c->do_move(move);
//             int val=MinVal(c,INT_MIN,INT_MAX,depthlevel);
//             if(val>mx){
//                 best_move=move;
//                 mx=val;
//             }
//             _undo_last_move(move,c);
//             _flip_player(c);
//         }
//         c->do_move(best_move);
//         delete c;
//         this->best_move=best_move;
        
//     }
// }




