#include<bits/stdc++.h>
#include <random>

#define en cout<<"\n";
// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

// // Our graph consists of a list of nodes where each node is represented as follows:
auto start = std::chrono::high_resolution_clock::now();

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
	int ct=0;

	public:
	BayesNet(string bayesfile,string datafile,float t){
		Parameter_values.resize(37);
		Parent.resize(37);
		Child.resize(37);
		Cond_Pro_Table.resize(37);

		ReadBayes(bayesfile);

		DataReader(datafile);

		for(int i=0;i<Cond_Pro_Table.size();i++){

			int sz=Parameter_values[i].size();

			for(int j=0;j<Cond_Pro_Table[i].size();j++){
				Cond_Pro_Table[i][j]=1.0/sz;
			}
		}
		ct=0;

		Inferencing(t);
		WriteTofile("solved_alarm.bif",bayesfile);
		cout<<" no of iterations: "<<ct<<endl;
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
				ct++;
				DataPoints.push_back(Sample);
				weights.push_back(1.0);
				Questions.push_back(-1);
			}

		}
	}

	void Inferencing(float t){
		auto interval=0.0;
		while(true){
			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			if(((duration.count())/1e6)+0.002+interval>=t)break;
			auto st=std::chrono::high_resolution_clock::now();
			Expectation();
			update_cpt();
			auto et=std::chrono::high_resolution_clock::now();
			if(ct==0){
				auto  it= std::chrono::duration_cast<std::chrono::microseconds>(et - st);
				interval=(it.count())/1e6;

			}
			ct++;
		}
		
	}

	void update_cpt(){
		for(int var=0;var<Cond_Pro_Table.size();var++){

			int pval_sz=Parameter_values[var].size();
			int p_sz=Cond_Pro_Table[var].size()/pval_sz;

			vector<float> p_sum(p_sz,0.0035);
			vector<float> pc(p_sz*pval_sz,0.00);

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

				Cond_Pro_Table[var][j]=max(pc[j]/p_sum[j%p_sz],(float)0.01);

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
			vector<int> ss=DataPoints[i];
			float cp=calcProb(ss,index,DataPoints[i][index]);

			for(int cvar : Child[index] ){
				cp*=calcProb(ss,cvar,DataPoints[i][cvar]);
			}
			weights[i]=cp;
		}
	}
	
	void WriteTofile(string filename,string file){
		ofstream outfile(filename);
		ifstream inpfile(file);
		getline(inpfile,line);
		while(line[0]!='p'){
			outfile<<line<<endl;
			getline(inpfile,line);
		}
		int currID=0;
		while(!inpfile.eof() && currID<37){
			outfile<<line<<endl;
			getline(inpfile,line);
			string s="	table ";

			for(float cp : Cond_Pro_Table[currID] ){
				std::stringstream ss;
				ss << std::fixed << std::setprecision(4) << cp;
				s+=ss.str();
				s+=" ";
			}
			s+=";";

			outfile<<s<<endl;
			getline(inpfile,line);
			outfile<<line<<endl;
			currID++;
			getline(inpfile,line);

		}
	}
	
};

int main(int argc, char* argv[])
{
	BayesNet*it=new BayesNet(argv[1],argv[2],120);
	free(it);
	// Example: to do something
	cout<<"Perfect! Hurrah!\n";
	
}




