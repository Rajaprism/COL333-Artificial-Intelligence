#include<bits/stdc++.h>
#include "debuggers.hpp"

#define en cout<<"\n";
// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

// // Our graph consists of a list of nodes where each node is represented as follows:


class BayesNet{

	vector<int> Questions;
	vector<vector<int>> DataPoints;
	vector<float> weights;
	
	vector<vector<int>> Child; //
	vector<vector<int>> Parent; //

	vector<vector<float>> Cond_Pro_Table; //

	map<string,int> Map_name_index; //
	vector<string> Parameter;  //
	vector<vector<string>> Parameter_values; //
	int Index=0;

	string name,line,temp;

	public:
	BayesNet(string bayesfile,string datafile){
		Parameter_values.resize(37);
		Parent.resize(37);
		Child.resize(37);
		Cond_Pro_Table.resize(37);

		ReadBayes(bayesfile);
		DataReader(datafile);

		for(int i=0;i<Cond_Pro_Table.size();i++){

			int sz=Parameter_values[i].size();

			for(int j=0;j<Cond_Pro_Table[i].size();j++){
				if(Cond_Pro_Table[i][j]!=-1)continue;
				Cond_Pro_Table[i][j]=1.0/sz;
			}
		}

		Aastha();

		for(int i=0;i<Cond_Pro_Table.size();i++){
			for(float j: Cond_Pro_Table[i])cout<<j<<" ";
			en;
		}

	}

	void ReadBayes(string filename){

		ifstream myfile(filename);
		int CurrId;

		while (! myfile.eof())
    	{
    		stringstream ss;
      		getline (myfile,line);
      		
      		ss.str(line);
     		ss>>temp;
     		
     		if(temp.compare("variable")==0)
     		{
                    
     				ss>>name;

					string nv;
					ss>>nv;ss>>nv;
					Map_name_index[name]=Index;
					CurrId=Index;
					Index++;

					Parameter.push_back(name);

     				getline (myfile,line);
                   
     				stringstream ss2;
     				ss2.str(line);

     				for(int i=0;i<4;i++)ss2>>temp;

     				while(temp.compare("};")!=0)
     				{
     					Parameter_values[CurrId].push_back(temp);
     					ss2>>temp;
    				}
     				
     		}
     		else if(temp.compare("probability")==0)
     		{

     				ss>>temp; 
     				ss>>temp;

					CurrId=Map_name_index[temp];
     				
                    ss>>temp;
					int index;
     				while(temp.compare(")")!=0)
     				{
                        index=Map_name_index[temp];
						Child[index].push_back(CurrId);
						Parent[CurrId].push_back(index);

     					ss>>temp;

    				}
    				getline (myfile,line);
     				stringstream ss2;
                    
     				ss2.str(line);
     				ss2>> temp;
                    
     				ss2>> temp;
                    
     				while(temp.compare(";")!=0)
     				{
                        Cond_Pro_Table[CurrId].push_back(atof(temp.c_str()));
     					ss2>>temp;
    				}

     		}

    	}
	}

	void DataReader(string data_filename){

		ifstream inputFile(data_filename);
		string currline;
		string token;


		while(!inputFile.eof()){

			getline(inputFile,currline);
			istringstream iss(currline);
			vector<int> Sample(37);
			int i=0;
			iss>>temp;
			int qindex=-1;

			while(i<37){

				if(temp=="\"?\""){

					Sample[i]=-1;
					qindex=i;
				}
				else{
					int index=0;
					for(auto pval : Parameter_values[i])
					if(pval!=temp)index++;
					else{
						Sample[i]=index;
						break;
					}
				}
				i++;
				iss >> temp;
			}

			if(qindex !=-1){
				float wt=(1.0)/Parameter_values[i].size();
				for(int k=0;k<Parameter_values[qindex].size();k++){
					Sample[qindex]=k;
					
					DataPoints.push_back(Sample);
					weights.push_back(wt);
					Questions.push_back(qindex);
				}
			}else{
				DataPoints.push_back(Sample);
				weights.push_back(1.0);
				Questions.push_back(-1);
			}

		}
	}

	void Aastha(){
		int k=0;
		while(k<100){
			Expectation();
			update_cpt();
			k++;
		}
		
	}

	void update_cpt(){
		for(int var=0;var<Cond_Pro_Table.size();var++){

			int pval_sz=Parameter_values[var].size();
			int p_sz=Cond_Pro_Table[var].size()/pval_sz;

			vector<float> p_sum(p_sz,1.0);
			vector<float> pc(p_sz*pval_sz,0.0);

			for(int j=0;j<Questions.size();j++){
				int indx=0;
				int sz=1;

				for(int k=Parent[var].size()-1;k>=0;k--){

					int p=Parent[var][k];
					indx+=DataPoints[j][p]*sz;
					sz*=Parameter_values[p].size();

				}

				p_sum[indx]+=weights[j];

				indx+=DataPoints[j][var]*sz;

				pc[indx]+=weights[j];

			}

			for(int j=0;j<Cond_Pro_Table[var].size();j++){

				Cond_Pro_Table[var][j]=pc[j]/p_sum[j%p_sz];

			}
		}
	}

	float calcProb(vector<int> &sample,int var,int val){
		int indx=0;
		int sz=1;
		for(int i=Parent[var].size()-1;i>=0;i--){
			int p=Parent[var][i];
			indx+=sample[p]*sz;
			sz*=Parameter_values[p].size();
		}
		indx+=val*sz;
		return Cond_Pro_Table[var][indx];
	}

	void Expectation(){
		for(int i=0;i<Questions.size();i++){
			if(Questions[i]==-1)continue;

			int index=Questions[i];
			int pvalues=Parameter_values[index].size();
			float s=0.0;
			vector<int> ss=DataPoints[i];
			float cp=calcProb(ss,index,DataPoints[i][index]);
			for(int cvar : Child[index] ){
				cp*=calcProb(ss,cvar,DataPoints[i][cvar]);
			}
			weights[i]=cp;
		}
	}

};



int main()
{
	BayesNet*it=new BayesNet("alarm.bif","records.txt");
	// free(it);
	// Example: to do something
	cout<<"Perfect! Hurrah! \n";
	
}





