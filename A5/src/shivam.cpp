#include <algorithm>
#include <random>
#include <iostream>
#include <thread>

using namespace std;

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"

PlayerColor curr_player, opp_player;
int depth_lim = 4;

const int king = 10000;
const int rook = 2000;
const int bishop = 1000;
const int knight = 1500;
const int pawn = 500;
const int check = 500;
const int bonus = 500;

const double lambda1 = 1.00;
const double lambda2 = 1.00;
const int lambda3 = 20;
const int mult = 2;

vector<int> white_73_reward = {
    46, 48, 51, 0, 0, 0, 0, 0,
    42, 42, 44, 0, 0, 0, 0, 0,
    36, 36, 0, 0, 0, 0, 0, 0,
    30, 30, 0, 0, 0, 0, 0, 0,
    27, 21, 0, 0, 0, 0, 0, 0,
    24, 18, 12, 6, 0, 0, 0, 0,
    24, 18, 12, 6, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};
vector<int> black_73_reward = {
    0, 0, 0, 6, 12, 18, 24, 0,
    0, 0, 0, 6, 12, 18, 24, 0,
    0, 0, 0, 0, 0,  21, 27, 0,
    0, 0, 0, 0, 0,  30, 30, 0,
    0, 0, 0, 0, 0,  36, 36, 0,
    0, 0, 0, 0, 44, 42, 42, 0,
    0, 0, 0, 0, 51, 48, 46, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};
vector<int> white_84_reward={
    66, 69, 72, 77, 80, 84, 0, 0,
    60, 60, 63, 75, 79, 81, 0, 0,
    54, 54, 0,  0,  0, 0, 0, 0,
    48, 48, 0,  0,  0, 0, 0, 0,
    42, 42, 0,  0,  0, 0, 0, 0,
    36, 27, 0,  0,  0, 0, 0, 0,
    33, 24, 18, 12, 6, 0, 0, 0,
    30, 24, 18, 12, 6, 0, 0, 0
};
vector<int> black_84_reward={
    0, 0, 0, 6, 12, 18, 24, 30,
    0, 0, 0, 6, 12, 18, 24, 33,
    0, 0, 0, 0, 0,  0,  27, 36,
    0, 0, 0, 0, 0,  0,  42, 42,
    0, 0, 0, 0, 0,  0,  48, 48,
    0, 0, 0, 0, 0,  0,  54, 54,
    0, 0, 81, 79, 75,  63, 60, 60,
    0, 0, 84, 80, 77,  72, 69, 66
};
vector<int> white_82_reward={
    51, 52, 55, 60, 0,  0,  0, 0,
    48, 46, 47, 56, 63, 0,  0, 0,
    45, 42, 43, 45, 65, 70, 0, 0,
    41, 36, 39, 0,  0,  0,  0, 0,
    40, 35, 21, 0,  0,  0,  0, 0,
    39, 31, 20, 12, 0,  0,  0, 0,
    36, 24, 15, 6,  0,  0,  0, 0,
    33, 30, 20, 12, 0,  0,  0, 0
};
vector<int> black_82_reward={
    0, 0, 0,  0,  12, 20, 30, 33,
    0, 0, 0,  0,  6,  15, 24, 36,
    0, 0, 0,  0,  12, 20, 31, 39,
    0, 0, 0,  0,  0,  21, 35, 40,
    0, 0, 0,  0,  0,  39, 36, 41,
    0, 0, 70, 65, 45, 43, 42, 45,
    0, 0, 0,  63, 56, 47, 46, 48,
    0, 0, 0,  0,  60, 55, 52, 51
};

int pawn_(BoardData& b, U8 pos){
    if (b.board_0[pos] & ROOK) return 10;
    if (b.board_0[pos] & BISHOP) return 5;
    return 2;
}

void depth_check(BoardData b){
    int result = 0;

    if (b.b_rook_1   != DEAD) result += 10;
    if (b.b_rook_2   != DEAD) result += 10;
    if (b.b_king     != DEAD) result += 5;
    if (b.b_bishop   != DEAD) result += 5;
    if (b.b_knight_1 != DEAD) result += 3;
    if (b.b_knight_2 != DEAD) result += 3;
    if (b.b_pawn_1   != DEAD) result += pawn_(b, b.b_pawn_1);
    if (b.b_pawn_2   != DEAD) result += pawn_(b, b.b_pawn_2);
    if (b.b_pawn_3   != DEAD) result += pawn_(b, b.b_pawn_3);
    if (b.b_pawn_4   != DEAD) result += pawn_(b, b.b_pawn_4);

    if (b.w_rook_1   != DEAD) result += 10;
    if (b.w_rook_2   != DEAD) result += 10;
    if (b.w_king     != DEAD) result += 5;
    if (b.w_bishop   != DEAD) result += 5;
    if (b.w_knight_1 != DEAD) result += 3;
    if (b.w_knight_2 != DEAD) result += 3;
    if (b.w_pawn_1   != DEAD) result += pawn_(b, b.w_pawn_1);
    if (b.w_pawn_2   != DEAD) result += pawn_(b, b.w_pawn_2);
    if (b.w_pawn_3   != DEAD) result += pawn_(b, b.w_pawn_3);
    if (b.w_pawn_4   != DEAD) result += pawn_(b, b.w_pawn_4);

    if(b.board_type==SEVEN_THREE){
        if(result>=40){
            depth_lim=4;
        }
        else if(result>=21){
            depth_lim=6;
        }
        else if(result>=11){
            depth_lim=4;
        }
        else{
            depth_lim=1;
        }
    }
    else if(b.board_type==EIGHT_FOUR){
        if(result>=35){
            depth_lim=4;
        }
        else if(result>=21){
            depth_lim=6;
        }
        else if(result>=11){
            depth_lim=6;
        }
        else{
            depth_lim=1;
        }
    }
    else{
        if(result>=40){
            depth_lim=4;
        }
        if(result>=21){
            depth_lim=4;
        }
        else if(result>=11){
            depth_lim=4;
        }
        else{
            depth_lim=1;
        }
    }
}

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
        if (b.w_pawn_1 != DEAD && promotion(b, b.w_pawn_1) == 0) result += 90 - white_84_reward[(int) b.w_pawn_1];
        if (b.w_pawn_2 != DEAD && promotion(b, b.w_pawn_2) == 0) result += 90 - white_84_reward[(int) b.w_pawn_2];
        if (b.w_pawn_3 != DEAD && promotion(b, b.w_pawn_3) == 0) result += 90 - white_84_reward[(int) b.w_pawn_3];
        if (b.w_pawn_4 != DEAD && promotion(b, b.w_pawn_4) == 0) result += 90 - white_84_reward[(int) b.w_pawn_4];
    }
    else{
        if (b.w_pawn_1 != DEAD && promotion(b, b.w_pawn_1) == 0) result += 80 - white_82_reward[(int) b.w_pawn_1];
        if (b.w_pawn_2 != DEAD && promotion(b, b.w_pawn_2) == 0) result += 80 - white_82_reward[(int) b.w_pawn_2];
        if (b.w_pawn_3 != DEAD && promotion(b, b.w_pawn_3) == 0) result += 80 - white_82_reward[(int) b.w_pawn_3];
        if (b.w_pawn_4 != DEAD && promotion(b, b.w_pawn_4) == 0) result += 80 - white_82_reward[(int) b.w_pawn_4];
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
        if (b.b_pawn_1 != DEAD && promotion(b, b.b_pawn_1) == 0) result += 90 - black_84_reward[(int) b.b_pawn_1];
        if (b.b_pawn_2 != DEAD && promotion(b, b.b_pawn_2) == 0) result += 90 - black_84_reward[(int) b.b_pawn_2];
        if (b.b_pawn_3 != DEAD && promotion(b, b.b_pawn_3) == 0) result += 90 - black_84_reward[(int) b.b_pawn_3];
        if (b.b_pawn_4 != DEAD && promotion(b, b.b_pawn_4) == 0) result += 90 - black_84_reward[(int) b.b_pawn_4];
    }
    else{
        if (b.b_pawn_1 != DEAD && promotion(b, b.b_pawn_1) == 0) result += 80 - black_82_reward[(int) b.b_pawn_1];
        if (b.b_pawn_2 != DEAD && promotion(b, b.b_pawn_2) == 0) result += 80 - black_82_reward[(int) b.b_pawn_2];
        if (b.b_pawn_3 != DEAD && promotion(b, b.b_pawn_3) == 0) result += 80 - black_82_reward[(int) b.b_pawn_3];
        if (b.b_pawn_4 != DEAD && promotion(b, b.b_pawn_4) == 0) result += 80 - black_82_reward[(int) b.b_pawn_4];
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

int eval(Board &b){
    int result = 0;

    //if (b.in_check()) result -= 200;

    if (curr_player == WHITE){
        result = white_contry(b.data) - (int) (lambda1 * black_contry(b.data));
        result += (white_pawn_dist(b.data) - (int) (lambda2 * black_pawn_dist(b.data))) * mult;
    }
    else{
        result = black_contry(b.data) - (int) (lambda1 * white_contry(b.data));
        result += (black_pawn_dist(b.data) - (int) (lambda2 * white_pawn_dist(b.data))) * mult;
    }
    return result;
}

int MaxVal(Board &b, int alpha, int beta, int depth);

int MinVal(Board &b, int alpha, int beta, int depth){
    depth++;
    if (depth >= depth_lim){
        return eval(b);
    }
    unordered_set<U16> children = b.get_legal_moves();

    int inf = (int) 1e9 + 7;
    int mn = (int) 1e9 + 7;

    bool ok=b.in_check();
    int additional=0;
    if (children.empty()){
        if (ok) return inf;
        //else return -inf;
    }
    else{
        if(ok) additional=bonus;
    }

    for (auto s : children){
        Board cp = Board(b);
        cp.do_move_(s);
        int child = MaxVal(cp, alpha, beta, depth);
        mn = min(mn, child);
        beta = min(beta, child);
        if (alpha >= beta) return mn;
    }
    return mn+(depth==depth_lim-1 ? additional : 0);
}

int MaxVal(Board &b, int alpha, int beta, int depth){
    depth++;
    if (depth >= depth_lim){
        return eval(b);
    }
    unordered_set<U16> children = b.get_legal_moves();

    int inf = (int) 1e9 + 7;
    int mx = (int) - 1e9 - 7;

    if (children.empty()){
        if (b.in_check()) return -inf;
        //else return inf;
    }

    for (auto s : children){
        Board cp = Board(b);
        cp.do_move_(s);
        int child = MinVal(cp, alpha, beta, depth);
        mx = max(mx, child);
        alpha = max(alpha, child);
        if (alpha >= beta) return mx;
    }
    return mx;
}


auto give_move(const Board &b, unordered_set<U16>& legal_moves){
    U16 best_mov = 0;
    int depth = 0;
    int inf = (int) 1e9 + 8;
    int alpha = -2*inf, beta = 2*inf, mx = -inf;

    for (auto s : legal_moves){
        Board cp = Board(b);
        cp.do_move_(s);
        int child = MinVal(cp, alpha, beta, depth);
        if (child > mx){
            mx = child;
            best_mov = s;
        }
        alpha = max(alpha, child);
    }
    cout << best_mov << "\n";
    return best_mov;
}

void Engine::find_best_move(const Board& b) {
    depth_check(b.data);
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
        cout << "best_4785386_" << this->best_move << "\n";
    }

    // just for debugging, to slow down the moves
    // this_thread::sleep_for(chrono::milliseconds(1000));
}