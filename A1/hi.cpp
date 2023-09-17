#include<bits/stdc++.h>
#include <random>
using namespace std;

// auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
// std::mt19937 mt(seed);

int z=10,l=20;

vector<int>f(){
    vector<int>v(l);
    iota(v.begin(),v.end(),1);
    random_device rd;
    mt19937 rng(rd());
    shuffle(v.begin(),v.end(),rng);
    vector<int>gib(z);
    for(int i=0;i<z;i++){
        gib[i]=v[i];
    }
    return gib;
}

void solve(){
    vector<int>v=f();
    // for(auto &ch:v){
    //     cout<<ch<<" ";
    // }
    // for(int i=0;i<10;i++){
    //     cout<<mt()<<'\n';
    // }

    for(int i=0;i<z;i++){
        cout<<v[i]<<" ";
    }
    cout<<'\n';
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t=10;
    // cin>>t;
    while(t--){
        solve();
    }
    return 0;
}