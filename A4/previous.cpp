#include <bits/stdc++.h>

// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

private:
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning

public:
	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals)
	{
		Node_Name=name;
		nvalues=n;
		values=vals;
	}
	string get_name()
	{
		return Node_Name;
	}
	vector<int> get_children()
	{
		return Children;
	}
	vector<string> get_Parents()
	{
		return Parents;
	}
	vector<float> get_CPT()
	{
		return CPT;
	}
	int get_nvalues()
	{
		return nvalues;
	}
	vector<string> get_values()
	{
		return values;
	}
	void set_CPT(vector<float> new_CPT)
	{
		CPT.clear();
		CPT=new_CPT;
	}
    void set_Parents(vector<string> Parent_Nodes)
    {
        Parents.clear();
        Parents=Parent_Nodes;
    }

    int add_child(int new_child_index )
    {
        for(int i=0;i<Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }



};


class network{

	list <Graph_Node> Pres_Graph;

public:
	int addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
		return 0;
	}    
	int netSize()
	{
		return Pres_Graph.size();
	}

    int get_index(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return count;
            count++;
        }
        return -1;
    }
// get the node at nth index
    list<Graph_Node>::iterator get_nth_node(int n)
    {
       list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(count==n)
                return listIt;
            count++;
        }
        return listIt; 
    }
    //get the iterator of a node with a given name
    list<Graph_Node>::iterator search_node(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return listIt;
        }
            cout<<"node not found\n";
        return listIt;
    }
};

network read_network()
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile("alarm.bif"); 
  	string temp;
  	string name;
  	vector<string> values;
  	
    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
      		
      		
      		ss.str(line);
     		ss>>temp;
     		
     		
     		if(temp.compare("variable")==0)
     		{
                    
     				ss>>name;
     				getline (myfile,line);
                   
     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{
     					
     					ss2>>temp;
     					
     					
     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);
     					
     					ss2>>temp;
    				}
     				Graph_Node new_node(name,values.size(),values);
     				int pos=Alarm.addNode(new_node);

     				
     		}
     		else if(temp.compare("probability")==0)
     		{
                    
     				ss>>temp;
     				ss>>temp;
     				
                    list<Graph_Node>::iterator listIt;
                    list<Graph_Node>::iterator listIt1;
     				listIt=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0)
     				{
                        listIt1=Alarm.search_node(temp);
                        listIt1->add_child(index);
     					values.push_back(temp);
     					
     					ss>>temp;

    				}
                    listIt->set_Parents(values);
    				getline (myfile,line);
     				stringstream ss2;
                    
     				ss2.str(line);
     				ss2>> temp;
                    
     				ss2>> temp;
                    
     				vector<float> curr_CPT;
                    string::size_type sz;
     				while(temp.compare(";")!=0)
     				{
                        
     					curr_CPT.push_back(atof(temp.c_str()));
     					
     					ss2>>temp;
                       
    				}
                    
                    listIt->set_CPT(curr_CPT);


     		}
            else
            {
                
            }
    	}
    	
    	if(find==1)
    	myfile.close();
  	}
  	
  	return Alarm;
}

class Graph
{
public:
    vector<float> weights; //
    string filename; //
    map<string, int> dictionary; //
    string filename1; //
    vector<vector<float> > CPT; //
    float i_if = 1.0, i_jf = 0.0; //
    int totalVars; //
    vector<string> names; //
    time_t initTime; 
    vector<vector<string> > possibleValues; //
    vector<vector<int> > child; //
    vector<vector<int> > parents; //
    int i_i = 1, i_j = 0; //
    vector<int> questionMarks; //
    vector<vector<int> > realData; // 
    vector<vector<int> > data; //
    float smoothfact = 0.0035;
    int runtime = 5;
    Graph(string fnm, string file,time_t init)
    {
        dictionary.clear(); //
        questionMarks.resize(0); //
        initTime = init; //
        totalVars = 0; //
        child.resize(37); //
        parents.resize(37); //
        possibleValues.resize(37); //
        names.resize(37); //
        filename1=file; // data
        realData.resize(0); //
        weights.resize(0); //
        CPT.resize(37); //
        data.resize(0);
        filename = fnm; // bif
        bayesNetReader(); //
        dataFileReader(); //
        findInferencing(); //
    }

    void CPTupdater()
    {
        int updt=0;
        updt++;
        for(int i=0; i<CPT.size(); i++)
        {
            CPT[i] = vector<float>(CPT[i].size(), smoothfact); 
            vector<int> ids(0);
            vector<int> allSize(0); 
            ids.push_back(i);
            allSize.push_back(possibleValues[i].size());

            for(int j=0; j<parents[i].size(); j++)
            {
                allSize.push_back(possibleValues[parents[i][j]].size());
                ids.push_back(parents[i][j]);
            }
            
            int jump = (CPT[i].size()/possibleValues[i].size());
            vector<float> sumParts(jump, possibleValues[i].size() * smoothfact);
                //ith index table cpt[i]
            for(int j=0; j<data.size(); j++)
            {
                vector<int> values(0);
                int k=0;
                while(k<ids.size()) 
                {
                    values.push_back(data[j][ids[k]]); 
                    k++;
                }
                int data_index = idxCalc(values, allSize);
                CPT[i][data_index] += weights[j]; 
                sumParts[data_index%jump]+=weights[j];

            }

            for(int j=0; j<CPT[i].size(); j++)
            {
                if(sumParts[j%jump] == 0)
                    CPT[i][j] = i_jf;
                else
                    CPT[i][j] /= sumParts[j%jump];
            }
        }
    }

    void dataFileWriter(string outfile)
    {
        ifstream myfile(filename); 
        string temp_input;
        string line;
        ofstream out;
        string name;
        out.open(outfile);
        if (!myfile.is_open())
            return;
        else
        {
            while (! myfile.eof() )
            {
                stringstream ss;
                getline (myfile,line);
                ss.str(line);

                ss>>temp_input;
                if(temp_input.compare("probability")==0)
                {                        
                    ss>>temp_input;
                    ss>>temp_input;
                    int currentID = (dictionary.find(temp_input)->second);
                    out << line << endl;
                    getline (myfile,line);
                    out << "    table ";
                    int i=0;
                    while(i < CPT[currentID].size())
                    {
                        if(CPT[currentID][i]<0.0001)
                            out << "0.0001" << " ";
                        else
                            out << fixed << setprecision(4) << CPT[currentID][i] << " ";
                        i++;
                    }
                    out << ";" << endl;
                }
                else if(line.compare("")!=0)
                    out << line << endl;
                else 
                    out << line;
            }
            
            myfile.close();
        }
    }
    //
    void bayesNetReader()
    {
        string l;
        string name;
        ifstream myfile(filename); 
        string temp_input;
        
        
        if (! myfile.is_open())
            return;
        else
        {
            while (! myfile.eof() )
            {
                stringstream ss;
                getline (myfile,l);
                ss.str(l);
                ss>>temp_input;

                if(temp_input.compare("probability")==0)
                {                        
                    ss>>temp_input;
                    ss>>temp_input;
                    int currentID = dictionary.find(temp_input)->second;
                    ss>>temp_input;
                    parents[currentID].clear();
                    while(1)
                    {
                        if(temp_input.compare(")")==0)
                            break;
                        int parIndex = (dictionary.find(temp_input)->second);
                        child[parIndex].push_back(currentID);
                        parents[currentID].push_back(parIndex);
                        ss>>temp_input;
                    }
                    getline (myfile,l);
                    stringstream ss2;
                    ss2.str(l);
                    ss2>> temp_input;
                    ss2>> temp_input;
                    CPT[currentID].clear();
                    while(temp_input.compare(";")!=0)
                    {

                        CPT[currentID].push_back(atof(temp_input.c_str()));
                        ss2>>temp_input;
                    }                
                }   
            

                else if(temp_input.compare("variable")==0)
                {
                    ss>>name;
                    dictionary[name] = totalVars++;
                    names[totalVars-1] = name;
                    getline (myfile,l);
                    stringstream ss2;
                    ss2.str(l);
                    for(int i=0;i<4;i++)
                        ss2>>temp_input;
                    possibleValues[totalVars-1].clear();
                    while(temp_input.compare("};")!=0) 
                    {
                        possibleValues[totalVars-1].push_back(temp_input);
                        ss2>>temp_input;
                    }
                }
            }
            
            myfile.close();
        }
    }
    
     void dataFileWriter() 
    {
        string line;
        ifstream myfile(filename); 
        string temp_input;
        string name;
        if (! myfile.is_open())
            return;
        else
        {
            while (! myfile.eof() )
            {
                stringstream ss;
                getline (myfile,line);
                ss.str(line);
                ss>>temp_input;
                if(line.compare("")!=0)
                	cout << line << endl;
                else if(temp_input.compare("probability")==0)
                {                        
                    ss>>temp_input;
                    ss>>temp_input;
                    int currentID = (dictionary.find(temp_input)->second);
                    cout << line << endl;
                    getline (myfile,line);
                    cout << "    table ";
                    for(int i=0; i < CPT[currentID].size(); i++)
                        printf("%.4f ",CPT[currentID][i]);
                    cout << ";" << endl;
                }
                else 
                    cout << line;
            }
            
            myfile.close();
        }
    }
    //
    void dataFileReader()
    {
        string line;
        ifstream myfile(filename1); 
        string temp_input;
        string name;

        if (!myfile.is_open())
            return;
        else
        {
            while (! myfile.eof() )
            {
            stringstream ss;
            getline (myfile,line);
            ss.str(line);
            vector<int> values;
            int question_index = -1;
            for(int i=0; i<totalVars; i++)
            {
                ss >> temp_input;
                if(temp_input.compare("\"?\"") !=0)
                {

                }
                else
             	{
             		question_index = i;
             		values.push_back(-1);
             		continue;
             	}
                int data_index = 0;
                while(data_index < possibleValues[i].size())
                {
                	if(possibleValues[i][data_index].compare(temp_input) != 0)
                    {
                        data_index+=1;
                    }
                    else
                		break;
                }
                values.push_back(data_index);
            }
            questionMarks.push_back(question_index);
            realData.push_back(values);
            if(question_index!=-1)
            {
                int node_id = question_index;
                int s = possibleValues[node_id].size();
                int i=0;
                while(i<s)
                {
                    vector<int> temp_input(values.begin(), values.end());
                    temp_input[node_id] = i;
                    data.push_back(temp_input);
                    weights.push_back(i_if/s);
                    i++;
                }
            }
            else
            {
                data.push_back(values);
                weights.push_back(1);
            }
        }
        myfile.close();
    }
}

    //
    void findInferencing()
    {
        int iter_count = 0;
        CPTinitializer(); // not of use
        dataUpdater();
        while(time(NULL) - initTime < runtime)
        {
            iter_count+=1;
        	CPTupdater();
            dataUpdater();
        }
        dataFileWriter("solved_alarm.bif");
    }
    
    //
    float probabilityCalc(int lno, int var, int val)
    {
        vector<int> allSize;
        vector<int> values;
        values.push_back(val);
        allSize.push_back(possibleValues[var].size());
        int j=0;
        while(j<parents[var].size()) 
        {
            allSize.push_back(possibleValues[parents[var][j]].size());
            values.push_back(realData[lno][parents[var][j]]);
            j++;
        }

        return CPT[var][idxCalc(values, allSize)];
    }

    //
    void dataUpdater()
    {
        weights.clear();
        for(int i=0; i<realData.size(); i++)
        {

            if(questionMarks[i] != -1)
            {
                int var = questionMarks[i];
                vector<float> probs(0);
                float probs_sum = i_jf;
                int j=0;
                while(j<possibleValues[var].size())
                {
                    float prob = probabilityCalc(i, var, j);
                    for(int k = 0; k<child[var].size(); k++)
                    {
                        realData[i][var] = j;
                        int t1 = child[var][k];
                        float t2 = probabilityCalc(i,t1,realData[i][t1]);
                        prob*=t2;
                    }
                    probs_sum += prob;
                    probs.push_back(prob);
                    j++;
                }
                int max_index = max_element(probs.begin(), probs.end()) - probs.begin();
                realData[i][var] = max_index ;

                float alpha = (i_jf+i_if)*(i_if/probs_sum);
                for(int j=0; j<probs.size(); j++)
                    weights.push_back(alpha*probs[j]);
            }
            else
            {
                weights.push_back(1);
                continue;
            }
            
        }
    }

    //
    void CPTinitializer()
    {
        int i=0;
        while(i<CPT.size())
        {
            int len = (CPT[i].size()/possibleValues[i].size());
            int slots = possibleValues[i].size();
            vector<float> sums(len,1.0);
            vector<int> noni_jcount(len,0);


            for(int j=0; j<CPT[i].size(); j++)
            {
                if(CPT[i][j] >= 0)
                    continue;
                else
                    CPT[i][j] = sums[j%len]/(slots - noni_jcount[j%len]);
            }
            i++;
        }
    }

    //
    int idxCalc(vector<int>& values, vector<int>& allSize)
    {
    	int ans =0,x=1;
    	for(int j=allSize.size()-1; j>=0; j--)
    	{
    		ans += x*values[j];
    		x*=allSize[j];
    	}
    	return ans;
    }	

};

int main(int argc, char** argv)
{
	srand(time(NULL));
    Graph* G = new Graph(argv[1],argv[2], time(NULL));
    return 0;
}