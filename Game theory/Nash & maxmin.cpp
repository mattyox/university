#include <iostream>
#include <vector>
#include <algorithm>
#include <conio.h>
using namespace std;

class Nash
{
    vector<vector<double>> gameA;   //vector storing matrix A
    vector<vector<double>> gameB;   //vector storing matrix B
    vector<vector<int>> equilibria; //vector storing "coordinates" of Nash equilibria
public:
    Nash(vector<vector<double>> A, vector<vector<double>> B): gameA(A), gameB(B) {}

    void find()
    {
        //temporary variables used to store various information
        double m;
        vector<double> temp;
        //vectors containing position of best outcomes in strategies for matrices A and B
        vector<double> outcomesA;
        vector<double> outcomesB;

        int a;  //integer storing number of most profitable equilibrium in matrix A
        int b; //integer storing number of most profitable equilibrium in matrix B

        //find best outcomes in strategies
        for(size_t i = 0; i < gameA[0].size(); i++)
        {
            for(size_t j = 0; j < gameA.size(); j++)
            {
                temp.push_back(gameA[j][i]);
            }
            m = *max_element(temp.begin(), temp.end());
            for(size_t k=0; k < temp.size(); k++)
            {
                if(temp[k]==m)
                    outcomesA.push_back(k);
            }
            temp.clear();
        }

        for(size_t i = 0; i < gameB.size(); i++)
        {
            m = *max_element(gameB[i].begin(), gameB[i].end());
            for(size_t k=0; k < gameB[i].size(); k++)
            {
                if(gameB[i][k] == m)
                    outcomesB.push_back(k);
            }
        }
        //if best outcomes are on the same position in both matrices, save their position as Nash equilibrium
        for(size_t i = 0; i < outcomesA.size(); i++)
        {
            if(outcomesA[i] == outcomesB[i])
            {
                equilibria.push_back(vector<int>());
                equilibria.back().push_back(outcomesA[i]);
                equilibria.back().push_back(outcomesB[i]);
            }

        }

        //if more than one Nash equilibrium has been found
        if(equilibria.size()>1)
        {
            //compare equilibria values in matrix A and find most profitable

            double n = gameA[equilibria[0][0]][equilibria[0][1]];   //first equilibrium value in matrix A
            temp.push_back(0);
            for(size_t i = 1; i < equilibria.size(); i++)
            {
                m = max(gameA[equilibria[i][0]][equilibria[i][1]], gameA[equilibria[i-1][0]][equilibria[i-1][1]]);
                if(m>n)
                {
                    a = i;
                    m = n;
                }
            }
            //compare equilibria values in matrix B and find most profitable
            n = gameB[equilibria[0][0]][equilibria[0][1]]; //first equilibrium value in matrix B

            for(size_t i = 1; i < equilibria.size(); i++)
            {
                m = max(gameB[equilibria[i][0]][equilibria[i][1]], gameB[equilibria[i-1][0]][equilibria[i-1][1]]);
                if(m>n)
                {
                    b = i;
                    m = n;
                }
            }
            //if there exists most profitable equilibrium in both matrices A and B
            if(a == b)
                cout<<"Feasible Nash equilibrium found for strategies ("<<equilibria[a][0]+1<<", "<<equilibria[a][1]+1
                    <<") equal to ("<<gameA[equilibria[a][0]][equilibria[a][1]]<<", "<<gameB[equilibria[a][0]][equilibria[a][1]]<<")."
                    <<endl;
            else
                cout<<"No feasible Nash equilibrium found."<<endl;
        }
        else //if there is only one equilibrium found
            cout<<"Nash equilibrium found for strategies ("
                <<equilibria[0][0]+1<<", "<<equilibria[0][1]+1<<") equal to ("
                <<gameA[equilibria[0][0]][equilibria[0][1]]<<", "<<gameB[equilibria[0][0]][equilibria[0][1]]<<")."<<endl;
    }

};

class Maxmin
{
    vector<vector<double>> gameA;   //vector storing matrix A
    vector<vector<double>> gameB;   //vector storing matrix B
    double maxminA;     //maxmin solution of A
    double maxminB;     //maxmin solution of B

public:
    Maxmin(vector<vector<double>> A, vector<vector<double>> B): gameA(A), gameB(B) {}

    void find()
    {
        //temporary variables used to store various information
        double m;
        vector<double> temp;

        //vectors containing position of best outcomes in strategies for matrices A and B
        vector<double> outcomesA;
        vector<double> outcomesB;

        //find worst outcome for each strategy for game A
        for(size_t i = 0; i < gameA[0].size(); i++)
        {
            for(size_t j = 0; j < gameA.size(); j++)
            {
                temp.push_back(gameA[i][j]);
            }
            m = *min_element(temp.begin(), temp.end());
            for(size_t k = 0; k < temp.size(); k++)
            {
                if(temp[k] == m)
                    outcomesA.push_back(k);
            }
            temp.clear();
        }


        //find worst outcome for each strategy for game B
        for(size_t i = 0; i < gameB.size(); i++)
        {
            m = gameB.front().at(i);
            outcomesB.push_back(0);
            for(size_t j = 0; j < gameB[i].size(); j++)
            {
                if(gameB[j][i] < m)
                {
                    m = gameB[j][i];
                }
                if(gameB[j][i] == m)
                    outcomesB[i] = gameB[j][i];
            }
        }

        //out of worst outcomes find maximum profit for game A
        m = outcomesA[0];
        for(size_t i = 0; i < outcomesA.size(); i++)
        {
            if(outcomesA[i] > m)
                m = outcomesA[i];

            if(outcomesA[i] == m)
                maxminA = i;
        }

        //out of worst outcomes find maximum profit for game B
        m = outcomesB[0];
        for(size_t i = 0; i < outcomesB.size(); i++)
        {
            if(outcomesB[i] > m)
                m = outcomesB[i];

            if(outcomesB[i] == m)
                maxminB = i;
        }

        //find safety level for maxmin of B
        double safetyB;
        m = gameB[0][maxminB];
        for(size_t i = 0; i < gameB.size(); i++)
        {
            if(gameB[i][maxminB] < m)
                m = gameB[i][maxminB];

            if(gameB[i][maxminB] == m)
                safetyB = m;
        }
        //print solutions
        cout<<"Maxmin for D1: strategy nr "<<maxminA+1<<" (safety level: "<<*min_element(gameA[maxminA].begin(), gameA[maxminA].end())<<")."<<endl;
        cout<<"Maxmin for D2: strategy nr "<<maxminB+1<<" (safety level: "<<safetyB<<")."<<endl;
    }
};

int main()
{
    vector<vector<double>> gameA = {{4, 3, 2}, {-2, 5, 1}, {3, -10, 0}};	//here matrices can be changed
    vector<vector<double>> gameB = {{6, -1, 4},{-3, 7, -2},{2, -1, 3}};

    //print the game matrices
    cout<<"   A:"<<endl;
    for(size_t i=0; i<gameA.size(); i++)
    {
        for(size_t j=0; j<gameA[i].size(); j++)
            cout<<gameA[i][j]<<"\t";
        cout<<endl;
    }
    cout<<endl;
    cout<<"   B:"<<endl;
    for(size_t i=0; i<gameB.size(); i++)
    {
        for(size_t j=0; j<gameB[i].size(); j++)
            cout<<gameB[i][j]<<"\t";
        cout<<endl;
    }
    cout<<endl;

    Nash n(gameA, gameB);
    n.find();

    cout<<endl;
    Maxmin m(gameA, gameB);
    m.find();

    cout<<"\nPress any key to exit."<<endl;
    _getch();
    return 0;
}
