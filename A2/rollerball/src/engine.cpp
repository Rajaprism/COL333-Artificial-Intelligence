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

// #define o_rookval 100
// #define i_rookval 100

#define rookval 100
#define bishopval 70
#define pawnval 50
#define checkval 20
#define depthlevel 2
#define promote 3
#define dist 10

PlayerColor playertoplay;

int manhatten(U8 pos,int a,int b){
    int x=getx(pos),y=gety(pos);
    return abs(x-a)+abs(y-b);
}



int checkpawn(Board* b, U8 pos){
    U8 pawn=b->data.board_0[pos];
    if(pawn & PAWN) return pawnval;
    if(pawn & PAWN_BISHOP) return bishopval;
    if(pawn & PAWN_ROOK) return rookval;
    return 0;
}


int evaluate(Board* b){
    int val1=0;
    int val2=0;
    int val=0;
    if(b->data.w_king!=DEAD)val1+=kingval;
    if(b->data.w_bishop!=DEAD)val1+=bishopval;
    if(b->data.w_rook_bs!=DEAD)val1+=rookval;
    if(b->data.w_rook_ws!=DEAD)val1+=rookval;
    if(b->data.w_pawn_bs!=DEAD){
        U8 pos=b->data.w_pawn_bs;
        U8 pawn=b->data.board_0[pos];
        val1+=checkpawn(b,pos);
        int val=0;
        if(pawn&PAWN){
            int x=getx(pos),y=gety(pos);
            val+=min(manhatten(pos,4,5),manhatten(pos,4,6));
            if(x==2 && y==0)val+=2;
        }
        val1+=abs(dist-val)*promote; 
    }
    if(b->data.w_pawn_ws!=DEAD){
        U8 pos=b->data.w_pawn_ws;
        U8 pawn=b->data.board_0[pos];
        val1+=checkpawn(b,pos);
        int val=0;
        if(pawn&PAWN){
            int x=getx(pos),y=gety(pos);
            val+=min(manhatten(pos,4,5),manhatten(pos,4,6));
            if(x==2 && y==1)val+=2;
        }
        val1+=abs(dist-val)*promote; 
    }

    if(b->data.b_king!=DEAD)val2+=kingval;
    if(b->data.b_bishop!=DEAD)val2+=bishopval;
    if(b->data.b_rook_bs!=DEAD)val2+=rookval;
    if(b->data.b_rook_ws!=DEAD)val2+=rookval;
    if(b->data.b_pawn_bs!=DEAD){
        U8 pos=b->data.b_pawn_bs;
        U8 pawn=b->data.board_0[pos];
        val2+=checkpawn(b,pos);
        int val=0;
        if(pawn&PAWN){
            int x=getx(pos),y=gety(pos);
            val+=min(manhatten(pos,2,1),manhatten(pos,2,0));
            if(x==4 && y==6)val+=2;
        }
        val2+=abs(dist-val)*promote; 
    }
    if(b->data.b_pawn_ws!=DEAD){
        U8 pos=b->data.b_pawn_ws;
        U8 pawn=b->data.board_0[pos];
        val2+=checkpawn(b,pos);
        int val=0;
        if(pawn&PAWN){
            int x=getx(pos),y=gety(pos);
            val+=min(manhatten(pos,2,1),manhatten(pos,2,0));
            if(x==4 && y==5)val+=2;
        }
        val2+=abs(dist-val)*promote; 
    }

    if(b->data.player_to_play==playertoplay){
        if(b->in_check())val-=checkval;
    }else{
        if(b->in_check())val+=checkval;
    }
    if(playertoplay==WHITE)val+=(val1-val2);
    else val+=(val2-val1);
    return val;
}


int MinVal(Board* b,int alpha,int beta,int depth){

    auto moveset=b->get_legal_moves();
    if(depth==depthlevel){
        return evaluate(b);
    }
    int val=INT_MAX;
    for(auto move: moveset){
        auto copyb=b->copy();
        copyb->do_move(move);
        val=min(val,MaxVal(copyb,alpha,beta,depth+1));
        beta=min(beta,val);
        delete copyb;
        if(alpha>=beta)return val;
    }
    return val;
}

int MaxVal(Board* b,int alpha,int beta,int depth){
    auto moveset=b->get_legal_moves();
    if(depth==depthlevel){
        return evaluate(b);
    }
    int val=INT_MIN;
    for(auto move : moveset){
        auto copyb=b->copy();
        copyb->do_move(move);
        val=max(val,MinVal(copyb,alpha,beta,depth+1));
        alpha=max(alpha,val);
        delete copyb;
        if(alpha>=beta) return val;
    }
    return val;
}

U16 BestMove(Board* b){
    auto moveset =b->get_legal_moves();
    U16 best_move=*moveset.begin();
    int mx=INT_MIN;
    playertoplay=b->data.player_to_play;
    for(auto move : moveset){
        Board* c = b->copy();
        c->do_move(move);
        int val=MinVal(c,INT_MIN,INT_MAX,1);
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
        U16 best_move=BestMove(c);
        delete c;
        this->best_move=best_move;
    }
}

// #include<queue>
// #include <algorithm>
// #include <random>
// #include <iostream>

// #include "board.hpp"
// #include "engine.hpp"

// const int glob = 10000;
// const int king_val = 1000;
// const int rook_val = 100;
// const int bishop_val = 70;
// const int pawn_val = 50;
// const int check_val = 20;
// const int pawn_dis_val = 3;
// const int depth_lim = 3;

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

// constexpr U8 id[64] = {
//      0,  1,  2,  3,  4,  5,  6,  7,
//      8,  9, 10, 11, 12, 13, 14, 15,
//     16, 17, 18, 19, 20, 21, 22, 23,
//     24, 25, 26, 27, 28, 29, 30, 31,
//     32, 33, 34, 35, 36, 37, 38, 39,
//     40, 41, 42, 43, 44, 45, 46, 47,
//     48, 49, 50, 51, 52, 53, 54, 55,
//     56, 57, 58, 59, 60, 61, 62, 63
// };

// #define cw_90_pos(p) cw_90[p]
// #define cw_180_pos(p) cw_180[p]
// #define acw_90_pos(p) acw_90[p]
// #define cw_90_move(m) move_promo(cw_90[getp0(m)], cw_90[getp1(m)], getpromo(m))
// #define acw_90_move(m) move_promo(acw_90[getp0(m)], acw_90[getp1(m)], getpromo(m))
// #define cw_180_move(p) move_promo(cw_180[getp0(m)], cw_180[getp1(m)], getpromo(m))
// #define color(p) ((PlayerColor)(p & (WHITE | BLACK)))

// void undo_move(Board *b, U16 move) {

//     U8 p0 = getp0(move);
//     U8 p1 = getp1(move);
//     U8 promo = getpromo(move);

//     U8 piecetype = b->data.board_0[p1];
//     U8 deadpiece = b->data.last_killed_piece;
//     b->data.last_killed_piece = 0;

//     U8 pieces = (U8)(&(b->data));
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
//         piecetype = ((piecetype & (WHITE | BLACK)) ^ ROOK) | PAWN;
//     }
//     else if (promo == PAWN_BISHOP) {
//         piecetype = ((piecetype & (WHITE | BLACK)) ^ BISHOP) | PAWN;
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

// int manhattan(U8 pos1, U8 pos2){
//     int x1 = getx(pos1), y1 = gety(pos1);
//     int x2 = getx(pos2), y2 = gety(pos2);
//     return abs(x1 - x2) + abs(y1 - y2);
// }

// int pieces_contry(const BoardData& b, PlayerColor color){
//     int sum = 0;
//     if (color == WHITE){
//         if (b.w_king    != DEAD) sum += king_val;
//         if (b.w_rook_ws != DEAD) sum += rook_val;
//         if (b.w_rook_bs != DEAD) sum += rook_val;
//         if (b.w_bishop  != DEAD) sum += bishop_val;
//         if (b.w_pawn_ws != DEAD) sum += pawn_val;
//         if (b.w_pawn_bs != DEAD) sum += pawn_val;
//     }
//     else{
//         if (b.b_king    != DEAD) sum += king_val;
//         if (b.b_rook_ws != DEAD) sum += rook_val;
//         if (b.b_rook_bs != DEAD) sum += rook_val;
//         if (b.b_bishop  != DEAD) sum += bishop_val;
//         if (b.b_pawn_ws != DEAD) sum += pawn_val;
//         if (b.b_pawn_bs != DEAD) sum += pawn_val;
//     }
//     return sum;
// }

// int pawn_dist_contry(const BoardData& b, PlayerColor color){
//     int sum = 0;
//     if (color == WHITE){
//         if (b.w_pawn_ws != DEAD){
//             int pawn_d = 0;
//             if (b.w_pawn_ws == pos(2, 1)) pawn_d += 2;
//             pawn_d += std::min(manhattan(b.w_pawn_ws, pos(4, 5)), manhattan(b.w_pawn_ws, pos(4, 6)));
//             sum += (10 - pawn_d) * pawn_dis_val;
//         }
//         if (b.w_pawn_bs != DEAD){
//             int pawn_d = 0;
//             if (b.w_pawn_bs == pos(2, 0)) pawn_d += 2;
//             pawn_d += std::min(manhattan(b.w_pawn_bs, pos(4, 5)), manhattan(b.w_pawn_bs, pos(4, 6)));
//             sum += (10 - pawn_d) * pawn_dis_val;
//         }
//     }
//     else{
//         if (b.b_pawn_ws != DEAD){
//             int pawn_d = 0;
//             if (b.b_pawn_ws == pos(4, 5)) pawn_d += 2;
//             pawn_d += std::min(manhattan(b.b_pawn_ws, pos(2, 1)), manhattan(b.b_pawn_ws, pos(2, 0)));
//             sum += (10 - pawn_d) * pawn_dis_val;
//         }
//         if (b.b_pawn_bs != DEAD){
//             int pawn_d = 0;
//             if (b.b_pawn_bs == pos(4, 6)) pawn_d += 2;
//             pawn_d += std::min(manhattan(b.b_pawn_bs, pos(2, 1)), manhattan(b.b_pawn_bs, pos(2, 0)));
//             sum += (10 - pawn_d) * pawn_dis_val;
//         }
//     }
//     return sum;
// }

// int check_score(const Board& b, PlayerColor curr_player){
//     int score = 0;
//     bool check = b.in_check();
//     if (b.data.player_to_play == curr_player){
//         if (check) score -= check_val;
//     }
//     else{
//         if (check) score += check_val;
//     }
//     return score;
// }

// int checkmate(const Board& b, PlayerColor curr_player){
//     int score = 0;
//     std::unordered_set<U16> children = b.get_legal_moves();
//     bool check = b.in_check();
//     if (b.data.player_to_play == curr_player){
//         if (check && children.empty()) score -= glob;
//     }
//     else{
//         if (check && children.empty()) score += glob;
//     }
//     return score;
// }

// int evaluation(const Board& b, PlayerColor curr_player){
//     int eval1 = pieces_contry(b.data, curr_player);
//     int eval2 = pawn_dist_contry(b.data, curr_player);
//     PlayerColor opp_player = (curr_player == WHITE) ? BLACK : WHITE;
//     int eval3 = pieces_contry(b.data, opp_player);
//     int eval4 = pawn_dist_contry(b.data, opp_player);
//     int score = eval1 + eval2 - eval3 - (0.8 * eval4) + check_score(b, curr_player);
//     score += checkmate(b, curr_player);
//     score -= checkmate(b, opp_player);
//     return score;
// }

// int MaxVal(Board& b, int alpha, int beta, int depth);

// int MinVal(Board& b, int alpha, int beta, int depth){
//     depth++;
//     if (depth == depth_lim){
//         PlayerColor opp_player = (b.data.player_to_play == WHITE) ? BLACK : WHITE;
//         return evaluation(b, opp_player);
//     }
//     std::unordered_set<U16> children = b.get_legal_moves();

//     // preventing stalemate
//     // if (children.empty() && !b.in_check()) return -glob;

//     // checkmate
//     if (children.empty() && b.in_check()) return glob;

//     int mn = (int) 1e9 + 7;
//     for (auto s : children){
//         Board* cp = b.copy();
//         cp->do_move(s);
//         // b.do_move(s);
//         int child = MaxVal(*cp, alpha, beta, depth);
//         mn = std::min(mn, child);
//         beta = std::min(beta, child);
//         delete(cp);
//         // undo_move(&b, s);
//         if (alpha >= beta) return mn;
//     }
//     return mn;
// }

// int MaxVal(Board& b, int alpha, int beta, int depth){
//     depth++;
//     if (depth == depth_lim){
//         return evaluation(b, b.data.player_to_play);
//     }
//     std::unordered_set<U16> children = b.get_legal_moves();

//     // preventing stalemate
//     // if (children.empty() && !b.in_check()) return glob;

//     // checkmate
//     if (children.empty() && b.in_check()) return -glob;

//     int mx = (int) - 1e9 - 7;
//     for (auto s : children){
//         Board* cp = b.copy();
//         cp->do_move(s);
//         // b.do_move(s);
//         int child = MinVal(*cp, alpha, beta, depth);
//         mx = std::max(mx, child);
//         alpha = std::max(alpha, child);
//         // undo_move(&b, s);
//         delete(cp);
//         if (alpha >= beta) return mx;
//     }
//     return mx;
// }

// auto give_move(const Board& b, std::unordered_set<U16> legal_moves){
//     // using ps = std::pair<int, std::atomic<U16>>;
//     // std::priority_queue<ps, std::vector<ps>, std::greater<ps>> pq;
//     U16 best_mov = 0;
//     int depth = 1;
//     int inf = (int) 1e9 + 7;
//     int alpha = -inf, beta = inf, mx = -inf;
//     for (auto s : legal_moves){
//         Board* cp = b.copy();
//         cp->do_move(s);
//         // depth = 1;
//         int child = MinVal(*cp, alpha, beta, depth);
//         // std::cout << depth << "\n";
//         if (child > mx){
//             mx = child;
//             best_mov = s;
//         }
//         alpha = std::max(alpha, child);
//         delete(cp);
//     }
//     std::cout << "out\n";
//     return best_mov;
// }

// void Engine::find_best_move(const Board& b) {

//     // pick a random move
    
//     auto moveset = b.get_legal_moves();
//     if (moveset.size() == 0) {
//         this->best_move = 0;
//     }
//     else {
//         std::vector<U16> moves;
//         std::cout << all_boards_to_str(b) << std::endl;
//         for (auto m : moveset) {
//             std::cout << move_to_str(m) << " ";
//         }
//         std::cout << std::endl;
//         // std::sample(
//         //     moveset.begin(),
//         //     moveset.end(),
//         //     std::back_inserter(moves),
//         //     1,
//         //     std::mt19937{std::random_device{}()}
//         // );
//         // this->best_move = moves[0];
//         this->best_move = give_move(b, moveset);
//     }
// }