#include <iostream>
#include <vector>
#include <algorithm>
#include <conio.h>

using namespace std;

//Program solving a zero sum 2x2 game
int main()
{
    vector<vector<double>> matrix = { {4, -16}, {-12, 8} };      //game matrix { {1st row}, {2nd row} }

    //print the matrix
    for(size_t i=0; i<matrix.size(); i++)
    {
        for(size_t j=0; j<matrix[i].size(); j++)
            cout<<matrix[i][j]<<"\t";
        cout<<endl;
    }
    cout<<endl;


    //find safe strategy for D1
    vector<double> outcomesD1; //vector storing best outcomes for each strategy

    //values used during comparison of outcomes
    double min;
    double max;
    //find best outcome in every D1 strategy
    for(size_t i=0; i<matrix.size(); i++)
    {
        max = matrix.front().at(i);
        outcomesD1.push_back(0);
        for(size_t j=0; j<matrix[0].size(); j++)
        {
            if(matrix[i][j]>max)
            {
                max=matrix[i][j];
            }
            if(matrix[i][j]==max)
                outcomesD1[i]=matrix[i][j];
        }
    }

    min = outcomesD1[0];
    vector<int> pureD1;               //vector storing numbers of pure strategies for D1
    //find best outcome(s) in all D1 strategies
    for(size_t i=0; i<outcomesD1.size(); ++i)
    {
        if(outcomesD1[i]<min)
        {
            min=outcomesD1[i];
            pureD1.clear();
        }

        if(outcomesD1[i]==min)
            pureD1.push_back(i);
    }

    //print pure strategies for D1
    cout<<"Pure/safe strategies for D1: ";
    if(!pureD1.empty())
    {
        for(size_t i=0; i<pureD1.size(); ++i)
        {
            cout<<pureD1[i]+1;
            if(i+1<pureD1.size())
                cout<<", ";
        }
        cout<<" (Safety level: "<<outcomesD1[pureD1[0]]<<")"<<endl;
    }

    else
        cout<<"None."<<endl;



    //find safe strategy for D2
    vector<double> outcomesD2; //vector storing best outcomes for each strategy

    //find best outcome in every strategy
    for(size_t i=0; i<matrix[0].size(); i++)
    {
        min = matrix.front().at(i);
        outcomesD2.push_back(0);
        for(size_t j=0; j<matrix.size(); j++)
        {
            if(matrix[j][i]<min)
            {
                min=matrix[j][i];
            }
            if(matrix[j][i]==min)
                outcomesD2[i]=matrix[j][i];
        }

    }

    //find best outcome(s) of all strategies
    max = outcomesD2[0];
    vector<int> pureD2;               //vector storing numbers of pure strategies for D2
    for(size_t i=0; i<outcomesD2.size(); ++i)
    {
        if(outcomesD2[i]>max)
        {
            max=outcomesD2[i];
            pureD2.clear();
        }

        if(outcomesD2[i]==max)
            pureD2.push_back(i);
    }

    //print pure strategies for D2
    cout<<"Pure/safe strategies for D2: ";
    if(!pureD2.empty())
    {
        for(size_t i=0; i<pureD2.size(); ++i)
        {
            cout<<pureD2[i]+1;
            if(i+1<pureD2.size())
                cout<<", ";
        }
        cout<<" (Safety level: "<<outcomesD2[pureD2[0]]<<")"<<endl;
    }

    else
        cout<<"None."<<endl;
    cout<<endl;







    //finding mixed strategies

    vector<double> y;   //probability for D1
    vector<double> z;   //probability for D2

    //check for dominant strategies for D1
    if(matrix[0][0] < matrix[1][0] && matrix[0][1] < matrix[1][1])
    {
        y.push_back(1);
        y.push_back(0);
    }

    if(matrix[0][0] > matrix[1][0] && matrix[0][1] > matrix[1][1])
    {
        y.push_back(0);
        y.push_back(1);
    }


    //check for dominant strategies for D2
    if(matrix[0][0] > matrix[0][1] && matrix[1][0] > matrix[1][1])
    {
        z.push_back(1);
        z.push_back(0);
    }

    if(matrix[0][0] < matrix[0][1] && matrix[1][0] < matrix[1][1])
    {
        z.push_back(0);
        z.push_back(1);
    }

    //if there are no dominant strategies, and if all results of the game are not the same
    if(z.empty()&&y.empty()&&(matrix[0][0] + matrix[1][1] != matrix[0][1] + matrix[1][0]))
    {
        //Calculate probability of first strategy for D1
        y.push_back((matrix[1][1] - matrix[1][0])/(matrix[0][0] - matrix[1][0] - matrix[0][1] + matrix[1][1]));
        //Calculate probability of second strategy for D1
        y.push_back(1-y[0]);

        //Calculate probability of first strategy for D2
        z.push_back((matrix[1][1] - matrix[0][1])/(matrix[0][0] - matrix[1][0] - matrix[0][1] + matrix[1][1]));
        //Calculate probability of second strategy for D2
        z.push_back(1-z[0]);
    }
    //if there are dominant strategies
    else
    {
        //if D1 has dominant strategy while D2 has not
        if(!y.empty()&&z.empty())
        {
            //set D2's strategy as safe strategy
            if(pureD2[0]==0)
            {
                z.push_back(1);
                z.push_back(0);
            }
            else
            {
                z.push_back(0);
                z.push_back(1);
            }
        }
        //if D2 has dominant strategy while D1 has not
        if(!z.empty()&&y.empty())
        {
            //set D1's strategy as safe strategy
            if(pureD2[0]==0)
            {
                y.push_back(1);
                y.push_back(0);
            }
            else
            {
                y.push_back(0);
                y.push_back(1);
            }
        }
        //if D1 and D2 have a dominant strategy, problem solves on its own
    }

    if(!y.empty()&&!z.empty())
    {
        cout<<"Mixed strategy for D1: y1 = "<<y[0]<<", y2 = "<<y[1]<<endl;
        cout<<"Mixed strategy for D2: z1 = "<<z[0]<<", z2 = "<<z[1]<<endl;
    }

    cout<<"\nPress any key to exit."<<endl;
    _getch();
    return 0;
}
