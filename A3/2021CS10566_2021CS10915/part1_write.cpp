#include<bits/stdc++.h>
using namespace std;

int N,M,k1,k2;

int main(int argc, char** argv) {
    string param = (string)argv[1];
    // cout<<param<<"\n";
    ifstream graph, output;
    ofstream mapping;
    graph.open(param + ".graph");
    graph>>N>>M>>k1>>k2;
    output.open(param + ".satoutput");
    mapping.open("test.mapping");
    string s;
    output>>s;
    if (s=="UNSAT") {
        mapping<<"0";
        return 0;
    }
    mapping<<"#1\n";
    int c1=0,c2=0,v;
    bool b=false;
    for (int i=0; i<N; i++) {
        output>>v;
        if (v>0 && c1<k1) {
            c1++;
            if (b) mapping<<" "<<v;
            else {
                mapping<<v;
                b=true;
            }
        }
    }
    mapping<<"\n#2\n";
    b=false;
    for (int i=0; i<N; i++) {
        output>>v;
        if (v>0 && c2<k2) {
            c2++;
            if (b) mapping<<" "<<v-N;
            else {
                mapping<<v-N;
                b=true;
            }
        }
    }
    // cout<<"\n";
}