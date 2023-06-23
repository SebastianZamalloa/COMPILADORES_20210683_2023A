#include<iostream>
#include<vector>
#include<string>
using namespace std;

void error()
{
    cout<<"\nERROR\n";
    throw runtime_error("Error");
}

int main()
{
    string entry;
    cout<<"Ingrese: ",getline(cin,entry);
    vector<int>pila;
    for(int i = 0; i<entry.size(); i++)
    {
        if(entry[i] == '+')
        {
            if(pila.size()<2)
                error();
            int c3 = pila[pila.size()-2] + pila[pila.size()-1];
            pila.pop_back();pila.pop_back();
            pila.push_back(c3);
        }
        else if(entry[i] == '-')
        {
            if(pila.size()<2)
                error();
            int c3 = pila[pila.size()-2] - pila[pila.size()-1];
            pila.pop_back();pila.pop_back();
            pila.push_back(c3);
        }
        else if(entry[i] == '*')
        {
            if(pila.size()<2)
                error();
            int c3 = pila[pila.size()-2] * pila[pila.size()-1];
            pila.pop_back();pila.pop_back();
            pila.push_back(c3);
        }
        else if(entry[i] == '/')
        {
            if(pila.size()<2)
                error();
            int c3 = pila[pila.size()-2] / pila[pila.size()-1];
            pila.pop_back();pila.pop_back();
            pila.push_back(c3);
        }
        else if(isdigit(entry[i]))
        {
            string num;
            num += entry[i];
            while(isdigit(entry[i+1]) && entry[i+1] != ' ')
            {
                i++;
                num += entry[i];
            }
            pila.push_back(stoi(num));
        }
        else if(entry[i] != ' ')
            error();
            
    }
    if(pila.size() > 1 || pila.size() <= 0)
        error();
    cout<<"\nRESULTADO: "<<pila[0];
}
