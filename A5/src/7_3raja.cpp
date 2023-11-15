#include <algorithm>
#include <queue>
#include <random>
#include <iostream>
#include<thread>
#include <set>
#include <chrono>
using namespace std;;

#include "board.hpp"
#include "engine.hpp"


int MaxVal(Board*b,int alpha,int beta,int depth,int deeplevel,Engine*e);
int MinVal(Board*b,int alpha,int beta,int depth,int deeplevel,Engine*e);

#define kingval 1000

#define Global 100000
#define rookval 2000
#define bishopval 1000
#define pawnval 600
#define checkval 1000
#define checkmate 100000
#define promote 3
#define dist73 60
#define dist84 0
#define knightval 1500
#define lambda 20

int depthlevel=4;
int nodes=0;
int counter=0;

PlayerColor playertoplay;

int pro_73_white[]={

    46, 48, 51, 0, 0, 0, 0, 0,
    42, 42, 44, 0, 0, 0, 0, 0,
    36, 36,  0, 0, 0, 0, 0, 0,
    30, 30,  0, 0, 0, 0, 0, 0,
    27, 21,  0, 0, 0, 0, 0, 0,
    24, 18, 12, 6, 0, 0, 0, 0,
    24, 18, 12, 6, 0, 0, 0, 0,
     0,  0,  0, 0, 0, 0, 0, 0
};

int pro_73_black[]={

    0, 0, 0, 6, 12, 18, 24, 0,
    0, 0, 0, 6, 12, 18, 24, 0,
    0, 0, 0, 0,  0, 21, 27, 0,
    0, 0, 0, 0,  0, 30, 30, 0,
    0, 0, 0, 0,  0, 36, 36, 0,
    0, 0, 0, 0, 44, 42, 42, 0,
    0, 0, 0, 0, 51, 48, 46, 0,
    0, 0, 0, 0,  0,  0,  0, 0
};

double pro_84_white[]={

    20, 16, 12, 8, 4, 0, 0, 0,
    18, 16, 12, 8, 4, 0, 0, 0,
    22, 16.8,0, 0, 0, 0, 0, 0,
    24, 24,  0, 0, 0, 0, 0, 0,
    28, 28,  0, 0, 0, 0, 0, 0,
    32, 32,  0, 0, 0, 0, 0, 0,
    36, 36, 38,44,48, 52, 0, 0,
    40, 40, 42,44,48, 52, 0, 0,
};

double pro_84_black[]={

    0, 0,52,48,44,42, 40, 40,
    0, 0,52,48,44,38, 36, 36,
    0, 0, 0, 0, 0, 0, 32, 32,
    0, 0, 0, 0, 0, 0, 28, 28,
    0, 0, 0, 0, 0, 0, 24, 24,
    0, 0, 0, 0, 0, 0,16.8,22,
    0, 0, 0, 4, 8,12, 16, 18,
    0, 0, 0, 4, 8,12, 16, 20
};

int manhatten(U8 pos,int a,int b){

    int x= getx(pos) , y= gety(pos) ;
    return abs(x-a) +  abs(y-b) ;

}
int pawn_pro_8_4(Board* b){

    int val1=0, val2=0 ;

    if(b->data.w_pawn_1!=DEAD){

        U8 pos=b->data.w_pawn_1;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist84-(int)pro_84_white[pos] ) * promote; 
    }
    if(b->data.w_pawn_2!=DEAD){

        U8 pos=b->data.w_pawn_2;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist84-(int)pro_84_white[pos] ) * promote; 
    }
    if(b->data.w_pawn_3!=DEAD){

        U8 pos=b->data.w_pawn_3;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist84-(int)pro_84_white[pos] ) * promote; 
    }
    if(b->data.w_pawn_4!=DEAD){

        U8 pos=b->data.w_pawn_4;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist84-(int)pro_84_white[pos] ) * promote; 
    }

    // for black
    if(b->data.b_pawn_1!=DEAD){

        U8 pos=b->data.b_pawn_1;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist84-(int)pro_84_black[pos] ) * promote;
    }
    if(b->data.b_pawn_2!=DEAD){

        U8 pos=b->data.b_pawn_2;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist84-(int)pro_84_black[pos] ) * promote; 
    }
    if(b->data.b_pawn_3!=DEAD){

        U8 pos=b->data.b_pawn_3;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist84-(int)pro_84_black[pos] ) * promote;
    }
    if(b->data.b_pawn_4!=DEAD){

        U8 pos=b->data.b_pawn_4;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist84-(int)pro_84_black[pos] ) * promote; 
    }

    if( playertoplay==WHITE ) return val1-val2 ;

    return val2-val1 ;
}

int pawn_pro_7_3(Board* b){

    int val1=0, val2=0 ;

    if(b->data.w_pawn_1!=DEAD){

        U8 pos=b->data.w_pawn_1;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist73-pro_73_white[pos] ) * promote; 
    }
    if(b->data.w_pawn_2!=DEAD){

        U8 pos=b->data.w_pawn_2;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist73-pro_73_white[pos] ) * promote; 
    }

    // for black
    if(b->data.b_pawn_1!=DEAD){

        U8 pos=b->data.b_pawn_1;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist73-pro_73_black[pos] ) * promote;
    }
    if(b->data.b_pawn_2!=DEAD){

        U8 pos=b->data.b_pawn_2;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist73-pro_73_black[pos] ) * promote; 
    }

    if( playertoplay==WHITE ) return val1-val2 ;

    return val2-val1 ;
}

int checkpawn( Board* b , U8 pos) {

    U8 pawn = b->data.board_0 [ pos ] ;

    if( pawn & PAWN   ) return pawnval   ; 
    if( pawn & BISHOP ) return bishopval ;
    if( pawn & ROOK   ) return rookval   ;

    return 0 ;
}



int evalpawn(Board* b){

    return pawn_pro_7_3(b);

}


int check(Board* b){

    int val = 0 ;

    if(b->data.player_to_play==playertoplay){

        if(b->in_check()) val -= checkval ;
    }
    else{

        if(b->in_check()) val += checkval ;
    }
    return val ;
}

int evaluate(Board* b){
    int val1 = 0 ;
    int val2 = 0 ;

    // for white
    if(b-> data.w_king     != DEAD ) val1 += kingval                            ;
    if(b-> data.w_bishop   != DEAD ) val1 += bishopval                          ;
    if(b-> data.w_rook_1   != DEAD ) val1 += rookval                            ;
    if(b-> data.w_rook_2   != DEAD ) val1 += rookval                            ;
    if(b-> data.w_knight_1 != DEAD ) val1 += knightval                          ;
    if(b-> data.w_knight_2 != DEAD ) val1 += knightval                          ;
    if(b-> data.w_pawn_1   != DEAD ) val1 += checkpawn (b , b-> data.w_pawn_1 ) ;
    if(b-> data.w_pawn_2   != DEAD ) val1 += checkpawn (b , b-> data.w_pawn_2 ) ;
    if(b-> data.w_pawn_3   != DEAD ) val1 += checkpawn (b , b-> data.w_pawn_3 ) ;
    if(b-> data.w_pawn_4   != DEAD ) val1 += checkpawn (b , b-> data.w_pawn_4 ) ;

    // for black
    if(b-> data.b_king     != DEAD ) val2 += kingval                            ;
    if(b-> data.b_bishop   != DEAD ) val2 += bishopval                          ;
    if(b-> data.b_rook_1   != DEAD ) val2 += rookval                            ;
    if(b-> data.b_rook_2   != DEAD ) val2 += rookval                            ;
    if(b-> data.b_knight_1 != DEAD ) val2 += knightval                          ;
    if(b-> data.b_knight_2 != DEAD ) val2 += knightval                          ;
    if(b-> data.b_pawn_1   != DEAD ) val2 += checkpawn (b , b-> data.b_pawn_1 ) ;
    if(b-> data.b_pawn_2   != DEAD ) val2 += checkpawn (b , b-> data.b_pawn_2 ) ;
    if(b-> data.b_pawn_3   != DEAD ) val2 += checkpawn (b , b-> data.b_pawn_3 ) ;
    if(b-> data.b_pawn_4   != DEAD ) val2 += checkpawn (b , b-> data.b_pawn_4 ) ;
    
    
    if ( playertoplay == WHITE ) return val1 - val2 ;

    return val2 - val1 ;
}

int utility_val( Board* b ) {

    int val = evaluate ( b ) + evalpawn( b ) + check( b ) ;

    return val ;
}




int MinVal( Board* b, int alpha, int beta, int depth, int deeplevel, Engine*e){
    nodes++;
    if(depth==deeplevel){
        auto moveset = b->get_legal_moves() ;
        int val=utility_val(b)              ;
        int extra=0                         ;
        for (auto move : moveset){
            Board *copyb=new Board(*b)      ;

            copyb-> do_move_(move)          ;
            int ut = utility_val(copyb)     ;

            extra += ( ut - val ) / lambda  ;
        }
        return val+extra                    ;

    }

    auto moveset=b->get_legal_moves()       ;

    if(moveset.size()==0){
        if(b->in_check()) return INT_MAX    ;   
        return INT_MIN                      ;
    }

    vector< pair< int, Board* > > Move_Order;

    for( auto move : moveset ) {
        Board *copyb=new Board(*b);

        copyb-> do_move_(move)    ;
        int ut=utility_val(copyb) ;
        Move_Order.push_back( { ut, copyb } ) ;
    }

    sort(Move_Order.begin(), Move_Order.end(), [&](const pair<int, Board*> &p, const pair<int, Board*> &q){
        if( p.first < q.first ) return true ;
        return false ;
    });

    int val=INT_MAX;

    for(auto [move,copyb] : Move_Order){

        val  = min( val, MaxVal( copyb, alpha, beta, depth+1, deeplevel, e ) )  ;
        beta = min( beta, val )                                                 ;
        delete copyb                                                            ;
        if( alpha >= beta ) return val                                          ;
    }

    return val;
}

int MaxVal(Board* b,int alpha,int beta,int depth,int deeplevel,Engine*e){
    nodes++;

    if(depth==deeplevel){

        auto moveset = b->get_legal_moves() ;
        int val=utility_val(b)              ;
        int extra=0                         ;
        for (auto move : moveset){
            Board *copyb=new Board(*b)      ;

            copyb-> do_move_(move)          ;
            int ut = utility_val(copyb)     ;

            extra += ( ut - val ) / lambda  ;
        }

        return val+extra                    ;

    }

    auto moveset=b->get_legal_moves()       ;

    if(moveset.size()==0){

        if(b->in_check()) return INT_MIN    ;

        return INT_MAX                      ;
    }

    vector< pair< int, Board* > > Move_Order;

    for( auto move : moveset ) {

        Board *copyb=new Board(*b)          ;

        copyb-> do_move_(move)              ;
        int ut=utility_val(copyb)           ;
        Move_Order.push_back( { ut, copyb } ) ;

    }

    sort(Move_Order.begin(),Move_Order.end(),[&](const pair<int,Board*> &p,const pair<int,Board*> &q){

        if ( p.first > q.first ) return true;
        return false;

    });

    int val=INT_MIN;
    for(auto [move,copyb] : Move_Order){

        val=max(val, MinVal(copyb, alpha, beta, depth+1, deeplevel, e));

        alpha = max( alpha, val );
        delete copyb;
        if(alpha >= beta) return val;
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

        Board *copyb=new Board(*b);
        copyb->do_move_(move);
        int val=MinVal(copyb,alpha,INT_MAX,1,depthlevel,e);
        alpha=max(alpha,val);
        if(val>mx){
            best_move=move;
            mx=val;
        }
        delete copyb;
    }
    b->do_move_(best_move);
    return best_move;
}

void Engine::find_best_move(const Board& b) {
    auto moveset = b.get_legal_moves();

    if (moveset.size() == 0) {

        this->best_move = 0;
        return;
    }
    else {

        Board* c=new Board(b);
        nodes=0;
        U16 best_move=BestMove(c,moveset,this);

        this->best_move=best_move;
        delete c;
        counter++;
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}
