#include <algorithm>
#include <queue>
#include <random>
#include <iostream>
#include<thread>
#include <set>
#include <chrono>
#include<climits>
#include<unordered_map>
using namespace std;;

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"
#include "butils.cpp"


unordered_map<string,int> board_map;


int MaxVal(Board*b,int alpha,int beta,int depth,int deeplevel,Engine*e);
int MinVal(Board*b,int alpha,int beta,int depth,int deeplevel,Engine*e);
int MaxVal82(Board*b,int alpha,int beta,int depth,int deeplevel,Engine*e);
int MinVal82(Board*b,int alpha,int beta,int depth,int deeplevel,Engine*e);

#define kingval 1000

#define Global    103400
#define rookval   2030
#define bishopval 1020
#define pawnval 610
#define checkval 1030
#define checkmate 100200
#define promote 3
#define dist73 61
#define dist84 91
#define dist82 79
#define knightval 1501
#define lambda 20

int b_factor82 = 10 ;
int b_depth82 = 2 ;
int b_factor_73_84 = INT_MAX;
int b_depth = 2 ;

int depthlevel82=4;
int depthlevel=4;
int nodes=0;
int counter=0;

PlayerColor playertoplay;

U8 lastmove=0;


void fixdepth(Board * b){
    return;
    int bishop=5;
    int rook=5;
    int pawn=1;
    int knight=4;
    int king=2;
    if(b->data.board_type==EIGHT_TWO)return;
    int p=0;

    for(int i=0;i<64;i++){
        if(b->data.board_0[i] & PAWN)p+=pawn;
        else if(b->data.board_0[i] & BISHOP)p+=bishop;
        else if(b->data.board_0[i] & ROOK)p+=rook;
        else if(b->data.board_0[i] & KING)p+=king;
        else if(b->data.board_0[i] & KNIGHT)p+=knight;
    }



    cout<<" pieces "<<p<<endl;
    if(p<=5){
        depthlevel=6;
        b_factor_73_84=10;
    }
    depthlevel=4;
    b_factor_73_84=INT_MAX;
}


int pro_73_white1 []= {
    21, -3, -38, -5, -30, -4, -55, -86, 32, -43, -25, -56, -56, -98, -71, -67, -18, -13, -82, -22, -70, -8, -63, -33, -42, 21, -59, -56, -43, -27, -3, -63, -59, 3, -89, -60, -78, -26, -40, -67, -27, -75, -10, -14, -54, -31, -59, -23, -27, -38, -60, -87, -58, -45, -77, -76, -49, -25, -42, -7, -32, -72, -98, -37
};
int pro_73_white2 []= {
    25, 51, 89, 5, 30, 4, 55, 86, 10, 85, 69, 56, 56, 98, 71, 67, 54, 49, 82, 22, 70, 8, 63, 33, 72, 9, 59, 56, 43, 27, 3, 63, 86, 18, 89, 60, 78, 26, 40, 67, 51, 93, 22, 20, 54, 31, 59, 23, 51, 56, 72, 93, 58, 45, 77, 76, 49, 25, 42, 7, 32, 72, 98, 37
};
int pro_73_black1 [] = {
    -18, -31, -72, -84, 0, -8, -36, -85, -52, -93, -67, -18, -31, 0, -39, -79, -78, -70, -2, -50, -12, -51, -65, -9, -90, -80, -41, -97, -18, 20, -7, -74, -2, -10, -77, -24, -24, -25, 6, -27, -9, -32, -25, -13, -19, -15, 14, -78, -99, -27, -36, -86, 45, -6, -48, -93, -10, -6, -11, -80, -49, -67, -44, -54
};
int pro_73_black2 []={
    18, 31, 72, 90, 12, 26, 60, 85, 52, 93, 67, 24, 43, 18, 63, 79, 78, 70, 2, 50, 12, 72, 92, 9, 90, 80, 41, 97, 18, 10, 37, 74, 2, 10, 77, 24, 24, 61, 30, 27, 9, 32, 25, 13, 63, 57, 28, 78, 99, 27, 36, 86, 6, 54, 94, 93, 10, 6, 11, 80, 49, 67, 44, 54
};
int pro_84_white1 []={
    61, 66, -11, 49, 18, -9, -73, -60, -25, 40, 16, 48, 15, 75, -5, -78, 21, -7, -21, -58, -31, -61, -63, -51, -42, -9, -38, -70, -67, -100, -31, -42, -42, -32, -20, -45, -80, -8, -8, -3, -6, 22, -41, -95, -29, -16, -39, -60, 19, -70, -32, -62, -54, -36, -53, -1, -45, -36, 11, -22, -90, -83, -4, -63
};
int pro_84_white2 []={
    5, 3, 83, 28, 62, 93, 73, 60, 85, 20, 47, 27, 64, 6, 5, 78, 33, 61, 21, 58, 31, 61, 63, 51, 90, 57, 38, 70, 67, 100, 31, 42, 84, 74, 20, 45, 80, 8, 8, 3, 42, 5, 41, 95, 29, 16, 39, 60, 14, 94, 50, 74, 60, 36, 53, 1, 75, 60, 7, 34, 96, 83, 4, 63
};
int pro_84_black1 []={
    -58, -59, -82, -7, -81, -40, -74, -42, -80, -74, -8, -94, -76, -57, -12, 13, -77, -92, -12, -27, -14, -17, 12, 5, -45, -94, -23, -40, -64, -88, -36, -15, -88, -85, -54, -82, -3, -75, -19, 29, -90, -89, -80, -29, -74, -16, 52, 7, -18, -49, -2, -6, 16, 39, 3, 27, -14, -53, 11, 6, 68, -12, -8, 8
};
int pro_84_black2 []={
    58, 59, 82, 13, 93, 58, 98, 72, 80, 74, 8, 100, 88, 75, 36, 20, 77, 92, 12, 27, 14, 17, 15, 31, 45, 94, 23, 40, 64, 88, 78, 57, 88, 85, 54, 82, 3, 75, 67, 19, 90, 89, 80, 29, 74, 16, 2, 47, 18, 49, 83, 85, 59, 24, 57, 33, 14, 53, 73, 74, 9, 84, 77, 58
};
int pro_82_white1 []={
    -32, 10, 12, 1, -91, -68, -33, -2, 18, 2, 16, -38, 54, -55, -30, -13, -40, 35, -42, 29, -30, -23, -52, -67, -4, 30, 22, -37, -27, -87, -74, -97, 12, -9, -59, -63, -55, -55, -65, -8, -60, 14, -10, -55, -37, -25, -11, -80, -55, 12, -67, -75, -65, -5, -61, -28, -25, -62, 3, -57, -73, -79, -25, -35
};
int pro_82_white2 []={
    83, 42, 43, 59, 91, 68, 33, 2, 30, 44, 31, 94, 9, 55, 30, 13, 85, 7, 85, 16, 95, 93, 52, 67, 45, 6, 17, 37, 27, 87, 74, 97, 28, 44, 80, 63, 55, 55, 65, 8, 99, 17, 30, 67, 37, 25, 11, 80, 91, 12, 82, 81, 65, 5, 61, 28, 58, 92, 17, 69, 73, 79, 25, 35
};
int pro_82_black1 []={
    -70, -100, -83, -62, -12, -67, -15, -28, -14, -1, -28, -58, -9, -45, -2, -28, -54, -50, -77, -12, -37, 3, 19, -16, -71, -60, -77, -46, -29, -75, 19, 28, -42, -66, -99, -1, -25, 31, -38, 35, -61, -90, 46, -23, -19, 7, -3, 14, -25, -27, -76, -34, 39, -6, -39, -6, -90, -54, -98, -72, 18, 9, -11, -46
};
int pro_82_black2 []={
    70, 100, 83, 62, 24, 87, 45, 61, 14, 1, 28, 58, 15, 60, 26, 64, 54, 50, 77, 12, 49, 17, 12, 55, 71, 60, 77, 46, 29, 96, 16, 12, 42, 66, 99, 1, 25, 8, 74, 6, 61, 90, 24, 88, 64, 36, 45, 31, 25, 27, 76, 97, 17, 53, 85, 54, 90, 54, 98, 72, 42, 46, 63, 97
};

int manhatten(U8 pos,int a,int b){

    int x= getx(pos) , y= gety(pos) ;
    return abs(x-a) +  abs(y-b) ;

}

int pawn_pro_8_4(Board* b){

    int val1=0, val2=0 ;

    if(b->data.w_pawn_1!=DEAD){

        U8 pos=b->data.w_pawn_1;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist84-(int)(pro_84_white1[pos]+pro_84_white2[pos]) ) * promote; 
    }
    if(b->data.w_pawn_2!=DEAD){

        U8 pos=b->data.w_pawn_2;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist84-(int)(pro_84_white1[pos]+pro_84_white2[pos]) ) * promote; 
    }
    if(b->data.w_pawn_3!=DEAD){

        U8 pos=b->data.w_pawn_3;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist84-(int)(pro_84_white1[pos]+pro_84_white2[pos]) ) * promote; 
    }
    if(b->data.w_pawn_4!=DEAD){

        U8 pos=b->data.w_pawn_4;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist84-(int)(pro_84_white1[pos]+pro_84_white2[pos]) ) * promote; 
    }

    // for black
    if(b->data.b_pawn_1!=DEAD){

        U8 pos=b->data.b_pawn_1;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist84-(int)(pro_84_black1[pos]+pro_84_black2[pos]) ) * promote;
    }
    if(b->data.b_pawn_2!=DEAD){

        U8 pos=b->data.b_pawn_2;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist84-(int)(pro_84_black1[pos]+pro_84_black2[pos]) ) * promote; 
    }
    if(b->data.b_pawn_3!=DEAD){

        U8 pos=b->data.b_pawn_3;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist84-(int)(pro_84_black1[pos]+pro_84_black2[pos]) ) * promote;
    }
    if(b->data.b_pawn_4!=DEAD){

        U8 pos=b->data.b_pawn_4;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist84-(int)(pro_84_black1[pos]+pro_84_black2[pos]) ) * promote; 
    }

    if( playertoplay==WHITE ) return val1-val2 ;

    return val2-val1 ;
}

int pawn_pro_8_2(Board* b){

    int val1=0, val2=0 ;

    if(b->data.w_pawn_1!=DEAD){

        U8 pos=b->data.w_pawn_1;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist82-(int)(pro_82_white1[pos]+pro_82_white2[pos]) ) * promote; 
    }
    if(b->data.w_pawn_2!=DEAD){

        U8 pos=b->data.w_pawn_2;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist82-(int)(pro_82_white1[pos]+pro_82_white2[pos]) ) * promote; 
    }
    if(b->data.w_pawn_3!=DEAD){

        U8 pos=b->data.w_pawn_3;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist82-(int)(pro_82_white1[pos]+pro_82_white2[pos]) ) * promote; 
    }
    if(b->data.w_pawn_4!=DEAD){

        U8 pos=b->data.w_pawn_4;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist82-(int)(pro_82_white1[pos]+pro_82_white2[pos]) ) * promote; 
    }

    // for black
    if(b->data.b_pawn_1!=DEAD){

        U8 pos=b->data.b_pawn_1;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist82-(int)(pro_82_black1[pos]+pro_82_black2[pos]) ) * promote;
    }
    if(b->data.b_pawn_2!=DEAD){

        U8 pos=b->data.b_pawn_2;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist82-(int)(pro_82_black1[pos]+pro_82_black2[pos]) ) * promote; 
    }
    if(b->data.b_pawn_3!=DEAD){

        U8 pos=b->data.b_pawn_3;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist82-(int)(pro_82_black1[pos]+pro_82_black2[pos]) ) * promote;
    }
    if(b->data.b_pawn_4!=DEAD){

        U8 pos=b->data.b_pawn_4;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist82-(int)(pro_82_black1[pos]+pro_82_black2[pos]) ) * promote; 
    }

    if( playertoplay==WHITE ) return val1-val2 ;

    return val2-val1 ;
}

int pawn_pro_7_3(Board* b){

    int val1=0, val2=0 ;

    if(b->data.w_pawn_1!=DEAD){

        U8 pos=b->data.w_pawn_1;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist73-(pro_73_white1[pos]+pro_73_white2[pos]) ) * promote; 
    }
    if(b->data.w_pawn_2!=DEAD){

        U8 pos=b->data.w_pawn_2;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist73-(pro_73_white1[pos]+pro_73_white2[pos]) ) * promote; 
    }

    // for black
    if(b->data.b_pawn_1!=DEAD){

        U8 pos=b->data.b_pawn_1;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist73-(pro_73_black1[pos]+pro_73_black2[pos]) ) * promote;
    }
    if(b->data.b_pawn_2!=DEAD){

        U8 pos=b->data.b_pawn_2;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist73-(pro_73_black1[pos]+pro_73_black2[pos]) ) * promote; 
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
    if( b-> data.board_type == EIGHT_FOUR  ) return pawn_pro_8_4(b);
    if( b-> data.board_type == EIGHT_TWO ) return   pawn_pro_8_2(b);
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

    int val = evaluate ( b ) + evalpawn( b ) ;

    return val ;
}

int Quesent(Board*b,int val){
    return 0;
    auto moveset = b->get_legal_moves() ;
    int extra=0                         ;
    for (auto move : moveset){
        Board *copyb=new Board(*b)      ;

        copyb-> do_move_(move)          ;
        int ut = utility_val(copyb)     ;

        extra += ( ut - val ) / lambda  ;
    }
    return extra;
}

int Quesent82(Board*b,int val){
    return 0;
    auto moveset = b->get_legal_moves() ;
    int extra=0                         ;
    for (auto move : moveset){
        Board *copyb=new Board(*b)      ;

        copyb-> do_move_(move)          ;
        int ut = utility_val(copyb)     ;

        extra += ( ut - val ) / lambda  ;
    }
    return extra;
}

int MinVal( Board* b, int alpha, int beta, int depth, int deeplevel, Engine*e){
    nodes++;

    if(depth==deeplevel){

        int val=utility_val(b)              ;

        return val                          ;

    }

    auto moveset=b->get_legal_moves()       ;

    if(moveset.size()==0){
        if(b->in_check()) return INT_MAX    ;   
        return INT_MIN                      ;
    }

    int val=INT_MAX                         ;

    for( auto move : moveset ) {
        Board *copyb=new Board(*b)          ;

        copyb-> do_move_(move)              ;

        val  = min( val, MaxVal( copyb, alpha, beta, depth+1, deeplevel, e ) )  ;
        beta = min( beta, val )             ;
        delete copyb                        ;
        if( alpha >= beta ) return val      ;
    }

    return val                              ;
}

int MaxVal(Board* b,int alpha,int beta,int depth,int deeplevel,Engine*e){
    nodes++;

    if(depth==deeplevel){

        int val=utility_val(b)              ;
        return val                          ;

    }

    auto moveset=b->get_legal_moves()       ;

    if(moveset.size()==0){

        if(b->in_check()) return INT_MIN    ;

        return INT_MAX                      ;
    }

    int val=INT_MIN                         ;
    
    for( auto move : moveset ) {

        Board *copyb=new Board(*b)          ;

        copyb-> do_move_(move)              ;

        val=max(val, MinVal(copyb, alpha, beta, depth+1, deeplevel, e));

        alpha = max( alpha, val )           ;
        delete copyb                        ;
        if(alpha >= beta) return val        ;
    }

    return val                              ;
}

int MinVal82( Board* b, int alpha, int beta, int depth, int deeplevel, Engine*e){
    nodes++;
    if(depth==deeplevel){

        int val=utility_val(b)              ;

        return val                   ;

    }

    auto moveset=b->get_legal_moves()       ;

    if(moveset.size()==0){
        if(b->in_check()) return INT_MAX    ;   
        return INT_MIN                      ;
    }


    int val=INT_MAX                         ;
    
    for( auto move : moveset ) {

        Board *copyb=new Board(*b)          ;

        copyb-> do_move_(move)              ;

        val  = min( val, MaxVal82( copyb, alpha, beta, depth+1, deeplevel, e ) )  ;
        beta = min( beta, val )             ;
        delete copyb                        ;
        if( alpha >= beta ) return val      ;
    }

    return val                              ;
}

int MaxVal82(Board* b,int alpha,int beta,int depth,int deeplevel,Engine*e){
    nodes++;

    if(depth==deeplevel){

        int val=utility_val(b)              ;

        return val                          ;
    }

    auto moveset=b->get_legal_moves()       ;

    if(moveset.size()==0){

        if(b->in_check()) return INT_MIN    ;

        return INT_MAX                      ;
    }


    int val=INT_MIN                         ;

    for( auto move : moveset ) {

        Board *copyb=new Board(*b)          ;

        copyb-> do_move_(move)              ;

        val=max(val, MinVal82(copyb, alpha, beta, depth+1, deeplevel, e));

        alpha = max( alpha, val )           ;
        delete copyb                        ;
        if(alpha >= beta) return val        ;
    }

    return val                              ;
}

U16 BestMove(Board* b,unordered_set<U16>& moveset,Engine* e){
    U16 best_move=*moveset.begin()          ;
    int mx=INT_MIN                          ;
    playertoplay=b->data.player_to_play     ;
    nodes=1                                 ;

    int alpha=INT_MIN                       ;  

    fixdepth(b)                             ;

    for(auto move : moveset){

        Board *copyb=new Board(*b)          ;
        copyb->do_move_(move)               ;

        if(board_map[board_to_str(&(copyb->data))]==1){
            continue;
        }

        int val;
        if(b->data.board_type==EIGHT_TWO) val=MinVal82(copyb,alpha,INT_MAX,1,depthlevel82,e);
        else val=val=MinVal(copyb,alpha,INT_MAX,1,depthlevel,e);
        alpha=max(alpha,val)                ;

        if ( val > mx ) {
            best_move = move                ;
            mx = val                        ;
        }
        delete copyb                        ;
    }
    cout<<"Nodes processed : "<<nodes<<endl ;
    b->do_move_(best_move)                  ;
    return best_move                        ;
}

void Engine::find_best_move(const Board& b) {
    auto moveset = b.get_legal_moves();

    if (moveset.size() == 0) {

        this->best_move = 0                 ;
        return;
    }
    else {

        Board* c=new Board(b)               ;
        nodes=0                             ;
        U16 best_move=BestMove(c,moveset,this);

        this->best_move=best_move           ;
        c->do_move_(best_move);
        board_map[board_to_str(&(c->data))]=1;
        delete c                            ;
        counter++                           ;

    }
}