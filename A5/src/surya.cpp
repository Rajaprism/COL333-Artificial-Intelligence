#include <algorithm>
#include <random>
#include <iostream>
#include <thread>

using namespace std;

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"

PlayerColor curr_player, opp_player;
const int depth_lim = 6;

const int king = 10000;
const int rook = 2000;
const int bishop = 1000;
const int knight = 1000;
const int pawn = 500;

vector<int> white_73_reward = {
    46, 48, 51, 0, 0, 0, 0, 0,
    42, 42, 44, 0, 0, 0, 0, 0,
    36, 36, 0, 0, 0, 0, 0, 0,
    30, 30, 0, 0, 0, 0, 0, 0,
    21, 21, 0, 0, 0, 0, 0, 0,
    24, 18, 12, 6, 0, 0, 0, 0,
    24, 18, 12, 6, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};
vector<int> black_73_reward = {
    0, 0, 0, 6, 12, 18, 24, 0,
    0, 0, 0, 6, 12, 18, 24, 0,
    0, 0, 0, 0, 0, 21, 27, 0,
    0, 0, 0, 0, 0, 30, 30, 0,
    0, 0, 0, 0, 0, 36, 36, 0,
    0, 0, 0, 0, 44, 42, 42, 0,
    0, 0, 0, 0, 51, 48, 46, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

int promotion(BoardData& b, U8 pos){
    if (b.board_0[pos] & ROOK) return 1;
    if (b.board_0[pos] & BISHOP) return -1;
    return 0;
}

int white_pawn_dist(BoardData& b){
    int result = 0;
    if (b.board_type == SEVEN_THREE){
        if (b.w_pawn_1 != DEAD && promotion(b, b.w_pawn_1) == 0) result += 60 - white_73_reward[(int) b.w_pawn_1];
        if (b.w_pawn_2 != DEAD && promotion(b, b.w_pawn_2) == 0) result += 60 - white_73_reward[(int) b.w_pawn_2];
        if (b.w_pawn_3 != DEAD && promotion(b, b.w_pawn_3) == 0) result += 60 - white_73_reward[(int) b.w_pawn_3];
        if (b.w_pawn_4 != DEAD && promotion(b, b.w_pawn_4) == 0) result += 60 - white_73_reward[(int) b.w_pawn_4];
    }
    else if (b.board_type == EIGHT_FOUR){

    }
    else{

    }
    return result;
}

int black_pawn_dist(BoardData& b){
    int result = 0;
    if (b.board_type == SEVEN_THREE){
        if (b.b_pawn_1 != DEAD && promotion(b, b.b_pawn_1) == 0) result += 60 - black_73_reward[(int) b.b_pawn_1];
        if (b.b_pawn_2 != DEAD && promotion(b, b.b_pawn_2) == 0) result += 60 - black_73_reward[(int) b.b_pawn_2];
        if (b.b_pawn_3 != DEAD && promotion(b, b.b_pawn_3) == 0) result += 60 - black_73_reward[(int) b.b_pawn_3];
        if (b.b_pawn_4 != DEAD && promotion(b, b.b_pawn_4) == 0) result += 60 - black_73_reward[(int) b.b_pawn_4];
    }
    else if (b.board_type == EIGHT_FOUR){

    }
    else{

    }
    return result;
}

int pawn_val(BoardData& b, U8 pos){
    if (b.board_0[pos] & ROOK) return rook;
    if (b.board_0[pos] & BISHOP) return bishop;
    return pawn;
}

int black_contry(BoardData& b){
    int result = 0;
    if (b.b_rook_1   != DEAD) result += rook;
    if (b.b_rook_2   != DEAD) result += rook;
    if (b.b_king     != DEAD) result += king;
    if (b.b_bishop   != DEAD) result += bishop;
    if (b.b_knight_1 != DEAD) result += knight;
    if (b.b_knight_2 != DEAD) result += knight;
    if (b.b_pawn_1   != DEAD) result += pawn_val(b, b.b_pawn_1);
    if (b.b_pawn_2   != DEAD) result += pawn_val(b, b.b_pawn_2);
    if (b.b_pawn_3   != DEAD) result += pawn_val(b, b.b_pawn_3);
    if (b.b_pawn_4   != DEAD) result += pawn_val(b, b.b_pawn_4);
    return result;
}

int white_contry(BoardData& b){
    int result = 0;
    if (b.w_rook_1   != DEAD) result += rook;
    if (b.w_rook_2   != DEAD) result += rook;
    if (b.w_king     != DEAD) result += king;
    if (b.w_bishop   != DEAD) result += bishop;
    if (b.w_knight_1 != DEAD) result += knight;
    if (b.w_knight_2 != DEAD) result += knight;
    if (b.w_pawn_1   != DEAD) result += pawn_val(b, b.w_pawn_1);
    if (b.w_pawn_2   != DEAD) result += pawn_val(b, b.w_pawn_2);
    if (b.w_pawn_3   != DEAD) result += pawn_val(b, b.w_pawn_3);
    if (b.w_pawn_4   != DEAD) result += pawn_val(b, b.w_pawn_4);
    return result;
}

int evaluation(Board& b){
    int result = 0;
    if (curr_player == WHITE){
        result = white_contry(b.data) - (int) (0.99 * black_contry(b.data));
        result += white_pawn_dist(b.data) - (int) (0.9 * black_pawn_dist(b.data));
    }
    else{
        result = black_contry(b.data) - (int) (0.99 * white_contry(b.data));
        result += black_pawn_dist(b.data) - (int) (0.9 * white_pawn_dist(b.data));
    }
    return result;
}

int MaxVal(Board& b, int alpha, int beta, int depth);

int MinVal(Board& b, int alpha, int beta, int depth){
    depth++;
    if (depth >= depth_lim){
        return evaluation(b);
    }
    unordered_set<U16> children = b.get_legal_moves();

    vector<pair<Board, int>> child;
    for (auto s : children){
        Board cp = Board(b);
        cp.do_move_(s);
        int e = evaluation(cp);
        child.push_back({cp, e});
    }
    sort(child.begin(), child.end(), [&] (pair<Board, int> &a, pair<Board, int> &b){
        return a.second < b.second;
    });

    int mn = (int) 1e9 + 7;
    for (auto [cp, val] : child){
        int child = MaxVal(cp, alpha, beta, depth);
        mn = min(mn, child);
        beta = min(beta, child);
        if (alpha >= beta) return mn;
    }
    return mn;
}

int MaxVal(Board& b, int alpha, int beta, int depth){
    depth++;
    if (depth >= depth_lim){
        return evaluation(b);
    }
    unordered_set<U16> children = b.get_legal_moves();

    vector<pair<Board, int>> child;
    for (auto s : children){
        Board cp = Board(b);
        cp.do_move_(s);
        int e = evaluation(cp);
        child.push_back({cp, e});
    }
    sort(child.begin(), child.end(), [&] (pair<Board, int> &a, pair<Board, int> &b){
        return a.second > b.second;
    });

    int mx = (int) - 1e9 - 7;
    for (auto [cp, val] : child){
        int child = MinVal(cp, alpha, beta, depth);
        mx = max(mx, child);
        alpha = max(alpha, child);
        if (alpha >= beta) return mx;
    }
    return mx;
}

auto give_move(const Board& b, unordered_set<U16>& legal_moves){
    U16 best_mov = 0;
    int depth = 0;
    int inf = (int) 1e9 + 7;
    int alpha = -inf, beta = inf, mx = -inf;

    vector<pair<Board, pair<int, U16>>> child;
    for (auto s : legal_moves){
        Board cp = Board(b);
        cp.do_move_(s);
        int e = evaluation(cp);
        child.push_back({cp, {e, s}});
    }
    sort(child.begin(), child.end(), [&] (pair<Board, pair<int, U16>> &a, pair<Board, pair<int, U16>> &b){
        return a.second.first > b.second.first;
    });

    for (auto [cp, val] : child){
        int child = MinVal(cp, alpha, beta, depth);
        if (child > mx){
            mx = child;
            best_mov = val.second;
        }
        alpha = max(alpha, child);
    }
    cout << best_mov << "\n";
    return best_mov;
}

void Engine::find_best_move(const Board& b) {
    curr_player = b.data.player_to_play;
    opp_player = (curr_player == WHITE) ? BLACK : WHITE;
    
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        cout << "Could not get any moves from board!\n";
        cout << board_to_str(&b.data);
        this->best_move = 0;
    }
    else {
        // vector<U16> moves;
        // cout << show_moves(&b.data, moveset) << endl;
        // for (auto m : moveset) {
        //     cout << move_to_str(m) << " ";
        // }
        // cout << endl;
        // sample(
        //     moveset.begin(),
        //     moveset.end(),
        //     back_inserter(moves),
        //     1,
        //     mt19937{random_device{}()}
        // );
        this->best_move = give_move(b, moveset);
    }

    // just for debugging, to slow down the moves
    // this_thread::sleep_for(chrono::milliseconds(1000));
}