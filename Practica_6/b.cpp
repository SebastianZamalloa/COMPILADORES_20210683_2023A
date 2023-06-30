#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

string trans;

template<typename T>
bool isThere(vector<T> v, T key)
{
    if (find(v.begin(), v.end(), key) != v.end())
        return true;
    return false;
}

template<typename T>
int searchKey(vector<T> v, T key)
{
    int look = 0;
    for(auto i:v)
    {
        if(key == i)
            return look;
        look++;
    }
    return -1;
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

vector<int> getEntry(string name)
{
    vector<int> S0;
    ifstream reader(name);
    string line;
    do getline(reader,line); while (line != "Entradas");
    getline(reader,line); int q = stoi(line); 
    getline(reader,line);
    istringstream transitions(line);
    string trans;
    for(int i = 0; i<q; i++)
    {
        transitions>>trans;
        S0.push_back(stoi(trans));
    }return S0;
}

vector<int> mueve(vector<int> U, int value, string name)
{
    vector<int>mov;
    while(U.size() != 0)
    {   
        ifstream reader(name);
        string line;
        int r = U[U.size()-1]; U.pop_back();
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
                if(stoi(trans) == value)
                {
                    transitions>>trans;
                    int s = stoi(trans);
                    if(!isThere(mov,s))
                        mov.push_back(s);
                }
            }
        }
        reader.close();
    }
    return mov;
}

bool isAllTrue(vector<bool>m)
{
    for(auto i:m)
        if(!i)
            return false;
    return true;
}

void D_tran(int index, int next, int val, bool final = false)
{
    //Transiciones_(x,y,z)_epsilon=-1
    trans += to_string(index) + " " + to_string(val) + " " + to_string(next);
    if(!final)
        trans += '\n';
}

int main()
{

    string ent;
    string outer;

    cout<<"Ingrese el nombre del AFDN: ",cin>>ent;
    cout<<"Ingrese el nombre del AFD: ",cin>>outer;

    vector<int> entries = getEntry(ent);
    vector<int> A = E_clausura(ent,{0});
    vector<vector<int>> D_est = {A};
    vector<bool> marked = {0};

    int index = 0;
    while(!isAllTrue(marked))
    {
        marked[index] = 1;
        for(auto j:entries)
        {
            vector<int> U = E_clausura(ent,mueve(D_est[index],j,ent));
            if(!isThere(D_est,U))
            {
                D_est.push_back(U);
                marked.push_back(0);
            }
            D_tran(index,searchKey(D_est,U),j);  
        }
        index++;
    }
    trans = "\nTransiciones_(x,y,z)\n" + trans;
    {
        string e = "\n";
        for(int i = 0; i<entries.size();i++)
        {
            e += to_string(entries[i]);
            if(i+1 != entries.size())
                e += " ";
        }
        trans = "\nEntradas\n" + to_string(entries.size()) + e + trans;
    }
    trans = "Estados_de_Aceptacion\n1\n" + to_string(D_est.size()-1) + trans;
    {
        string e = "Estados\n";
        e += to_string(D_est.size()) + "\n";
        for(int i = 0; i<D_est.size(); i++)
        {
            e += to_string(i) + " = { ";
            for(auto j:D_est[i])
                e += to_string(j) + " ";
            e += "}\n";
        }
        trans = e + trans;
    }
    ofstream writer;
    writer.open(outer.c_str(), fstream::app);
    writer << trans;
    writer.close();
}
