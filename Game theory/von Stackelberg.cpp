#include <vector>
#include <iostream>
#include <conio.h>
using namespace std;

class Stackelberg
{
    vector<vector<double>> matrixA;
    vector<vector<double>> matrixB;
    vector<vector<int>> equilibria;

public:
    Stackelberg(vector<vector<double>> A, vector<vector<double>> B): matrixA(A), matrixB(B) {}

    void find()
    {
        vector<vector<double>> responses;   //vector storing response strategies from follower
        vector<double> outcomes;            //vector storing outcomes for leader based on follower's response
        //variables used to store various data
        double max;
        double min;

        //find responses for follower
        for(size_t i = 0; i < matrixB.size(); i++)
        {
            max = matrixB[i][0];
            responses.push_back(vector<double>());
            for(size_t j = 0; j < matrixB[i].size(); j++)
            {
                if(matrixB[i][j] > max)
                {
                    responses[i].clear();
                    max = matrixB[i][j];
                }

                if(matrixB[i][j] == max)
                    responses[i].push_back(j);
            }
        }

        //find outcomes for leader depending on responses from follower
        for(size_t i = 0; i < responses.size(); i++)
        {
            //if there is only one response, find outcome accordingly
            if(responses[i].size() == 1)
                outcomes.push_back(responses[i][0]);
            else    //if there are more than one responses
            {
                //find minimal outcome
                min = matrixA[i][responses[i][0]];
                outcomes.push_back(0);
                for(size_t j = 0; j < responses[i].size(); j++)
                {
                    if(min > matrixA[i][responses[i][j]])
                        min = matrixA[i][responses[i][j]];

                    if(matrixA[i][responses[i][j]] == min)
                        outcomes.back() = min;
                }
            }
        }

        //find maximum profit for leader
        max = matrixA[0][outcomes[0]];
        for(size_t i = 0; i < outcomes.size(); i++)
        {
            if(max < matrixA[i][outcomes[i]])
            {
                equilibria.clear();
                max = matrixA[i][outcomes[i]];
            }


            if(matrixA[i][outcomes[i]] == max)
            {
                equilibria.push_back(vector<int>());
                equilibria[i].push_back(i);
                equilibria[i].push_back(outcomes[i]);
            }
        }

        //print solution
        if(!equilibria.empty())
        {
            if(equilibria.size() == 1)
                cout<<"Von Stackelberg equilibrium found for strategies ("<<equilibria[0][0]+1<<", "<<equilibria[0][1]+1
                <<") with outcomes ("<<matrixA[equilibria[0][0]][equilibria[0][1]]<<", "
                <<matrixB[equilibria[0][0]][equilibria[0][1]]<<")."<<endl;
            else
            {
                for(size_t i = 0; i < equilibria.size(); i++)
                    cout<<"Von Stackelberg equilibrium found for strategies ("<<equilibria[i][0]+1<<", "<<equilibria[i][1]+1
                    <<") with outcomes ("<<matrixA[equilibria[i][0]][equilibria[i][1]]<<", "
                    <<matrixB[equilibria[i][0]][equilibria[i][1]]<<")."<<endl;
            }
        }
        else
            cout<<"No von Stackelberg equilibrium found."<<endl;
    }
};


int main()
{
    vector<vector<double>> matrixA  = {{-5, 3, 8}, {4, 1, 5}, {1, -5, 7}};	//here matrices can be changed
    vector<vector<double>> matrixB  = {{-3, 2, 1}, {-5, 4, 4}, {6, 2, 1}};

    //print the game matrices
    cout<<"   A:"<<endl;
    for(size_t i = 0; i<matrixA.size(); i++)
    {
        for(size_t j = 0; j < matrixA[i].size(); j++)
            cout<<matrixA[i][j]<<"\t";
        cout<<endl;
    }
    cout<<endl;
    cout<<"   B:"<<endl;
    for(size_t i = 0; i < matrixB.size(); i++)
    {
        for(size_t j = 0; j < matrixB[i].size(); j++)
            cout<<matrixB[i][j]<<"\t";
        cout<<endl;
    }
    cout<<endl;

    Stackelberg s(matrixA, matrixB);
    s.find();

    cout<<"\nPress any key to continue..."<<endl;
    _getch();
    return 0;
}
