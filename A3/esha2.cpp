#include<bits/stdc++.h>

using namespace std;

int N,M;

vector<bool> ValidNode;

vector<int> DegreeCount;
string param, command;
// struct  edge{
//     int a,b;
//     edge(int _a,int _b) : a(_a), b(_b){}
//     bool operator<(const edge& other) const {
//         if (a != other.a) {
//             return a < other.a;
//         }
//         return b < other.b;
//     }
// };

// return id of the following fresh variable
inline int gi(int g){ return g;}

inline int sij(int i,int j,int k){ return (N+1)+i*(k+1)+j; }

// inline int eij(int i,int j){ return (i-1)*N+j; }

vector<int> Gdegree;
// map<int,int> Gdegree;

// map<edge,int> edge_id;

vector<vector<bool>> edge_check;

void AddConstraint(int a,int b,int c,int d,vector<vector<int>>&Tclauses){

    Tclauses.push_back({-b,a});
    Tclauses.push_back({-c,-d,a});
    Tclauses.push_back({c,b,-a});
    Tclauses.push_back({d,b,-a});

}

bool runMiniSAT() {
    // Replace "minisat" with the actual path to the MiniSAT executable if needed
    const char* minisatCommand = command.c_str();
    // cout << minisatCommand << "\n";
    int exitCode = std::system(minisatCommand);

    // if (exitCode == 0) {

        std::cout << "MiniSAT executed successfully." << std::endl;

        ifstream Output(param+".satoutput");

        string s;

        Output>>s;


        if(s=="UNSAT") return false;

        return true;

    // } else {

    //     std::cerr << "MiniSAT execution failed with exit code " << exitCode << "." << std::endl;

    // }

    // return false;
}


bool MakeInput( int k , vector<vector<int>>&Tclauses ){

    ofstream inpFile(param+".satinput");

    inpFile<<"p cnf "<<N+(N+1)*(k+1)<<" "<<Tclauses.size()<<endl;

    for( int i=0 ; i<Tclauses.size() ; i++ ){

        for(int x : Tclauses[i]){

            inpFile<<x<<" ";

        }

        inpFile<<"0\n";

    }

    inpFile.close();

    return runMiniSAT();

}

int check(int k,int ans){

    if(DegreeCount[k-1]<k)return false;

    vector<vector<int>> Tclauses;

    for( int i=1 ; i<=N ; i++ ){

        for( int j = i+1 ; j<=N ; j++ ){

            if( !edge_check[i][j] ){

                Tclauses.push_back({-gi(i),-gi(j)});

            }
        }
    }

    for(int j=1 ; j<=k ; j++){

        Tclauses.push_back({-sij(0,j,k)});

    }

    for(int i=0 ; i<=N ; i++){

        Tclauses.push_back({sij(i,0,k)});

    }

    for(int i=1 ; i<=N ; i++){

        for(int j=1 ; j<=k ; j++){

            AddConstraint( sij(i,j,k) , sij(i-1,j,k) , gi(i) , sij(i-1,j-1,k) , Tclauses );

        }

    }

    // enforcing for k1 to be true
    Tclauses.push_back( {  sij(N, k  , k) } );

    for (int i=1; i<=N; i++) {
        if (Gdegree[i]<k-1) Tclauses.push_back({-gi(i)});
    }
    // Tclauses.push_back( { -sij(N, k+1, k) } );

    bool t=MakeInput(k,Tclauses);

    if(!t) return -1;
    int c=0;
    if(ans<k){
        
        ifstream Output(param+".satoutput");
        string s;
        Output>>s;
        int Node;
        for(int i=1;i<=N;i++){
            Output>>Node;
            if(Node>0) {
                ValidNode[i]=true;
                c++;
            }
            else ValidNode[i]=false;
        }
        
    }
    return c;
}




void solve(){
    ifstream graph(param+".graph");
    graph>>N>>M;
    // ofstream debug("d.txt");
    ValidNode.assign(N+1,false);
    DegreeCount.assign(N+1,0);
    Gdegree.assign(N+1,0);
    edge_check.assign(N+1,vector<bool>(N+1,false));
    // int Gdegree[N] = {0};
    for(int i=0;i<M;i++){

        int a,b,a1,b1;graph>>a1>>b1;

        a=min(a1,b1),b=max(a1,b1);

        edge_check[a][b]=true;
        edge_check[b][a]=true;

        // edge_id[edge(a,b)]=eij(a,b);

        Gdegree[a]++;
        Gdegree[b]++;
    }




    int l=1,r=1;

    for(int i=1;i<=N;i++){
        int a=Gdegree[i];
        DegreeCount[a]++;
        r=max(r,a);
    }

    for(int i=N-1;i>0;i--)DegreeCount[i]+=DegreeCount[i+1];

    int ans=1;

    while(l<=r){

        int mid=(l+r)/2;
        int c=check(mid,ans);
        // debug<<mid<<" "<<c<<"\n";
        if(c!=-1){

            ans=max(ans,mid);

            l=c+1;

        }else{

            r=mid-1;

        }
    }

    ofstream OutFile(param+".mapping");
    OutFile<<"#1\n";
    bool b=false;
    for(int i=1;i<=N;i++) if(ValidNode[i]) {
        if (b) OutFile<<" "<<i;
        else {
            OutFile<<i;
            b=true;
        }
    }
    // OutFile<<endl;
    OutFile.close();


}
int main(int argc, char** argv){
    auto start_time = std::chrono::high_resolution_clock::now();
    param = (string)argv[1];
    command = "minisat "+param+".satinput "+param+".satoutput";
    solve();
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the duration in milliseconds
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Print the execution time
    std::cout << "Execution Time: " << duration.count() << " milliseconds" << std::endl;
}