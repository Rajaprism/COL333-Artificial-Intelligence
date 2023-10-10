#include<queue>
#include <algorithm>
#include <random>
#include <iostream>
#include<bitset>

#include "board.hpp"
#include "engine.hpp"

int nodes = 0;

const int mod = (int) 1e8 + 7;

const int glob = 10000;
const int king_val = 1000;
const int rook_val = 200;
const int bishop_val = 100;
const int pawn_val = 60;
const int check_val = 100;
const int pawn_dis_val = 3;
const int depth_lim = 5;

// board hashing
const int p = 31;
const int q = 37;
const int play = 41;
const int d_ = 43;

std::bitset<mod> vis;

PlayerColor color;

long long binpow(long long a, int b){
    a %= mod;
    long long res = 1;
    while (b > 0){
        if (b & 1){
            res = (res * a) % mod;
        }
        a = (a * a) % mod;
        b >>= 1;
    }
    return res;
}

int calc(U8 ps, int pwr){
    int x = getx(ps), y = gety(ps);
    long long s = ((x * binpow(p, pwr)) + (y * binpow(q, pwr))) % mod;
    return (int) s;
}

int hash(BoardData& b, int depth){
    long long h = 0;
    // for white
    if (b.w_king    != DEAD) h = (h + calc(b.w_king   , 1)) % mod;
    if (b.w_rook_ws != DEAD) h = (h + calc(b.w_rook_ws, 2)) % mod;
    if (b.w_rook_bs != DEAD) h = (h + calc(b.w_rook_bs, 3)) % mod;
    if (b.w_bishop  != DEAD) h = (h + calc(b.w_bishop , 4)) % mod;
    if (b.w_pawn_ws != DEAD) h = (h + calc(b.w_pawn_ws, 5)) % mod;
    if (b.w_pawn_bs != DEAD) h = (h + calc(b.w_pawn_bs, 6)) % mod;

    // for black
    if (b.b_king    != DEAD) h = (h + calc(b.b_king   , 7 )) % mod;
    if (b.b_rook_ws != DEAD) h = (h + calc(b.b_rook_ws, 8 )) % mod;
    if (b.b_rook_bs != DEAD) h = (h + calc(b.b_rook_bs, 9 )) % mod;
    if (b.b_bishop  != DEAD) h = (h + calc(b.b_bishop , 10)) % mod;
    if (b.b_pawn_ws != DEAD) h = (h + calc(b.b_pawn_ws, 11)) % mod;
    if (b.b_pawn_bs != DEAD) h = (h + calc(b.b_pawn_bs, 12)) % mod;

    // for player to play
    h = (h + (b.player_to_play * binpow(play, 13)) % mod) % mod;
    h = (h + (depth * binpow(d_, 14)) % mod) % mod;
    return (int) h;
}

int manhattan(U8 pos1, U8 pos2){
    int x1 = getx(pos1), y1 = gety(pos1);
    int x2 = getx(pos2), y2 = gety(pos2);
    return abs(x1 - x2) + abs(y1 - y2);
}

int pieces_contry(const BoardData& b){
    int sum = 0;
    int mult1 = (color == WHITE) ? 1 : -1; 
    int mult2 = (color == WHITE) ? -1 : 1;
    {
        if (b.w_king    != DEAD) sum += king_val * mult1;
        if (b.w_rook_ws != DEAD) sum += rook_val * mult1;
        if (b.w_rook_bs != DEAD) sum += rook_val * mult1;
        if (b.w_bishop  != DEAD) sum += bishop_val * mult1;
        if (b.w_pawn_ws != DEAD) sum += pawn_val * mult1;
        if (b.w_pawn_bs != DEAD) sum += pawn_val * mult1;
    }
    {
        if (b.b_king    != DEAD) sum += king_val * mult2;
        if (b.b_rook_ws != DEAD) sum += rook_val * mult2;
        if (b.b_rook_bs != DEAD) sum += rook_val * mult2;
        if (b.b_bishop  != DEAD) sum += bishop_val * mult2;
        if (b.b_pawn_ws != DEAD) sum += pawn_val * mult2;
        if (b.b_pawn_bs != DEAD) sum += pawn_val * mult2;
    }
    return sum;
}

int pawn_dist_contry(const BoardData& b){
    int sum = 0;
    int mult1 = (color == WHITE) ? 1 : -1;
    int mult2 = (color == WHITE) ? -1 : 1;
    {
        if (b.w_pawn_ws != DEAD){
            int pawn_d = 0;
            if (b.w_pawn_ws == pos(2, 1)) pawn_d += 2;
            pawn_d += std::min(manhattan(b.w_pawn_ws, pos(4, 5)), manhattan(b.w_pawn_ws, pos(4, 6)));
            sum += (20 - pawn_d) * pawn_dis_val * mult1;
        }
        if (b.w_pawn_bs != DEAD){
            int pawn_d = 0;
            if (b.w_pawn_bs == pos(2, 0)) pawn_d += 2;
            pawn_d += std::min(manhattan(b.w_pawn_bs, pos(4, 5)), manhattan(b.w_pawn_bs, pos(4, 6)));
            sum += (20 - pawn_d) * pawn_dis_val * mult1;
        }
    }
    {
        if (b.b_pawn_ws != DEAD){
            int pawn_d = 0;
            if (b.b_pawn_ws == pos(4, 5)) pawn_d += 2;
            pawn_d += std::min(manhattan(b.b_pawn_ws, pos(2, 1)), manhattan(b.b_pawn_ws, pos(2, 0)));
            sum += (20 - pawn_d) * pawn_dis_val * mult2;
        }
        if (b.b_pawn_bs != DEAD){
            int pawn_d = 0;
            if (b.b_pawn_bs == pos(4, 6)) pawn_d += 2;
            pawn_d += std::min(manhattan(b.b_pawn_bs, pos(2, 1)), manhattan(b.b_pawn_bs, pos(2, 0)));
            sum += (20 - pawn_d) * pawn_dis_val * mult2;
        }
    }
    return sum;
}

int check_score(const Board& b){
    int score = 0;
    bool check = b.in_check();
    if (b.data.player_to_play == color){
        if (check) score -= check_val;
    }
    else{
        if (check) score += check_val;
    }
    return score;
}

int checkmate(const Board& b, std::unordered_set<U16>& children){
    int score = 0;
    bool check = b.in_check();
    if (b.data.player_to_play == color){
        if (check && children.empty()) score -= glob;
    }
    else{
        if (check && children.empty()) score += glob;
    }
    return score;
}

int evaluation(const Board& b){
    int eval1 = pieces_contry(b.data);
    // int eval2 = pawn_dist_contry(b.data);
    // PlayerColor opp_player = (curr_player == WHITE) ? BLACK : WHITE;

    // std::cout << (opp_player == WHITE ? "WHITE" : "BLACK") << "\n";
    
    // int eval3 = pieces_contry(b.data);
    int eval3 = pawn_dist_contry(b.data);
    // int score = eval1 + eval2 - eval3 - (0.8 * eval4) + check_score(b, curr_player);
    int score = eval1 + eval3;
    // score += checkmate(b, children) + check_score(b);
    // score += checkmate(b, curr_player);
    // score -= checkmate(b, opp_player);
    return score;
}

int MaxVal(Board& b, int alpha, int beta, int depth);

int MinVal(Board& b, int alpha, int beta, int depth){
    nodes++;
    depth++;
    if (depth == depth_lim){
        // PlayerColor opp_player = (b.data.player_to_play == WHITE) ? BLACK : WHITE;
        return evaluation(b);
    }
    std::unordered_set<U16> children = b.get_legal_moves();

    // preventing stalemate
    // if (children.empty() && !b.in_check()) return -glob;

    // checkmate
    // if (children.empty() && b.in_check()) return glob;

    int mn = (int) 1e9 + 7;
    for (auto s : children){
        Board* cp = b.copy();
        cp->do_move(s);

        // int h = hash(cp->data, depth);
        // if (vis[h] == 1){
        //     delete(cp);
        //     continue;
        // }
        // vis[h] = 1;

        int child = MaxVal(*cp, alpha, beta, depth);
        mn = std::min(mn, child);
        beta = std::min(beta, child);
        delete(cp);
        if (alpha >= beta) return mn;
    }
    return mn;
}

int MaxVal(Board& b, int alpha, int beta, int depth){
    nodes++;
    depth++;
    if (depth == depth_lim){
        return evaluation(b);
    }

    std::unordered_set<U16> children = b.get_legal_moves();
    // preventing stalemate
    // if (children.empty() && !b.in_check()) return glob;

    // checkmate
    // if (children.empty() && b.in_check()) return -glob;

    int mx = (int) - 1e9 - 7;
    for (auto s : children){
        Board* cp = b.copy();
        cp->do_move(s);

        // int h = hash(cp->data, depth);
        // if (vis[h] == 1){
        //     delete(cp);
        //     continue;
        // }
        // vis[h] = 1;

        int child = MinVal(*cp, alpha, beta, depth);
        mx = std::max(mx, child);
        alpha = std::max(alpha, child);
        delete(cp);
        if (alpha >= beta) return mx;
    }
    return mx;
}

auto give_move(const Board& b, std::unordered_set<U16>& legal_moves){
    nodes++;
    U16 best_mov = 0;
    int depth = 1;
    int inf = (int) 1e9 + 7;
    int alpha = -inf, beta = inf, mx = -inf;
    for (auto s : legal_moves){
        Board* cp = b.copy();
        cp->do_move(s);

        // int h = hash(cp->data, 1);
        // if (vis[h] == 1){
        //     delete(cp);
        //     continue;
        // }
        // vis[h] = 1;

        int child = MinVal(*cp, alpha, beta, depth);
        if (child > mx){
            mx = child;
            best_mov = s;
        }
        alpha = std::max(alpha, child);
        delete(cp);
    }
    std::cout << best_mov << "\n";
    return best_mov;
}

// struct item{
//     Board* b;
//     int d;
// };

// auto bfs(const Board& b, std::unordered_set<U16>& legal_moves){
//     std::queue<std::pair<int, Board>> q;
//     q.push({depth_lim, *b.copy()});
//     while(!q.empty()){
//         auto [dt, bd] = q.front();
//         q.pop();
//         std::unordered_set<U16> children = bd.get_legal_moves();
//         for (auto s : children){
//             Board k = *bd.copy();
//             k.do_move(s);

//             int h = hash(k.data);
//             if (vis[h] == 1){
//                 continue;
//             }
//             vis[h] = 1;

//         }
//     }
// }

void Engine::find_best_move(const Board& b) {

    color = b.data.player_to_play;

    // pick a random move
    
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        this->best_move = 0;
    }
    else {
        std::vector<U16> moves;
        std::cout << all_boards_to_str(b) << std::endl;
        for (auto m : moveset) {
            std::cout << move_to_str(m) << " ";
        }
        std::cout << std::endl;
        // std::sample(
        //     moveset.begin(),
        //     moveset.end(),
        //     std::back_inserter(moves),
        //     1,
        //     std::mt19937{std::random_device{}()}
        // );
        // this->best_move = moves[0];
        this->best_move = give_move(b, moveset);
        std::cout << nodes << "\n";
        nodes = 0;
        // vis >>= mod + 1;
    }
}