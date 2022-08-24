#include <iostream>
#include <vector>

using namespace std;

// Asks for dimension of the problem
int get_dimension()
{
    cout << "Enter the dimension of the problem: ";
    int dimension;
    cin >> dimension;
    
    return dimension;
}

// Asks for the number of constraints
int get_constraints_number()
{
    cout << "Enter the number of constrainst: ";
    int constraints_number;
    cin >> constraints_number;
    
    cout << "\n";
    
    return constraints_number;
}

// Creates a simplex table to store all needed elements
vector <vector <double>> initial_table(vector <vector <double>> initial_table, int constraints_number, int dimension)
{
    for (int i = 0; i <= constraints_number; i++)
    {
        vector <double> row;
        
        for (int j = 0; j <= (dimension + constraints_number); j++)
        {
            row.push_back(0.0);
        }
        
        initial_table.push_back(row);
    }
    
    return initial_table;
}

// Fills the simplex table with A matrix
vector <vector <double>> A_matrix(vector <vector <double>> simplex_table, int constraints_number, int dimension)
{
    for (int i = 0; i <= (constraints_number - 1); i++)
    {
        cout << "Enter the " << (i + 1) << " row of matrix A";
        
        for (int j = 0; j <= (dimension - 1); j++)
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
vector <vector <double>> linear_constraints(vector <vector <double>> simplex_table, int constraints_number, int dimension)
{
    for (int i = 0; i <= (constraints_number - 1); i++)
    {
        for (int j = dimension; j <= ((dimension + constraints_number) - 1); j++)
        {
            if (i == (j - dimension))
            {
                simplex_table[i][j] = 1.0;
            }
        }
    }
    
    return simplex_table;
}

// Fills the simplex table with b vector
vector <vector <double>> b_vector(vector <vector <double>> simplex_table, int constraints_number, int dimension)
{
    for (int i = 0; i <= (constraints_number - 1); i++)
    {
        cout << "Enter the " << (i + 1) << " value of b vector: ";
        double value;
        cin >> value;
        
        simplex_table[i][(dimension + constraints_number)] = value;
    }
    
    return simplex_table;
}

// Fills the simplex table with c vector
vector <vector <double>> c_vector(vector <vector <double>> simplex_table, int dimension, int constraints_number)
{
    for (int i = 0; i <= (dimension - 1); i++)
    {
        cout << "Enter the " << (i + 1) << " value of c vector: ";
        double value;
        cin >> value;
        
        simplex_table[constraints_number][i] = -value;
    }
    
    return simplex_table;
}

// Creates the complet initial simplex table
vector <vector <double>> create_table(int constraints_number, int dimension)
{
    vector <vector <double>> simplex_table;
    
    simplex_table = initial_table(simplex_table, constraints_number, dimension);
    
    simplex_table = A_matrix(simplex_table, constraints_number, dimension);
    
    simplex_table = linear_constraints(simplex_table, constraints_number, dimension);
    
    simplex_table = b_vector(simplex_table, constraints_number, dimension);
    
    cout << "\n";
    
    simplex_table = c_vector(simplex_table, dimension, constraints_number);
    
    cout << "\n\n";
    
    return simplex_table;
}

// Checks if the the stop condition is reached and prints the result
void if_solution(double maximum, vector <vector <double>> simplex_table, int constraints_number, int dimension)
{
    if (maximum == 0.0)
    {
        for (int i = 1; i <= (constraints_number + dimension); i++)
        {
            cout << "x" << i << "\t\t";
        }
        cout << "b\n\n";
        
        for (int i = 0; i <= constraints_number; i++)
        {
            for (int j = 0; j <= (constraints_number + dimension); j++)
            {
                cout << simplex_table[i][j] << "\t\t";
            }
            
            cout << "\n\n";
        }
        
        exit(0);
    }
}

// Searches for column of pivot element
int pivot_column(vector <vector <double>> simplex_table, int dimension, int constraints_number)
{
    double maximum = 0.0; // Lowest nonnegative value
    int column = 0;
    
    for (int i = 0; i <= ((dimension + constraints_number) - 1); i++)
    {
        double checked_element = simplex_table[constraints_number][i];
        
        if (checked_element > maximum)
        {
            maximum = checked_element;
            column = i;
        }
    }
    
    if_solution(maximum, simplex_table, constraints_number, dimension);
    
    return column;
}

// Searches for row of pivot element
int pivot_row(vector <vector <double>> simplex_table, int column, int constraints_number, int dimension)
{
    double minimum = 1.79769E+308; // Maximum value of double
    int row = 0;
    
    for (int i = 0; i <= (constraints_number - 1); i++)
    {
        double ratio = simplex_table[i][(dimension + constraints_number)] / simplex_table[i][column];
        
        if (ratio > 0.0 && ratio < minimum)
        {
            minimum = ratio;
            row = i;
        }
    }
    
    return row;
}

// Prints the coordinates of the pivot element and creates a new simplex table based on the previous simplex table and pivot element
vector <vector <double>> pivoting(vector <vector <double>> simplex_table, int constraints_number, int dimension)
{
    int column = pivot_column(simplex_table, dimension, constraints_number);
    int row = pivot_row(simplex_table, column, constraints_number, dimension);
    
    double pivot_element = simplex_table[row][column];

    vector <vector <double>> new_simplex_table (((dimension + constraints_number) + 1), vector <double> (((dimension * 2) + 1), 0.0));
    
    for (int i = 0; i <= constraints_number; i++)
    {
        if (i == row)
        {
            for (int j = 0; j <= (dimension + constraints_number); j++)
            {
                new_simplex_table[i][j] = simplex_table[i][j] / pivot_element;
            }
        }
        else
        {
            for (int j = 0; j <= (dimension + constraints_number); j++)
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

int main(int argc, const char * argv[])
{
    int dimension = get_dimension();
    int constraints_number = get_constraints_number();
    
    vector <vector <double>> simplex_table = create_table(constraints_number, dimension);
    
    while(1)
    {
        simplex_table = pivoting(simplex_table, constraints_number, dimension);
    }
    
    return 0;
}
