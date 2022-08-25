#include <iostream>
#include <string>

using namespace std;

void backward(double x[], double y[], int s)
{
    double temp[s][s]; // our temporary array to get lower Y values
    double h = x[1] - x[0]; // our interval

    for (int i = 0; i < s; i++) // fulfilling our temporary array with zeros
    {
        for (int j = 0; j < s; j++)
        {
            if (j == 0)
            {
                temp[i][j] = y[i];
            }
            else
            {
                temp[i][j] = 0;
            }
        }
    }

    int r = 1;

    for (int i = 0; i < s; i++) // subtracting values
    {
        for(int j = 1; j < r; j++)
        {
            temp[i][j] = temp[i][j-1] - temp[i-1][j-1];
        }

        r++;
    }

    for (int i = 0; i < s; i++) //function which prints our already solved table
    {
        cout << x[i] << "\t";

        for (int j = 0; j < s; j++)
        {
            cout << temp[i][j] << "\t";
        }
        cout << "\n";
    }

    cout << "\nt=(x -" << x[s-1] << ")/" << h << "\nh=" << h;
    cout << "\nP(x)= ";
    
    for(int i = 0; i < s; i++)
    {
        if(temp[4][i] != 0)
        {
            cout << "(" << temp[4][i] << ")";

            for(int j = 0; j < i; j++)
            {
                if(j == 0)
                {
                    cout << "t";
                }
                else
                {
                    cout << "(t+" << j << ")";
                }
            }

            if(i != 0 and i != 1)
            {
                cout << "/" << i << "!";
            }
        }
        
        if (i != s-1 and temp[4][i] != 0)
        {
            cout << " + ";
        }
    }
}

int main()
{
    //Firstly we test our program for Xs and Ys from pdf to be sure that it works
    double x1[5] = {1, 1.5, 2, 2.5, 3}; // our x's in order x4,x3,x2...
    double y1[5] = {1, 3, 4, 2, 0}; // our ys in order y4, y3, y2...
    int s1 = 5; //number of elements
    backward(x1, y1, s1);

    cout << "\n\n------------\n\n";

    //Then we use our program to calculate Newton Polynomial for given Xs and Ys
    double x2[5] = {1, 3, 5, 7, 9}; // our x's in order x4,x3,x2...
    double y2[5] = {2, 4, -1, 1, -2}; // our ys in order y4, y3, y2...
    int s2 = 5; //number of elements
    backward(x2, y2, s2);
}
