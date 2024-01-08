#include<iostream>
#include<limits>
#include <chrono>
#include <ctime>
#include <random>
#include <algorithm>
#include "engine.hpp"
#include <unordered_map>
#include "board.hpp"
#include "butils.hpp"

using namespace std;
class TimeException : public exception{
    public:
        const char * what() const throw(){
            return "Threshold Time exceeded\n";
        }
};
bool current_player = false;
vector<vector<int>> create_value_matrix(const Board& b);
class RollerBoard{
    public:
        Board b;
        vector<U16> moves;
        vector<RollerBoard*> children;
        int eval_val =0;
        vector<vector<int>> value_matrix;
        unordered_set<U16> future_moves;
        RollerBoard(const Board& b);
        void create_states();
};

RollerBoard::RollerBoard(const Board& b){
    this->b = b;
    // cout<<"before valmatrix"<<endl;
    this->value_matrix = create_value_matrix(b);
    // cout<<"before get legal"<<endl;
    unordered_set<U16> st =b.get_legal_moves();
    // cout<<"size is "<<st.size()<<endl;
    this->future_moves = st;
    // cout<<"glegal done"<<endl;
}

void RollerBoard::create_states(){

    // cout<<"in create states"<<endl;
    unordered_set<U16> future_moves = this->future_moves;
    // cout<<"size is "<<endl;
    for(auto i:future_moves){
        // cout<<"in the loop"<<endl;
        Board* temp = new Board(b.data);
        // cout<<"clling do move"<<endl;
        temp->do_move_(i);
        // cout<<"move done"<<endl;
        RollerBoard* rtemp = new RollerBoard(*temp);
        // cout<<"rtemp created"<<endl;
        rtemp->moves.push_back(i);
        this->children.push_back(rtemp);
        // cout<<"before free"<<endl;
        free(temp);
    }
    
}


int get_y(U8 p){ return ((p)>>3);}
int get_x(U8 p){ return ((p)&0x7);}
U8 get_p1(U16 m){ return ((m)&0x3f);}
U8 get_p0(U16 m){ return (((m)>>8)&0x3f);}
U8 get_pos(int x,int y){ return (((y)<<3)|(x));}

vector<vector<int>> create_value_matrix(const Board& p){
    vector<vector<int>> val(32, vector<int>(32,0));
    // cout<<"before val"<<endl;
    U8 b_rook_1 = p.data.b_rook_1;
    U8 b_rook_2 = p.data.b_rook_2;
    U8 b_pawn_1 = p.data.b_pawn_1;
    U8 b_pawn_2 = p.data.b_pawn_2;
    U8 b_bishop = p.data.b_bishop;
    U8 w_rook_1 = p.data.w_rook_1;
    U8 w_rook_2 = p.data.w_rook_2;
    U8 w_pawn_1 = p.data.w_pawn_1;
    U8 w_pawn_2 = p.data.w_pawn_2;
    U8 w_bishop = p.data.w_bishop;
    // cout<<"midway"<<endl;
    val[get_x(b_rook_1)][get_y(b_rook_1)] = -5;
    val[get_x(b_rook_2)][get_y(b_rook_2)] = -5;
    // cout<<"rooks creayed"<<endl;
    val[get_x(b_pawn_1)][get_y(b_pawn_1)] = -1;
    val[get_x(b_pawn_2)][get_y(b_pawn_2)] = -1;
    // cout<<"pawns done"<<endl;
    // cout<<"coordinates"<<get_x(b_bishop)<<" "<<get_y(b_bishop)<<" "<<static_cast<int>(b_bishop)<<endl;
    val[get_x(b_bishop)][get_y(b_bishop)] = -4;
    // cout<<"bishops done"<<endl;
    val[get_x(w_rook_1)][get_y(w_rook_1)] = 5;
    val[get_x(w_rook_2)][get_y(w_rook_2)] = 5;
    // cout<<"wrooks creayed"<<endl;
    val[get_x(w_pawn_1)][get_y(w_pawn_1)] = 1;
    val[get_x(w_pawn_2)][get_y(w_pawn_2)] = 1;
    // cout<<"wpawns done"<<endl;
    val[get_x(w_bishop)][get_y(w_bishop)] = 4;
    // cout<<"mtrix created"<<endl;
    return val;
}

// --------------------------------------------------------TRANSPOSITION TABLE--------------------------------------------------------------------
unsigned long long int random_table[8][8][6];
mt19937 mt(01234567);

unsigned long long int randomInt(){
    uniform_int_distribution<unsigned long long int> dist(0,UINT64_MAX);
    return dist(mt);
}

void initTable(){
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            for(int k=0;k<6;k++){
                random_table[i][j][k] = randomInt();
            }
        }
    }
}

int get_index(int piece_value){
    if(piece_value == -1) return 0;
    if(piece_value == 1) return 1;
    if(piece_value == -4) return 2;
    if(piece_value == 4) return 3;
    if(piece_value == -5) return 4;
    if(piece_value == 5) return 5;
    else return -1;
}

unsigned long long int get_hash(vector<vector<int>> val){
    unsigned long long int h = 0;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(val[i][j]!=0){
                int piece = get_index(val[i][j]);
                h ^= random_table[i][j][piece];
            }
        }
    }
    return h;
}
unordered_map<unsigned long long int, double> transposition_table;

// ---------------------------------------------------------------EVALUATION FUNCTION-----------------------------------------------------------------

vector<int> get_my_pieces(RollerBoard* rb){
    vector<int> my_pieces;
    int pawn_val = 0;
    int bishop_val = 0;
    int rook_val = 0;
    // int king_val = 0;
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            if(rb->value_matrix[i][j]==1) pawn_val+=1;
            else if(rb->value_matrix[i][j]==5) rook_val+=1;
            else if(rb->value_matrix[i][j]==4) bishop_val+=1;
        }
    }
    my_pieces = {1,bishop_val,pawn_val,rook_val};
    return my_pieces;
}

vector<int> get_opp_pieces(RollerBoard* rb){
    vector<int> opp_pieces;
    int pawn_val = 0;
    int bishop_val = 0;
    int rook_val = 0;
    // int king_val = 0;
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            if(rb->value_matrix[i][j]==-1) pawn_val+=1;
            else if(rb->value_matrix[i][j]==-5) rook_val+=1;
            else if(rb->value_matrix[i][j]==-4) bishop_val+=1;
        }
    }
    opp_pieces = {1,bishop_val,pawn_val,rook_val};
    return opp_pieces;
}

double compute_piece_value(RollerBoard* rb){
    double white = 0;
    double black = 0;
    if(static_cast<int>(rb->b.data.b_king) != static_cast<int>(DEAD) ) black += 100000;
    if(static_cast<int>(rb->b.data.w_king) != static_cast<int>(DEAD) ) white += 100000;

    if((rb->b.data.board_type==EIGHT_FOUR || rb->b.data.board_type==EIGHT_TWO) && (static_cast<int>(rb->b.data.b_rook_1) != static_cast<int>(DEAD)) ) {black += 9;}
    else if(static_cast<int>(rb->b.data.b_rook_1) != static_cast<int>(DEAD)){black += 5;}
    if((rb->b.data.board_type==EIGHT_FOUR || rb->b.data.board_type==EIGHT_TWO) && (static_cast<int>(rb->b.data.b_rook_2) != static_cast<int>(DEAD)) ) {black += 9;}
    else if(static_cast<int>(rb->b.data.b_rook_2) != static_cast<int>(DEAD)){black += 5;}
    if(static_cast<int>(rb->b.data.b_pawn_1) != static_cast<int>(DEAD) ) black += 1;
    if(static_cast<int>(rb->b.data.b_pawn_2) != static_cast<int>(DEAD) ) black += 1;
    if((rb->b.data.board_type==EIGHT_FOUR || rb->b.data.board_type==EIGHT_TWO) && static_cast<int>(rb->b.data.b_pawn_3) != static_cast<int>(DEAD) ) black += 1;
    if((rb->b.data.board_type==EIGHT_FOUR || rb->b.data.board_type==EIGHT_TWO) && static_cast<int>(rb->b.data.b_pawn_4) != static_cast<int>(DEAD) ) black += 1;
    if(rb->b.data.board_type==EIGHT_TWO && static_cast<int>(rb->b.data.b_knight_1) != static_cast<int>(DEAD) ) black += 6;
    if(rb->b.data.board_type==EIGHT_TWO && static_cast<int>(rb->b.data.b_knight_2) != static_cast<int>(DEAD) ) black += 6;
    if(static_cast<int>(rb->b.data.b_bishop) != static_cast<int>(DEAD) ) black += 3;

    if((rb->b.data.board_type==EIGHT_FOUR || rb->b.data.board_type==EIGHT_TWO) && (static_cast<int>(rb->b.data.w_rook_1) != static_cast<int>(DEAD)) ) {white += 9;}
    else if(static_cast<int>(rb->b.data.w_rook_1) != static_cast<int>(DEAD)){white += 5;}
    if((rb->b.data.board_type==EIGHT_FOUR || rb->b.data.board_type==EIGHT_TWO) && (static_cast<int>(rb->b.data.w_rook_2) != static_cast<int>(DEAD)) ) {white += 9;}
    else if(static_cast<int>(rb->b.data.w_rook_2) != static_cast<int>(DEAD)){white += 5;}
    if(static_cast<int>(rb->b.data.w_pawn_1) != static_cast<int>(DEAD) ) white += 1;
    if(static_cast<int>(rb->b.data.w_pawn_2) != static_cast<int>(DEAD) ) white += 1;
    if((rb->b.data.board_type==EIGHT_FOUR || rb->b.data.board_type==EIGHT_TWO) && static_cast<int>(rb->b.data.w_pawn_3) != static_cast<int>(DEAD) ) white += 1;
    if((rb->b.data.board_type==EIGHT_FOUR || rb->b.data.board_type==EIGHT_TWO) && static_cast<int>(rb->b.data.w_pawn_4) != static_cast<int>(DEAD) ) white += 1;
    if(rb->b.data.board_type==EIGHT_TWO && static_cast<int>(rb->b.data.w_knight_1) != static_cast<int>(DEAD) ) white += 6;
    if(rb->b.data.board_type==EIGHT_TWO && static_cast<int>(rb->b.data.w_knight_2) != static_cast<int>(DEAD) ) white += 6;
    if(static_cast<int>(rb->b.data.w_bishop) != static_cast<int>(DEAD) ) white += 3;

    if(current_player == true){ 
        // cout<<"white-black*************************************** "<<white-black<<" white is----------------"<<white<<" black is-------------"<<black<<endl;
        return white-black;}
    return black-white;
}

double compute_threats(RollerBoard* rb){
    U8 b_others[10] = {rb->b.data.b_rook_2,rb->b.data.b_rook_1,rb->b.data.b_king,rb->b.data.b_pawn_2,rb->b.data.b_pawn_1,rb->b.data.b_bishop,rb->b.data.b_knight_1,rb->b.data.b_knight_2,rb->b.data.b_pawn_3,rb->b.data.b_pawn_4};
    U8 w_others[10] = {rb->b.data.w_rook_2,rb->b.data.w_rook_1,rb->b.data.w_king,rb->b.data.w_pawn_2,rb->b.data.w_pawn_1,rb->b.data.w_bishop,rb->b.data.w_knight_1,rb->b.data.w_knight_2,rb->b.data.w_pawn_3,rb->b.data.b_pawn_4};
    double white_threats = 0;
    double black_threats = 0;
    if((rb->b.data.board_type==EIGHT_FOUR || rb->b.data.board_type==EIGHT_TWO) && rb->b.under_threat(b_others[0])){ black_threats+= 9;}
    else if(rb->b.under_threat(b_others[0])){black_threats+= 5; }
    if((rb->b.data.board_type==EIGHT_FOUR || rb->b.data.board_type==EIGHT_TWO) && rb->b.under_threat(b_others[1])){ black_threats+= 9;}
    else if(rb->b.under_threat(b_others[1])){black_threats+= 5; }
    if(rb->b.under_threat(b_others[2])){ black_threats+= 100000;}
    if(rb->b.under_threat(b_others[3])){ black_threats+= 1;}
    if(rb->b.under_threat(b_others[4])){ black_threats+= 1;}
    if(rb->b.under_threat(b_others[5])){ black_threats+= 3;}
    if(rb->b.data.board_type==EIGHT_TWO && (rb->b.under_threat(b_others[6]))){ black_threats+= 6;}
    if(rb->b.data.board_type==EIGHT_TWO && (rb->b.under_threat(b_others[7]))){ black_threats+= 6;}
    if((rb->b.data.board_type==EIGHT_FOUR || rb->b.data.board_type==EIGHT_TWO) && (rb->b.under_threat(b_others[8]))){ black_threats+= 1;}
    if((rb->b.data.board_type==EIGHT_FOUR || rb->b.data.board_type==EIGHT_TWO) && (rb->b.under_threat(b_others[9]))){ black_threats+= 1;}

    if((rb->b.data.board_type==EIGHT_FOUR || rb->b.data.board_type==EIGHT_TWO) && rb->b.under_threat(w_others[0])){ white_threats+= 9;}
    else if(rb->b.under_threat(w_others[0])){white_threats+= 5; }
    if((rb->b.data.board_type==EIGHT_FOUR || rb->b.data.board_type==EIGHT_TWO) && rb->b.under_threat(w_others[1])){ white_threats+= 9;}
    else if(rb->b.under_threat(w_others[1])){white_threats+= 5; }
    if(rb->b.under_threat(w_others[2])){ white_threats+= 100000;}
    if(rb->b.under_threat(w_others[3])){ white_threats+= 1;}
    if(rb->b.under_threat(w_others[4])){ white_threats+= 1;}
    if(rb->b.under_threat(w_others[5])){ white_threats+= 3;}    
    if(rb->b.data.board_type==EIGHT_TWO && (rb->b.under_threat(w_others[6]))){ white_threats+= 6;} 
    if(rb->b.data.board_type==EIGHT_TWO && (rb->b.under_threat(w_others[7]))){ white_threats+= 6;} 
    if((rb->b.data.board_type==EIGHT_FOUR || rb->b.data.board_type==EIGHT_TWO) && (rb->b.under_threat(w_others[8]))){ white_threats+= 1;}
    if((rb->b.data.board_type==EIGHT_FOUR || rb->b.data.board_type==EIGHT_TWO) && (rb->b.under_threat(w_others[9]))){ white_threats+= 1;}
    // cout<<"----------------------the threats difference---------------------- "<<"black_threats "<<black_threats<<" white threats "<<white_threats<<endl;
    // cout<<"CURRENT PLAYER "<<current_player<<endl;
    if(current_player == true){ 
        return black_threats-white_threats;
    }
    return white_threats-black_threats;
}

bool isPathClear(U8 start, U8 end, RollerBoard* rb){
    if(get_y(start)==get_y(end)){
        int dir=(get_x(start)<get_x(end))? 1: -1;
        for(int x = get_x(start)+dir; x!=get_x(end);x+=dir){
            if(rb->b.data.board_0[get_pos(x,get_y(start))]!=0){
                return false;
            }
        }
    }
    else if(get_x(start)==get_x(end)){
        int dir = (get_y(start)<get_y(end))?1:-1;
        for(int y=get_y(start)+dir;y!=get_y(end);y+=dir){
            if(rb->b.data.board_0[get_pos(get_x(start),y)]!=0){
                return false;
            }
        }
    }
    return true;
}
double king_safety(RollerBoard* rb){
    int adjacent[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    double black_threat_count = 0;
    double white_threat_count = 0;
    bool flag = false;
    if(rb->b.data.player_to_play == WHITE) flag = true;
    U8 king_w = rb->b.data.w_king;
    int king_w_x = get_x(king_w);
    int king_w_y = get_y(king_w);
    U8 king_b = rb->b.data.b_king;
    int king_b_x = get_x(king_b);
    int king_b_y = get_y(king_b);
    for(int i=0;i<8;i++){
        int new_w_X = king_w_x+adjacent[i][0]; 
        int new_w_Y = king_w_x+adjacent[i][1];
        int new_b_X = king_b_x+adjacent[i][0];
        int new_b_Y = king_b_y+adjacent[i][1];
        // if(flag == true){
        if(new_w_X>=0 && new_w_X<7 && new_w_Y>=0 && new_w_X<7){
            U8 p =rb->b.data.board_0[get_pos(new_w_X,new_w_Y)];
            if( p == BLACK|ROOK) white_threat_count+= 4.5;
            else if( p== BLACK|BISHOP) white_threat_count+= 3.5;
            else if( p==BLACK|PAWN) white_threat_count+= 1.5;
            else if( p==BLACK|KING ) white_threat_count+= 6;
        }
        // }
        // else{
        if(new_b_X>=0 && new_b_X<7 && new_b_Y>=0 && new_b_X<7){
            U8 p =rb->b.data.board_0[get_pos(new_b_X,new_b_Y)];
            if( p == WHITE|ROOK) black_threat_count+= 4.5;
            else if( p== WHITE|BISHOP) black_threat_count+= 3.5;
            else if( p==WHITE|PAWN) black_threat_count+= 1.5;
            else if( p==WHITE|KING ) black_threat_count+= 6;
        }  
        // }      
    }
    if(current_player==true){
        // cout<<"black_king --------"<<black_threat_count<<" white_king -------- "<<white_threat_count<<endl;
        return black_threat_count-white_threat_count;}
    return white_threat_count-black_threat_count;
}

double evaluate_pawn_promotion(RollerBoard* rb){
    vector<U8> white_pos;
    vector<U8> black_pos;
    // if(rb->b.data.board_type == SEVEN_THREE) {
        white_pos =  {get_pos(2,6), get_pos(3,6), get_pos(2,5)};
        black_pos = {get_pos(4,1), get_pos(4,0), get_pos(3,0)};
    // }
    // else if(rb->b.data.board_type == EIGHT_FOUR) {
    //     white_pos = {get_pos(4,7), get_pos(3,6), get_pos(3,7)};
    //     black_pos = {get_pos(4,1), get_pos(4,0), get_pos(3,0)};
    // }
    // else {
    //     white_pos = {get_pos(4,6), get_pos(4,5), get_pos(3,5),get_pos(2,6), get_pos(2,7)};
    //     black_pos = {get_pos(3,1), get_pos(3,2), get_pos(4,2),get_pos(5,1), get_pos(5,0)};
    // }
    
    double black_score =0;
    double white_score =0;
    U8 w_p1 = rb->b.data.w_pawn_1;
    U8 w_p2 = rb->b.data.w_pawn_2;
    U8 w_p3 = rb->b.data.w_pawn_3;
    U8 w_p4 = rb->b.data.w_pawn_4;
    U8 b_p1 = rb->b.data.b_pawn_1;
    U8 b_p2 = rb->b.data.b_pawn_2;
    U8 b_p3 = rb->b.data.b_pawn_3;
    U8 b_p4 = rb->b.data.w_pawn_4;
    //----------------------w_p1--------------------------------
    for(U8 e:white_pos){
        if(isPathClear(w_p1,e,rb)){
            double distance = abs(get_x(e)-get_x(w_p1))+abs(get_y(e)-get_y(w_p1));
            white_score += (14-distance)*0.3;
        }
        if(isPathClear(w_p2,e,rb)){
            double distance = abs(get_x(e)-get_x(w_p2))+abs(get_y(e)-get_y(w_p2));
            white_score += (14-distance)*0.3;
        }
        // if((rb->b.data.board_type == EIGHT_FOUR ||rb->b.data.board_type == EIGHT_TWO) && isPathClear(w_p3,e,rb)){
        //     double distance = abs(get_x(e)-get_x(w_p3))+abs(get_y(e)-get_y(w_p3));
        //     white_score += (14-distance)*0.3;
        // // }
        // if((rb->b.data.board_type == EIGHT_FOUR ||rb->b.data.board_type == EIGHT_TWO) && isPathClear(w_p4,e,rb)){
        //     double distance = abs(get_x(e)-get_x(w_p4))+abs(get_y(e)-get_y(w_p4));
        //     white_score += (14-distance)*0.3;
        // }
    }
    for(U8 e:black_pos){
        if(isPathClear(b_p1,e,rb)){
            double distance = abs(get_x(e)-get_x(b_p1))+abs(get_y(e)-get_y(b_p1));
            black_score += (14-distance)*0.3;
        }
        if(isPathClear(b_p2,e,rb)){
            double distance = abs(get_x(e)-get_x(b_p2))+abs(get_y(e)-get_y(b_p2));
            black_score += (14-distance)*0.3;
        }
        // if((rb->b.data.board_type == EIGHT_FOUR ||rb->b.data.board_type == EIGHT_TWO) && isPathClear(b_p3,e,rb)){
        //     double distance = abs(get_x(e)-get_x(b_p3))+abs(get_y(e)-get_y(b_p3));
        //     black_score += (14-distance)*0.3;
        // }
        // if((rb->b.data.board_type == EIGHT_FOUR ||rb->b.data.board_type == EIGHT_TWO) && isPathClear(b_p4,e,rb)){
        //     double distance = abs(get_x(e)-get_x(b_p4))+abs(get_y(e)-get_y(b_p4));
        //     black_score += (14-distance)*0.3;
        // }
    } 
    if(current_player==true){ 
        // cout<<"Pawn value is++++++++++++++++++++++++++++++++        "<<black_score<<"       "<<white_score<<endl;
        return white_score-black_score;}
    else{ return black_score-white_score;}

}

double sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}

double evaluation(RollerBoard* b){
    // if(b->b.in_check() && (b->b.data.player_to_play == WHITE && current_player== false) || (b->b.data.player_to_play == BLACK && current_player== true) ){
    //     return -1000; 
    // }
    // else if(b->b.in_check()){
    //     return 1000; 
    // }
    double c = compute_piece_value(b);
    double n;
    n = compute_threats(b);
    // else n = 0;
    // double a = king_safety(b);
    double p = evaluate_pawn_promotion(b);
    // 3,1.5 ---- 2nd and 3rd board
    if(b->b.data.board_type == SEVEN_THREE) return c*3 + n*0.5 + p*0.25;
    return  c*3 + n*1.5 + p*0.75;
}

// --------------------------------------------------------------------SEARCH--------------------------------------------------------------------
bool max_compare(RollerBoard* r1, RollerBoard* r2){
    return r1->eval_val>r2->eval_val;
}
bool min_compare(RollerBoard* r1, RollerBoard* r2){
    return r1->eval_val<r2->eval_val;
}

double min_val(RollerBoard* rb, double alpha, double beta, int d, chrono::time_point<chrono::system_clock>start, double time_limit, double parent_val);
double max_val(RollerBoard* rb, double alpha, double beta, int d, chrono::time_point<chrono::system_clock>start, double time_limit, double parent_val){
    chrono::duration<double> elapsed_time = chrono::system_clock::now()-start;
    // cout<<"Begin\n";
    if (elapsed_time.count()>time_limit){
        TimeException t;
        throw t;
    }
    vector<int> my_pieces = get_my_pieces(rb);
    vector<int> opp_pieces = get_opp_pieces(rb);
    int my_total = my_pieces[0]+my_pieces[1]+my_pieces[2]+my_pieces[3];
    int opp_total = opp_pieces[0]+opp_pieces[1]+opp_pieces[2]+opp_pieces[3];
    if(d==0){
        // cout<<"in d==0\n";
        double e = evaluation(rb);
        return e;
    }
    if(rb->children.size()==0){
        // cout<<"-------calling create states------"<<endl;
        rb->create_states();
        // cout<<"---------after create states-------"<<endl;
    }
    if(rb->children.size()==0){
        // cout<<" Before return in 0 size"<<endl;
        return ((my_total-opp_total)*4 -2)*50;
    }
    // cout<<"children states created\n";
    double v = numeric_limits<double>::min();  // minimum value
    double temp = 0;
    unsigned long long int h;
    for(auto i:rb->children){
        h = get_hash(i->value_matrix);
        // cout<<"max_hash is "<<h<<endl;
        if(transposition_table.find(h)==transposition_table.end()){
            // cout<<"Before min_val call"<<endl;
            temp = min_val(i,alpha,beta,d-1,start,time_limit,parent_val);
            // cout<<"After min_val "<<temp<<endl;
        }
        else{
            // cout<<"No need to call min_val"<<endl;
            temp = transposition_table.at(h);
            // cout<<"temp is "<<temp<<endl;
            // cout<<"In else"<<endl;
        }
        v = max(v,temp);
        parent_val = v;
        // cout<<"Before eval_val"<<endl;
        i->eval_val = temp;
        // cout<<"After eval_val "<<rb.eval_val<<endl;
        // if(temp>numeric_limits<double>::min() && temp<numeric_limits<double>::max()) transposition_table[h]=temp;
        // transposition_table[h]=temp;
        if(v>=beta){
            sort(rb->children.begin(),rb->children.end(),max_compare);
            // cout<<"---------Beta prunes-------------------"<<endl;
            return v;
        }
        // cout<<"Before alpha"<<endl;        
        alpha = max(alpha,v);
    }
    sort(rb->children.begin(),rb->children.end(),max_compare);
    return v;
}

double min_val(RollerBoard* rb, double alpha, double beta, int d, chrono::time_point<chrono::system_clock>start, double time_limit, double parent_val){
    chrono::duration<double> elapsed_time = chrono::system_clock::now()-start;
    // cout<<"min_Begin\n";
    if (elapsed_time.count()>time_limit){
        TimeException t;
        throw t;
    }
    vector<int> my_pieces = get_my_pieces(rb);
    vector<int> opp_pieces = get_opp_pieces(rb);
    int my_total = my_pieces[0]+my_pieces[1]+my_pieces[2]+my_pieces[3];
    // cout<<"min_my_total is "<<my_total<<endl;
    int opp_total = opp_pieces[0]+opp_pieces[1]+opp_pieces[2]+opp_pieces[3];
    // cout<<"min_opp_total is "<<opp_total<<endl;
    if(d==0){
        // cout<<"min_in d==0\n";
        double e = evaluation(rb);
        // cout<<"min evaluated val is "<<e<<" parent_val is "<<parent_val<<endl;
        return e;
    }
    if(rb->children.size()==0){
        rb->create_states();
    }
    if(rb->children.size()==0){
        // cout<<"child states is zero"<<endl;
        return ((my_total-opp_total)*4 -2)*50;
    }
    // cout<<"min_children states created\n";
    double v = numeric_limits<double>::max();  // minimum value
    double temp = 0;
    unsigned long long int h;
    for(auto i:rb->children){
        h = get_hash(i->value_matrix);
        // cout<<"min_hash is "<<h<<endl;
        if(transposition_table.find(h)==transposition_table.end()){
            // cout<<"min_Before max_val"<<endl;
            temp = max_val(i,alpha,beta,d-1,start,time_limit,parent_val);
            // cout<<"min_After max_val "<<temp<<endl;
        }
        else{
            // cout<<"No need to call max_val"<<endl;
            temp = transposition_table.at(h);
            // cout<<"Temp_max is "<<temp<<endl;
            // cout<<"min_In else"<<endl;
        }
        v = min(v,temp);
        parent_val = v;
        // cout<<"min_Before eval_val"<<endl;
        i->eval_val = temp;
        // cout<<"min_After eval_val "<<rb.eval_val<<endl;
        // if(temp>numeric_limits<double>::min() && temp<numeric_limits<double>::max()) transposition_table[h]=temp;
        // transposition_table[h]=temp;
        if(v<=alpha){
            sort(rb->children.begin(),rb->children.end(),min_compare);
            // cout<<"------------Alpha prunes----------------------"<<endl;
            return v;
        }
        beta = min(beta,v);
    }
    sort(rb->children.begin(),rb->children.end(),min_compare);
    return v;
}

U16 alpha_beta_search(RollerBoard* b, int d, chrono::time_point<chrono::system_clock> start,double time_limit){
    // cout<<"in alpha beta-----------------------------------"<<endl;
    int value = 0;
    double maxi = numeric_limits<double>::max();  // maximum value
    double mini = numeric_limits<double>::min();  // minimum value
    value = max_val(b,mini,maxi,d,start,time_limit,mini);
    b->eval_val = value;
    if(b->children.size()>0) return b->children[0]->moves.at(b->children[0]->moves.size()-1);
    abort();   
}

U16 iterative_deepening(RollerBoard* b, double time_left, double total_time){
    initTable();

    int depth = 6;
    int d = 1;
    double time_limit = 2;
    U16 best_move = 0;
    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    try{
        while(d<depth){
            cout<<"started with depth "<<d<<endl;
            transposition_table.erase(transposition_table.begin(),transposition_table.end());
            best_move = alpha_beta_search(b,d,start,time_limit);
            d++;
            // break;
        }
    }
    catch(const exception& TimeException){
        return best_move;
    }
    return best_move;
}

void Engine::find_best_move(const Board& b) {
    if(b.data.player_to_play == WHITE){ current_player = true;}
    RollerBoard* rb = new RollerBoard(b);
    // cout<<"id started"<<endl;
    this->best_move = iterative_deepening(rb,1,1);
    // chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    // this->best_move = alpha_beta_search(rb,4,start,2);
}
