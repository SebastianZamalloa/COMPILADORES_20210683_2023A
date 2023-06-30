#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

bool isThere(vector<int> v, int key)
{
    if (find(v.begin(), v.end(), key) != v.end())
        return true;
    return false;
}

vector<int> E_clausura(string name, vector<int> R)
{
    vector<int>E_c = R;
    while(R.size() != 0)
    {   
        ifstream reader(name);
        string line;
        int r = R[R.size()-1]; R.pop_back();
        do getline(reader,line); while (line != "Transiciones_(x,y,z)_epsilon=-1");
        getline(reader,line); int q = stoi(line); 
        for(int i = 0; i<q; i++)
        {
            getline(reader,line);
            istringstream transitions(line);
            string trans;
            transitions>>trans;
            if(stoi(trans) == r)
            {
                transitions>>trans;
                if(stoi(trans) == -1)
                {
                    transitions>>trans;
                    int s = stoi(trans);
                    if(!isThere(E_c,s))
                    {
                        E_c.push_back(s);
                        R.push_back(s);
                    }
                }
            }
        }
        reader.close();
    }
    return E_c;
}

int main()
{
    vector<int> R = {0,1};
    vector<int> E = E_clausura("entry.txt",R);
    for(auto i:E)
        cout<<i<<endl;
}
