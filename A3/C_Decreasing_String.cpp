#ifndef ONLINE_JUDGE
// #include "debugger.hpp"
#else
#define debug(...) 42
#endif
#include<bits/stdc++.h>
using namespace std;


// ===================================#define==============================================
#define dbg(v)                                                                 \
    cout << "Line(" << __LINE__ << ") -> " << #v << " = " << (v) << endl;
#define int long long
#define cyes cout<<"YES\n"
#define cno cout<<"NO\n"
#define ed "\n"

 
const int inf=(int)1e17+5;
const int mod=(int)1e9+7;
const int N=2*(1e5+5);

// ===================functions========================
void pstr(string s){cout<<s<<endl;}
void pint(int n){cout<<n<<endl;}



void solve( )
{
    string s;cin>>s;
    int pos,n=s.size();
    cin>>pos;
    int k=0,u=0;
    while(true){
        if(u+n-k<pos){
            pos-=n-k;
            k++;
        }else break;
    }
    stack<char> st;
    for(int i=0;i<n;i++){
        if(st.empty())st.push(s[i]);
        else if(st.top()<=s[i])st.push(s[i]);
        else{
            while(!st.empty() && k && st.top()>s[i]){
                st.pop();
                k--;
            }
            st.push(s[i]);
        }
    }
    string su="";
    while (!st.empty())
    {
        su+=st.top();
        st.pop();
    }
    reverse(su.begin(),su.end());
    cout<<su[pos-1];
    

}
 
 
int32_t main()
{
    #ifndef ONLINE_JUDGE
    freopen("Error.txt", "w", stderr);
    #endif
    ios::sync_with_stdio(0); cin.tie(0);
    int t=1;    
    cin>>t;
    while(t-->0)
    {
        solve();
    }
}