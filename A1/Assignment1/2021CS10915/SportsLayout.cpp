#include <fstream>
#include <iostream>
#include <random>
#include <bits/stdc++.h>


using namespace std;

#include "SportsLayout.h"
auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
mt19937 mt(seed);
long long inf=(long long)(1e18);
long long costmin=(long long)(1e18);
int iterations;
int restart;
long long climb;
double mult=60;
double ltime=1.5;
int shufflestate;
auto startTime =chrono::high_resolution_clock::now();

SportsLayout::SportsLayout(string inputfilename)
{
        
    readInInputFile(inputfilename);
    mapping= new int[z];

}

bool SportsLayout::check_output_format()
{

    vector<bool> visited(l,false);
    for(int i=0;i<z;i++)
    {
        if((mapping[i]>=1 && mapping[i]<=l))
        {
            if(!visited[mapping[i]-1])
            visited[mapping[i]-1]=true;
            else
            {
                cout<<"Repeated locations, check format\n";
                return false;
            }
        }
        else
        {
            cout<<"Invalid location, check format\n";
            return false;
        }
    }

    return true;

}

// void SportsLayout::readOutputFile(string output_filename)
// {
//         fstream ipfile;
//         ipfile.open(output_filename, ios::in);
//         if (!ipfile) {
//             cout << "No such file\n";
//             exit( 0 );
//         }
//         else {
            
//             vector<int> ip;

//             while (1) {
//                 int t;
//                 ipfile >> t;
//                 ip.push_back(t);
//                 if (ipfile.eof())
//                     break;
//             }
        
//         if(ip.size()!=z)
//         {
//             cout<<"number of values not equal to number of zones, check output format\n";
//             exit(0);
//         }
//         for(int i=0;i<z;i++)
//         mapping[i]=ip[i];
//     ipfile.close();

//     if(!check_output_format())
//         exit(0);
//     cout<<"Read output file, format OK"<<endl;

//         }
    
// }


long long SportsLayout::cost_fn(){


    long long cost=0;

    for(int i=0;i<z;i++)
    {
        for(int j=0;j<z;j++) 
        {
            cost+=(long long)N[i][j]*(long long)T[mapping[i]-1][mapping[j]-1];
        }
    }

    return cost;
}

void SportsLayout::readInInputFile(string inputfilename)
{
        fstream ipfile;
        ipfile.open(inputfilename, ios::in);
        if (!ipfile) {
            cout << "No such file\n";
            exit( 0 );
        }
        else {
            

            ipfile>> time;
            ipfile >> z;
            ipfile >> l;

            if(z>l)
            {
                cout<<"Number of zones more than locations, check format of input file\n";
                exit(0);
            }
        int **tempT;
        int **tempN;

        tempT = new int*[l];
        for (int i = 0; i < l; ++i)
        tempT[i] = new int[l];
    
    tempN = new int*[z];
    for (int i = 0; i < z; ++i)
        tempN[i] = new int[z];

    for(int i=0;i<z;i++)
    {
        for(int j=0;j<z;j++)
        ipfile>>tempN[i][j];
    }

    for(int i=0;i<l;i++)
    {
        for(int j=0;j<l;j++)
        ipfile>>tempT[i][j];
    }

    ipfile.close();

    T= tempT;
    N= tempN;
        }

}

void SportsLayout::write_to_file(string outputfilename){

    // Open the file for writing
    ofstream outputFile(outputfilename);

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        cerr << "Failed to open the file for writing." << std::endl;
        exit(0);
    }

    for(int i=0;i<z;i++)
    outputFile<<mapping[i]<<" ";

    // Close the file
    outputFile.close();

    cout << "Allocation written to the file successfully." << endl;

}

int random_num(int r) {
    return mt()%r;
}

long long partialcost(int zs,int ls,vector<int>&cs,int** T,int** N,int z){
    long long zlcost=0LL;
    for(int i=1;i<=z;i++){
        if(i==zs)continue;
        zlcost+=((long long)(N[i-1][zs-1]+N[zs-1][i-1]))*(long long)T[ls-1][cs[i-1]-1];
    }
    return zlcost;
}


void GHC_with_RRS(int* mapping,int &z,int &l,int** T,int** N,vector<int>&cs,int time,long long fcost,vector<pair<int,int>> &all_pairs){
    int p=all_pairs.size();
    int climb=0;
    while(p>0){
        bool changemap=false;
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> timeInterval = endTime - startTime;
        double secondsInterval = timeInterval.count();
        if(secondsInterval+ltime>=time*mult){
            break;
        }
        int index=random_num(p);
        int i=all_pairs[index].first;
        int j=all_pairs[index].second;
        long long preci=partialcost(i+1,cs[i],cs,T,N,z);
        if(j+1<=z){
            long long precj=partialcost(j+1,cs[j],cs,T,N,z);
            swap(cs[i],cs[j]);
            long long nextci=partialcost(i+1,cs[i],cs,T,N,z);
            long long nextcj=partialcost(j+1,cs[j],cs,T,N,z);
            if(nextci+nextcj-preci-precj<0){
                costmin+=nextci+nextcj-preci-precj;
                changemap=true;
                climb++;
            }else swap(cs[i],cs[j]);
            swap(all_pairs[index],all_pairs[p-1]);
            p--;
        }else{
            long long newci=partialcost(i+1,cs[j],cs,T,N,z);
            if(newci-preci<0){
                swap(cs[i],cs[j]);
                climb++;
                costmin+=newci-preci;
            }
            swap(all_pairs[index],all_pairs[p-1]);
            p--;
        }
        if(changemap){
            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> timeInterval = endTime - startTime;
            double secondsInterval = timeInterval.count();
            cout<<"costmin : "<<costmin<<" optimal : "<<fcost<<" time : "<<secondsInterval<<endl;
            p=all_pairs.size();
        }
    }

}

long long findcost(int &z,vector<int>&mapping,int** N,int**T){

    long long cost=0;

    for(int i=0;i<z;i++)
    {
        for(int j=0;j<z;j++) 
        {
            cost+=(long long)N[i][j]*(long long)T[mapping[i]-1][mapping[j]-1];
        }
    }

    return cost;
}

long long CostFunction(int**T,int** N,int z,int l,vector<int>&mapping){


    long long cost=0;

    for(int i=0;i<z;i++)
    {
        for(int j=0;j<z;j++) 
        {
            cost+=(long long)N[i][j]*(long long)T[mapping[i]-1][mapping[j]-1];
        }
    }

    return cost;
}

void findrandominit_state(vector<int>&init_state,int l){
    for(int i=0;i<l;i++)init_state[i]=i+1;
    random_device rd;
    mt19937 rng(rd());
    shuffle(init_state.begin(),init_state.end(),rng);
}

long long findstate(int**T,int** N,int z,int l,int shufflestate,vector<int>&init_state){
    vector<int> v(l);
    for(int i=0;i<l;i++)v[i]=i+1;
    long long f=(1e18);
    for(int i=0;i<shufflestate;i++){
        findrandominit_state(init_state,l);
        long long ans=CostFunction(T,N,z,l,init_state);
        if(f>ans){
            f=ans;
            swap(v,init_state);
        }
    }
    swap(v,init_state);
    return f;
}

void SportsLayout::compute_allocation()
{
    long long fcost=inf;
    vector<int> init_state(l);
    for(int i=1;i<=l;i++)init_state[i-1]=i;
    vector<pair<int,int>> all_pairs;
    for(int i=0;i<z;i++)for(int j=0;j<l;j++)if(i!=j)all_pairs.push_back({i,j});
    restart=1000000;
    shufflestate=100;
    for(int i=0;i<restart;i++){
        costmin=findstate(T,N,z,l,shufflestate,init_state);
        GHC_with_RRS(mapping,z,l,T,N,init_state,time,fcost,all_pairs);
        if(fcost>costmin){
            fcost=min(fcost,costmin);
            for(int i=0;i<z;i++)mapping[i]=init_state[i];
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> timeInterval = endTime - startTime;
        double secondsInterval = timeInterval.count();
        if(secondsInterval+ltime>=time*mult)break;
    }
}