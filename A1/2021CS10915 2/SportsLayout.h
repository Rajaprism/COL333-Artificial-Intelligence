
#ifndef SPORTSLAYOUT_H
#define	SPORTSLAYOUT_H

#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

class SportsLayout{

    private:
    int z,l;
    int** T;
    int** N;
    double time;
    int* mapping;

    public:
    SportsLayout(string inputfilename);

    bool check_output_format();

    // void readOutputFile(string output_filename);
    
    long long cost_fn();
    void write_to_file(string outputfilename);

    void readInInputFile(string inputfilename);

    void compute_allocation();

};


#endif