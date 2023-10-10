#include<bits/stdc++.h>
using namespace std;

struct DSU{
    vector<int> parent;
    vector<int> siz;

    DSU(int n){
        parent.assign(n+1,n+1);
        siz.assign(n+1,1);
        for(int i=0;i<=n;i++){
            make(i);
        }
    }
    void make(int v){
        parent[v]=v;
        siz[v]=1;
    }
    int find(int v){
        if(parent[v]==v){
            return v;
        }
        return parent[v]=find(parent[v]);
    }
    void Union(int a , int b){
        a=find(a);
        b=find(b);
        if(a!=b){
            parent[b]=a;
            siz[a]+=siz[b];
        }
    }
    int sz(int v){
        return siz[find(v)];
    }
};


void solve(){
    int n;cin>>n;
    DSU dsu(n);
    vector<int> va(n),vb(n);
    for(int i=0;i<n;i++)cin>>va[i];
    for(int i=0;i<n;i++)cin>>vb[i];
    int m;cin>>m;
    multiset<int> sz;
    for(int i=0;i<m;i++){
        int a;cin>>a;
        sz.insert(a);
    }
    set<pair<int,int>> vbsorted;
    for(int i=0;i<n;i++)vbsorted.insert({vb[i],i});
    vector<bool> check(n,false);
    bool t=true;
    while(!vbsorted.empty() && !sz.empty()){
        auto [x,i]=*vbsorted.begin();
        auto hcut=*sz.begin();
        if(x<min(va[i],hcut)){
            t=false;break;
        }
        if(x>min(va[i],hcut)){
            sz.erase(sz.find(hcut));
            continue;
        }
        sz.erase(sz.find(hcut));
        while(i<n && vb[i]<=min(va[i],hcut)){
            vbsorted.erase({vb[i],i});
            dsu.Union(i+1,i);
            i=dsu.find(i+1);
        }
    }
    if(t)cout<<"YES";
    else cout<<"NO";
    cout<<endl;



}
int main(){
    int t;cin>>t;
    while(t--)solve();
}