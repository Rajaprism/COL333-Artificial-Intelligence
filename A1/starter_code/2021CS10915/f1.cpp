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
double ltime=1.0;
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



void GHC_with_RRS(int* mapping,int &z,int &l,int** T,int** N,vector<int>&cs,int time,long long fcost){
    bool changemap=false;
    while(true){
        bool visited[z][l];
        for (int i=0;i<z;i++)for(int j=0;j<l;j++)visited[i][j]=false;
        bool isvalid_neigh=false;
        for(int k=0;k<iterations;k++){
            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> timeInterval = endTime - startTime;
            double secondsInterval = timeInterval.count();
            // cout<<" time : "<<secondsInterval<<endl;
            if(secondsInterval+ltime>=time*mult){
                break;
            }
            int i=random_num(z);
            int j=random_num(l);
            while(i==j)j=random_num(l);
            if(visited[i][j])continue;
            visited[i][j]=true;
            long long preci=partialcost(i+1,cs[i],cs,T,N,z);
            if(j+1<=z){
                long long precj=partialcost(j+1,cs[j],cs,T,N,z);
                swap(cs[i],cs[j]);
                long long nextci=partialcost(i+1,cs[i],cs,T,N,z);
                long long nextcj=partialcost(j+1,cs[j],cs,T,N,z);
                if(nextci+nextcj-preci-precj<0){
                    isvalid_neigh=true;
                    costmin+=nextci+nextcj-preci-precj;
                    changemap=true;
                }else swap(cs[i],cs[j]);
            }else{
                long long newci=partialcost(i+1,cs[j],cs,T,N,z);
                if(newci-preci<0){
                    swap(cs[i],cs[j]);
                    isvalid_neigh=true;
                    costmin+=newci-preci;
                    changemap=true;
                }
            }
            if(isvalid_neigh){
                break;
            }
        }
        climb++;
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> timeInterval = endTime - startTime;
        double secondsInterval = timeInterval.count();
        cout<<"iterations : "<<climb<<", cost : "<<costmin<<", optimal : "<<fcost<<", time : "<<secondsInterval<<endl;
        if(!isvalid_neigh){
            break;
        }
    }
}


void GHC_with_RRS(int* mapping,int &z,int &l,int** T,int** N,vector<int>&cs,int time,long long fcost,vector<pair<int,int>> &all_pairs){
    bool changemap=false;
    int p=z*l;
    while(p>0){
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
                swap(all_pairs[index],all_pairs[p-1]);
                p--;
            }else swap(cs[i],cs[j]);
        }else{
            long long newci=partialcost(i+1,cs[j],cs,T,N,z);
            if(newci-preci<0){
                swap(cs[i],cs[j]);
                swap(all_pairs[index],all_pairs[p-1]);
                p--;
                costmin+=newci-preci;
            }
        }
    }
    // while(true){
    //     bool visited[z][l];
    //     for (int i=0;i<z;i++)for(int j=0;j<l;j++)visited[i][j]=false;
    //     bool isvalid_neigh=false;
    //     for(int k=0;k<iterations;k++){
    //         auto endTime = std::chrono::high_resolution_clock::now();
    //         std::chrono::duration<double> timeInterval = endTime - startTime;
    //         double secondsInterval = timeInterval.count();
    //         // cout<<" time : "<<secondsInterval<<endl;
    //         if(secondsInterval+ltime>=time*mult){
    //             break;
    //         }
    //         int i=random_num(z);
    //         int j=random_num(l);
    //         while(i==j)j=random_num(l);
    //         if(visited[i][j])continue;
    //         visited[i][j]=true;
    //         long long preci=partialcost(i+1,cs[i],cs,T,N,z);
    //         if(j+1<=z){
    //             long long precj=partialcost(j+1,cs[j],cs,T,N,z);
    //             swap(cs[i],cs[j]);
    //             long long nextci=partialcost(i+1,cs[i],cs,T,N,z);
    //             long long nextcj=partialcost(j+1,cs[j],cs,T,N,z);
    //             if(nextci+nextcj-preci-precj<0){
    //                 isvalid_neigh=true;
    //                 costmin+=nextci+nextcj-preci-precj;
    //                 changemap=true;
    //             }else swap(cs[i],cs[j]);
    //         }else{
    //             long long newci=partialcost(i+1,cs[j],cs,T,N,z);
    //             if(newci-preci<0){
    //                 swap(cs[i],cs[j]);
    //                 isvalid_neigh=true;
    //                 costmin+=newci-preci;
    //                 changemap=true;
    //             }
    //         }
    //         if(isvalid_neigh){
    //             break;
    //         }
    //     }
    //     climb++;
    //     // auto endTime = std::chrono::high_resolution_clock::now();
    //     // std::chrono::duration<double> timeInterval = endTime - startTime;
    //     // double secondsInterval = timeInterval.count();
    //     // cout<<"iterations : "<<climb<<", cost : "<<costmin<<", optimal : "<<fcost<<", time : "<<secondsInterval<<endl;
    //     if(!isvalid_neigh){
    //         break;
    //     }
    // }
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
void findrandominit_state(vector<int>&init_state,int l){
    for(int i=0;i<l;i++)init_state[i]=i+1;
    random_device rd;
    mt19937 rng(rd());
    shuffle(init_state.begin(),init_state.end(),rng);
}

void SportsLayout::compute_allocation()
{
    long long fcost=inf;
    vector<int> init_state(l);
    for(int i=1;i<=l;i++)init_state[i-1]=i;
    vector<pair<int,int>> all_pairs;
    for(int i=1;i<=z;i++)for(int j=1;j<=l;j++)if(i!=j)all_pairs.push_back({i,j});
    restart=1000000;
    for(int i=0;i<restart;i++){
        findrandominit_state(init_state,l);
        costmin=findcost(z,init_state,N,T);
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

    // iterations=100000000;
    // restart=1000000;
    // for(int i=0;i<restart;i++){
    //     climb=0;
    //     findrandominit_state(init_state,l);
    //     costmin=findcost(z,init_state,N,T);
    //     GHC_with_RRS(mapping,z,l,T,N,init_state,time,fcost);
    //     if(fcost>costmin){
    //         fcost=min(fcost,costmin);
    //         for(int i=0;i<z;i++)mapping[i]=init_state[i];
    //     }
    //     auto endTime = std::chrono::high_resolution_clock::now();
    //     std::chrono::duration<double> timeInterval = endTime - startTime;
    //     double secondsInterval = timeInterval.count();
    //     if(secondsInterval+ltime>=time*mult)break;
    //     cout<<"climbs "<<climb<<" : restarts -> "<<i+1<<" finalcost "<<fcost<<" timetaken : "<<secondsInterval<<"\n";
    // }
}