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




