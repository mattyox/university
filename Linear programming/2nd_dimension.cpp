#include <iostream>
#include <vector>

using namespace std;

// Creates a simplex table to store all needed elements
vector <vector <double>> initial_table(vector <vector <double>> initial_table)
{
    for (int i = 0; i <= 2; i++)
    {
        vector <double> row;
        
        for (int j = 0; j <= 4; j++)
        {
            row.push_back(0.0);
        }
        
        initial_table.push_back(row);
    }
    
    return initial_table;
}

// Fills the simplex table with A matrix
vector <vector <double>> A_matrix(vector <vector <double>> simplex_table)
{
    for (int i = 0; i <= 1; i++)
    {
        cout << "Enter the " << (i + 1) << " row of matrix A";
        
        for (int j = 0; j <= 1; j++)
        {
            cout << " and " << (j + 1) << " column: ";
            double value;
            cin >> value;
            
            simplex_table[i][j] = value;
        }
        
        cout << "\n";
    }
    
    return simplex_table;
}

// Adds linear constraints to the table
vector <vector <double>> linear_constraints(vector <vector <double>> simplex_table)
{
    for (int i = 0; i <= 1; i++)
    {
        for (int j = 2; j <= 3; j++)
        {
            if (i == (j - 2))
            {
                simplex_table[i][j] = 1.0;
            }
        }
    }
    
    return simplex_table;
}

// Fills the simplex table with b vector
vector <vector <double>> b_vector(vector <vector <double>> simplex_table)
{
    for (int i = 0; i <= 1; i++)
    {
        cout << "Enter the " << (i + 1) << " value of b vector: ";
        double value;
        cin >> value;
        
        simplex_table[i][4] = value;
    }
    
    return simplex_table;
}

// Fills the simplex table with c vector
vector <vector <double>> c_vector(vector <vector <double>> simplex_table)
{
    for (int i = 0; i <= 1; i++)
    {
        cout << "Enter the " << (i + 1) << " value of c vector: ";
        double value;
        cin >> value;
        
        simplex_table[2][i] = -value;
    }
    
    return simplex_table;
}

// Creates the complet initial simplex table
vector <vector <double>> create_table()
{
    vector <vector <double>> simplex_table;
    
    simplex_table = initial_table(simplex_table);
    
    simplex_table = A_matrix(simplex_table);
    
    simplex_table = linear_constraints(simplex_table);
    
    simplex_table = b_vector(simplex_table);
    
    cout << "\n";
    
    simplex_table = c_vector(simplex_table);
    
    cout << "\n\n";
    
    return simplex_table;
}


// Checks if the the stop condition is reached and prints the result
void if_solution(int minimum, vector <vector <double>> simplex_table)
{
    if (minimum == 0)
    {
        exit(0);
    }
}

// Searches for column of pivot element
int pivot_column(vector <vector <double>> simplex_table)
{
    double maximum = 0.0;
    int column = 0;
    
    for (int i = 0; i <= 3; i++)
    {
        double checked_element = simplex_table[2][i];
        
        if (checked_element > maximum)
        {
            maximum = checked_element;
            column = i;
        }
    }
    
    if_solution(maximum, simplex_table);
    
    return column;
}

// Searches for row of pivot element
int pivot_row(vector <vector <double>> simplex_table, int column)
{
    double minimum = 1.79769E+308; // Maximum value of double
    int row = 0;
    
    for (int i = 0; i <= 1; i++)
    {
        double ratio = simplex_table[i][4] / simplex_table[i][column];
        
        if (ratio > 0 && ratio < minimum)
        {
            minimum = ratio;
            row = i;
        }
    }
    
    return row;
}

// Prints the coordinates of the pivot element and creates a new simplex table based on the previous simplex table and pivot element
vector <vector <double>> pivoting(vector <vector <double>> simplex_table)
{
    int column = pivot_column(simplex_table);
    int row = pivot_row(simplex_table, column);
    
    double pivot_element = simplex_table[row][column];
    
    cout << "Pivot column: " << (column + 1) << "\n";
    cout << "Pivot row: " << (row + 1) << "\n\n\n\n";

    vector <vector <double>> new_simplex_table (3, vector <double> (5, 0.0));
    
    for (int i = 0; i <= 2; i++)
    {
        if (i == row)
        {
            for (int j = 0; j <= 4; j++)
            {
                new_simplex_table[i][j] = simplex_table[i][j] / pivot_element;
            }
        }
        else
        {
            for (int j = 0; j <= 4; j++)
            {
                if (j == column)
                {
                    new_simplex_table[i][column] = 0;
                }
                else
                {
                    new_simplex_table[i][j] = simplex_table[i][j] - ((simplex_table[i][column] * simplex_table[row][j]) / pivot_element);
                }
            }
        }
    }
    
    return new_simplex_table;
}

// Prints the simplex table
void draw_table(vector <vector <double>> simplex_table)
{
    cout << "Simplex Table:\n\n";
    cout << "x1\t\tx2\t\tx3\t\tx4\t\tb\n\n";
    
    for (int i = 0; i <= 2; i++)
    {
        for (int j = 0; j <= 4; j++)
        {
            cout << simplex_table[i][j] << "\t\t";
        }
        
        cout << "\n\n";
    }
    
    cout << "\n";
}

int main(int argc, const char * argv[])
{
    vector <vector <double>> simplex_table = create_table();
    
    while(1)
    {
        draw_table(simplex_table);
        simplex_table = pivoting(simplex_table);
    }
    
    return 0;
}
