#include <algorithm>
#include<queue>
#include <random>
#include <iostream>
#include <set>
#include <chrono>
using namespace std;;

#include "board.hpp"
#include "engine.hpp"


int MaxVal(Board*b,int alpha,int beta,int depth,int deeplevel,Engine*e);
int MinVal(Board*b,int alpha,int beta,int depth,int deeplevel,Engine*e);

#define kingval 1000

#define Global 100000
#define rookval 200
#define bishopval 100
#define pawnval 60
#define checkval 100
#define checkmate 100000
#define depthlevel 6
#define promote 3
#define dist 20


int nodes=0;

PlayerColor playertoplay;


int manhatten(U8 pos,int a,int b){
    int x=getx(pos),y=gety(pos);
    return abs(x-a)+abs(y-b);
}



// int checkpawn(Board* b, U8 pos){
//     U8 pawn=b->data.board_0[pos];
//     if(pawn & PAWN) return pawnval;
//     if(pawn & PAWN_BISHOP) return bishopval;
//     if(pawn & PAWN_ROOK) return rookval;
//     return 0;
// }

int evalpawn(Board* b){
    int val1=0;
    int val2=0;

    // for white
    if(b->data.w_pawn_bs!=DEAD){
        U8 pos=b->data.w_pawn_bs;
        val1+=pawnval;

        int val=0;
        int x=getx(pos),y=gety(pos);
        val+=min(manhatten(pos,4,5),manhatten(pos,4,6));
        if(x==2 && y==0)val+=2;

        val1+=abs(dist-val)*promote; 
    }
    if(b->data.w_pawn_ws!=DEAD){
        U8 pos=b->data.w_pawn_ws;
        val1+=pawnval;

        int val=0;
        int x=getx(pos),y=gety(pos);
        val+=min(manhatten(pos,4,5),manhatten(pos,4,6));
        if(x==2 && y==1)val+=2;

        val1+=abs(dist-val)*promote; 
    }

    // for black

    if(b->data.b_pawn_bs!=DEAD){
        U8 pos=b->data.b_pawn_bs;

        val2+=pawnval;

        int val=0;
        int x=getx(pos),y=gety(pos);
        val+=min(manhatten(pos,2,1),manhatten(pos,2,0));
        if(x==4 && y==6)val+=2;

        val2+=abs(dist-val)*promote; 
    }
    if(b->data.b_pawn_ws!=DEAD){
        U8 pos=b->data.b_pawn_ws;

        val2+=pawnval;

        int val=0;
        int x=getx(pos),y=gety(pos);
        val+=min(manhatten(pos,2,1),manhatten(pos,2,0));
        if(x==4 && y==5)val+=2;

        val2+=abs(dist-val)*promote; 
    }

    if(playertoplay==WHITE)return val1-val2;
    return val2-val1;

}


int check(Board* b){
    int val=0;
    if(b->data.player_to_play==playertoplay){
        if(b->in_check())val-=checkval;
    }else{
        if(b->in_check())val+=checkval;
    }
    return val;
}

int evaluate(Board* b){
    int val1=0;
    int val2=0;

    // for white
    if(b->data.w_king!=DEAD)val1+=kingval;
    if(b->data.w_bishop!=DEAD)val1+=bishopval;
    if(b->data.w_rook_bs!=DEAD)val1+=rookval;
    if(b->data.w_rook_ws!=DEAD)val1+=rookval;

    // for black
    if(b->data.b_king!=DEAD)val2+=kingval;
    if(b->data.b_bishop!=DEAD)val2+=bishopval;
    if(b->data.b_rook_bs!=DEAD)val2+=rookval;
    if(b->data.b_rook_ws!=DEAD)val2+=rookval;
    
    
    if(playertoplay==WHITE)return val1-val2;
    return val2-val1;
}

int utility_val(Board* b){
    int val=evaluate(b)+evalpawn(b)+check(b);
    return val;
}




int MinVal(Board* b,int alpha,int beta,int depth,int deeplevel,Engine*e){
    nodes++;
    if(depth==deeplevel){

        return utility_val(b);
    }

    auto moveset=b->get_legal_moves();

    if(moveset.size()==0) return utility_val(b);

    int val=INT_MAX;
    for(auto move: moveset){
        auto copyb=b->copy();
        copyb->do_move(move);
        val=min(val,MaxVal(copyb,alpha,beta,depth+1,deeplevel,e));
        beta=min(beta,val);
        delete copyb;
        if(alpha>=beta)return val;
    }
    return val;
}

int MaxVal(Board* b,int alpha,int beta,int depth,int deeplevel,Engine*e){
        nodes++;

    if(depth==deeplevel){

        return utility_val(b);
    }
    auto moveset=b->get_legal_moves();

    if(moveset.size()==0) return utility_val(b);
        
    int val=INT_MIN;
    for(auto move : moveset){
        auto copyb=b->copy();
        copyb->do_move(move);
        val=max(val,MinVal(copyb,alpha,beta,depth+1,deeplevel,e));
        alpha=max(alpha,val);
        delete copyb;
        if(alpha>=beta) return val;
    }
    return val;
}

U16 BestMove(Board* b,unordered_set<U16>& moveset,Engine* e){
    U16 best_move=*moveset.begin();
    int mx=INT_MIN;
    playertoplay=b->data.player_to_play;
    nodes=1;

    int alpha=INT_MIN;
    for(auto move : moveset){
        Board* c = b->copy();
        c->do_move(move);
        int val=MinVal(c,alpha,INT_MAX,1,depthlevel,e);
        alpha=max(alpha,val);
        if(val>mx){
            best_move=move;
            mx=val;
        }
        delete c;
    }
    b->do_move(best_move);
    return best_move;
}

void Engine::find_best_move(const Board& b) {
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        this->best_move = 0;
        return;
    }
    else {

        Board* c=b.copy();
        nodes=0;
        U16 best_move=BestMove(c,moveset,this);
        delete c;
        cout<<"final no of nodes processed "<<nodes<<endl;
        this->best_move=best_move;
    }
}
