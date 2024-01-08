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
int MaxVal82(Board*b,int alpha,int beta,int depth,int deeplevel,Engine*e);
int MinVal82(Board*b,int alpha,int beta,int depth,int deeplevel,Engine*e);

#define kingval 1000

#define Global    100000
#define rookval   2000
#define bishopval 1000
#define pawnval 600
#define checkval 1000
#define checkmate 100000
#define promote 3
#define dist73 60
#define dist84 80
#define dist82 80
#define knightval 1500
#define lambda 20

int b_factor82 = 10 ;
int b_depth82 = 2 ;
int b_factor_73_84 = 10 ;
int b_depth = 2 ;

int depthlevel82=4;
int depthlevel=6;
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

int pro_84_white[]={
    66,69,72,0,0,0,0,0,
    60,60,63,0,0,0,0,0,
    54,54,0,0,0,0,0,0,
    48,48,0,0,0,0,0,0,
    42,42,0,0,0,0,0,0,
    36,27,0,0,0,0,0,0,
    33,24,18,12,6,0,0,0,
    30,24,18,12,6,0,0,0
};
int pro_84_black[]={
    0,0,0,6,12,18,24,30,
    0,0,0,6,12,18,24,33,
    0,0,0,0,0,0,27,36,
    0,0,0,0,0,0,42,42,
    0,0,0,0,0,0,48,48,
    0,0,0,0,0,0,54,54,
    0,0,0,0,0,63,60,60,
    0,0,0,0,0,72,69,66
};

int pro_82_white[]={
    51,52,55,60,0,0,0,0,
    48,46,47,56,63,0,0,0,
    45,42,43,45,65,70,0,0,
    41,36,39,0,0,0,0,0,
    40,35,21,0,0,0,0,0,
    39,31,20,12,0,0,0,0,
    36,24,15,6,0,0,0,0,
    33,30,20,12,0,0,0,0
};

int pro_82_black[]={
    0,0,0,0,12,20,30,33,
    0,0,0,0,6,15,24,36,
    0,0,0,0,12,20,31,39,
    0,0,0,0,0,21,35,40,
    0,0,0,0,0,39,36,41,
    0,0,70,65,45,43,42,45,
    0,0,0,63,56,47,46,48,
    0,0,0,0,60,55,52,51
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

int pawn_pro_8_2(Board* b){

    int val1=0, val2=0 ;

    if(b->data.w_pawn_1!=DEAD){

        U8 pos=b->data.w_pawn_1;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist82-(int)pro_82_white[pos] ) * promote; 
    }
    if(b->data.w_pawn_2!=DEAD){

        U8 pos=b->data.w_pawn_2;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist82-(int)pro_82_white[pos] ) * promote; 
    }
    if(b->data.w_pawn_3!=DEAD){

        U8 pos=b->data.w_pawn_3;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist82-(int)pro_82_white[pos] ) * promote; 
    }
    if(b->data.w_pawn_4!=DEAD){

        U8 pos=b->data.w_pawn_4;
        if( b-> data.board_0[pos] & PAWN ) val1 += abs( dist82-(int)pro_82_white[pos] ) * promote; 
    }

    // for black
    if(b->data.b_pawn_1!=DEAD){

        U8 pos=b->data.b_pawn_1;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist82-(int)pro_82_black[pos] ) * promote;
    }
    if(b->data.b_pawn_2!=DEAD){

        U8 pos=b->data.b_pawn_2;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist82-(int)pro_82_black[pos] ) * promote; 
    }
    if(b->data.b_pawn_3!=DEAD){

        U8 pos=b->data.b_pawn_3;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist82-(int)pro_82_black[pos] ) * promote;
    }
    if(b->data.b_pawn_4!=DEAD){

        U8 pos=b->data.b_pawn_4;
        if( b-> data.board_0[pos] & PAWN ) val2 += abs( dist82-(int)pro_82_black[pos] ) * promote; 
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
    // return 0;
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
        int extra=Quesent(b,val)            ;
        // if(playertoplay&(1<<6))cout<<"\n\n\n\n\n\n\n"<<"white"<<" : "<<depth<<" "<<"\n\n\n\n\n\n";
        // else cout<<"\n\n\n\n\n\n\n"<<"black"<<" : "<<depth<<" "<<"\n\n\n\n\n\n";
        return val+extra                    ;

    }

    auto moveset=b->get_legal_moves()       ;

    if(moveset.size()==0){
        if(b->in_check()) return INT_MAX    ;   
        return INT_MIN                      ;
    }

    vector< pair< int, Board* > > Move_Order;

    for( auto move : moveset ) {
        Board *copyb=new Board(*b)          ;

        copyb-> do_move_(move)              ;
        int ut=utility_val(copyb)           ;
        Move_Order.push_back({ ut, copyb }) ;
    }

    sort(Move_Order.begin(), Move_Order.end(), [&](const pair<int, Board*> &p, const pair<int, Board*> &q){

        if( p.first < q.first ) return true ;
        return false                        ;

    })                                      ;

    int val=INT_MAX                         ;
    int child=0;

    for(auto [move,copyb] : Move_Order){

        if(depth>=b_depth && child>b_factor_73_84)break;

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
        int extra=Quesent(b,val)            ;
        // if(playertoplay&(1<<6))cout<<"\n\n\n\n\n\n\n"<<"white"<<" : "<<depth<<" "<<"\n\n\n\n\n\n";
        // else cout<<"\n\n\n\n\n\n\n"<<"black"<<" : "<<depth<<" "<<"\n\n\n\n\n\n";
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
        Move_Order.push_back({ ut, copyb }) ;

    }

    sort(Move_Order.begin(),Move_Order.end(),[&](const pair<int,Board*> &p,const pair<int,Board*> &q){

        if ( p.first > q.first ) return true;
        return false                        ;

    })                                      ;

    int val=INT_MIN                         ;
    int child=0;
    for(auto [move,copyb] : Move_Order){

        if(depth>=b_depth && child>b_factor_73_84)break;
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
        int extra=Quesent(b,val)            ;
        // if(playertoplay&(1<<6))cout<<"\n\n\n\n\n\n\n"<<"white"<<" : "<<depth<<" "<<"\n\n\n\n\n\n";
        // else cout<<"\n\n\n\n\n\n\n"<<"black"<<" : "<<depth<<" "<<"\n\n\n\n\n\n";

        return val+extra                    ;

    }

    auto moveset=b->get_legal_moves()       ;

    if(moveset.size()==0){
        if(b->in_check()) return INT_MAX    ;   
        return INT_MIN                      ;
    }

    vector< pair< int, Board* > > Move_Order;
    
    for( auto move : moveset ) {
        Board *copyb=new Board(*b)          ;

        copyb-> do_move_(move)              ;
        int ut=utility_val(copyb)           ;
        Move_Order.push_back({ ut, copyb }) ;
    }

    sort(Move_Order.begin(), Move_Order.end(), [&](const pair<int, Board*> &p, const pair<int, Board*> &q){

        if( p.first < q.first ) return true ;
        return false                        ;

    })                                      ;

    int val=INT_MAX                         ;
    int child=0;
    for(auto [move,copyb] : Move_Order){

        if(depth>=b_depth82 && child>b_factor82)break;
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
        int extra=Quesent(b,val)            ;
        // if(playertoplay&(1<<6))cout<<"\n\n\n\n\n\n\n"<<"white"<<" : "<<depth<<" "<<"\n\n\n\n\n\n";
        // else cout<<"\n\n\n\n\n\n\n"<<"black"<<" : "<<depth<<" "<<"\n\n\n\n\n\n";
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
        Move_Order.push_back({ ut, copyb }) ;

    }

    sort(Move_Order.begin(),Move_Order.end(),[&](const pair<int,Board*> &p,const pair<int,Board*> &q){

        if ( p.first > q.first ) return true;
        return false                        ;

    })                                      ;

    int val=INT_MIN                         ;
    int child=0;
    for(auto [move,copyb] : Move_Order){

        if(depth>=b_depth82 && child>b_factor82)break;
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
    for(auto move : moveset){

        Board *copyb=new Board(*b)          ;
        copyb->do_move_(move)               ;

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
    cout<<"Nodes processed : "<<nodes<<endl;
    b->do_move_(best_move)                  ;
    return best_move                        ;
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
