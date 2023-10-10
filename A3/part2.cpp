#include<bits/stdc++.h>

using namespace std;

int N,M;

struct  edge{
    int a,b;
    edge(int _a,int _b) : a(_a), b(_b){}
    bool operator<(const edge& other) const {
        if (a != other.a) {
            return a < other.a;
        }
        return b < other.b;
    }
};

vector<bool> ValidNode;

vector<int> DegreeCount;

// return id of the following fresh variable
inline int gi(int g){ return g;}

inline int sij(int i,int j,int k){ return (N+1)+i*(k+2)+j; }

inline int eij(int i,int j){ return (i-1)*N+j; }


map<int,int> Gdegree;

map<edge,int> edge_id;

// vector<vector<bool>> edge_check;

void AddConstraint(int a,int b,int c,int d,vector<vector<int>>&Tclauses){

    Tclauses.push_back({-b,a});
    Tclauses.push_back({-c,-d,a});
    Tclauses.push_back({c,b,-a});
    Tclauses.push_back({d,b,-a});

}

bool runMiniSAT() {
    // Replace "minisat" with the actual path to the MiniSAT executable if needed
    const char* minisatCommand = "minisat inp.txt out.txt";

    int exitCode = std::system(minisatCommand);

    // if (exitCode == 0) {

        std::cout << "MiniSAT executed successfully." << std::endl;

        ifstream Output("out.txt");

        string s;

        Output>>s;


        if(s=="UNSAT") return false;

        return true;

}


bool MakeInput( int k , vector<vector<int>>&Tclauses ){

    ofstream inpFile("inp.txt");

    inpFile<<"p cnf "<<2*N+(N+1)*(k+2)<<" "<<Tclauses.size()<<endl;

    for( int i=0 ; i<Tclauses.size() ; i++ ){

        for(int x : Tclauses[i]){

            inpFile<<x<<" ";

        }

        inpFile<<"0\n";

    }

    inpFile.close();

    return runMiniSAT();

}

bool check(int k,int ans){

    if(DegreeCount[k-1]<k)return false;

    vector<vector<int>> Tclauses;

    for( int i=1 ; i<=N ; i++ ){

        for( int j = i+1 ; j<=N ; j++ ){

            if( edge_id.find(edge(i,j))==edge_id.end()){

                Tclauses.push_back({-gi(i),-gi(j)});

            }
        }
    }

    for(int j=1 ; j<=k+1 ; j++){

        Tclauses.push_back({-sij(0,j,k)});

    }

    for(int i=0 ; i<=N ; i++){

        Tclauses.push_back({sij(i,0,k)});

    }

    for(int i=1 ; i<=N ; i++){

        for(int j=1 ; j<=k+1 ; j++){

            AddConstraint( sij(i,j,k) , sij(i-1,j,k) , gi(i) , sij(i-1,j-1,k) , Tclauses );

        }

    }

    // enforcing for k1 to be true
    Tclauses.push_back( {  sij(N, k  , k) } );

    Tclauses.push_back( { -sij(N, k+1, k) } );

    bool t=MakeInput(k,Tclauses);

    if(!t)return t;

    if(ans<k){
        
        ifstream Output("out.txt");
        string s;
        Output>>s;
        int Node;
        for(int i=1;i<=N;i++){
            Output>>Node;
            if(Node>0)ValidNode[i]=true;
            else ValidNode[i]=false;
        }
        
    }
    return true;
}




void solve(){

    cin>>N>>M;

    ValidNode.assign(N+1,false);
    DegreeCount.assign(N+1,0);
    // edge_check.assign(N+1,vector<bool>(N+1,false));

    for(int i=0;i<M;i++){

        int a,b,a1,b1;cin>>a1>>b1;

        a=min(a1,b1),b=max(a1,b1);

        // edge_check[a][b]=true;
        // edge_check[b][a]=true;

        edge_id[edge(a,b)]=eij(a,b);

        Gdegree[a]++;
        Gdegree[b]++;
    }




    int l=2,r=2;

    for(int i=1;i<=N;i++){
        int a=Gdegree[i];
        DegreeCount[a]++;
        r=max(r,a);
    }

    for(int i=N-1;i>0;i--)DegreeCount[i]+=DegreeCount[i+1];

    int ans=2;

    while(l<=r){

        int mid=(l+r)/2;

        if(check(mid,ans)){

            ans=max(ans,mid);

            l=mid+1;

        }else{

            r=mid-1;

        }
    }

    ofstream OutFile("out.txt");
    vector<int> v;
    for(int i=1;i<=N;i++)if(ValidNode[i])v.push_back(i);
    OutFile<<"#1"<<"\n";
    OutFile<<v.size()<<"\n";
    for(int x: v)OutFile<<x<<" ";
    OutFile.close();


}
int main(){
    solve();
}