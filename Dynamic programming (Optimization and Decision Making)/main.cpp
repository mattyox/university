#include <iostream>
#include <vector>

using namespace std;

// Asks the user for number of periods k
int get_periods_number()
{
    int periods;
    
    cout << "Enter number of periods: ";
    cin >> periods;
    cout << "\n";
    
    return periods;
}

// Asks the user for initial stock of the store x_0
int get_initial_stock()
{
    int initial_stock;
    
    cout << "Enter intial stock: ";
    cin >> initial_stock;
    cout << "\n";
    
    return initial_stock;
}

// Asks the user for the store capacity limit c
int get_store_capacity()
{
    int stock_capacity;
    
    cout << "Enter store capacity: ";
    cin >> stock_capacity;
    cout << "\n";
    
    return stock_capacity;
}

// Asks the user for unit cost alpha of each period
vector <int> get_costs(int periods)
{
    vector <int> costs; // Vector of all values
    
    cout << "Enter unit cost for\n";
    
    for (int k = 1; k <= periods; k++)
    {
        cout << k << " period: ";
        
        int cost;
        cin >> cost;
        costs.push_back(cost); // Adds unit cost of k-th period to the vector
    }
    
    cout << "\n";
    
    return costs;
}

// Asks the user for unit cost beta of each period
vector <int> get_profits(int periods)
{
    vector <int> profits; // Vector of all values
    
    cout << "Enter unit profit for\n";
    
    for (int k = 1; k <= periods; k++)
    {
        cout << k << " period: ";
        
        int profit;
        cin >> profit;
        profits.push_back(profit); // Adds unit profit of k-th period to the vector
        
    }
    
    cout << "\n";
    
    return profits;
}

// Creates a empty table, which will be used for storing calculations
// First row [0] - y parameters - number of items bought
// Second row [1] - z parameters - number of items sold
// Third row [2] - x parameters - number of items in store
// One column of the table represents one period
vector <vector <int>> create_table(int periods)
{
    vector <vector <int>> table;
    
    for (int i = 1; i <= 3; i++)
    {
        vector <int> row;
        
        for (int j = 1; j <= periods; j++)
        {
            row.push_back(0);
        }
        
        table.push_back(row);
    }
    
    return table;
}

// Calculates maximum profit using analytical method
vector <vector <int>> analytical_method(int periods, int initial_stock, int store_capacity, vector <int> costs, vector <int> profits)
{
    // Table for storing parameters of the S_max equations
    vector <vector <int>> parameters = create_table(periods);
    
    // Vector for parameters of x from the final form of S equations (additional period represents assumed final state with stock equal to 0 - to maximize profit all items have to be sold
    vector <int> s (periods + 1, 0);
    
    // Table for storing final values of y, z and x
    vector <vector <int>> values = create_table(periods);
    
    // Calculates parameters from last to firts period
    for (int k = periods; k >= 1; k--)
    {
        parameters[0][k - 1] = -costs[k - 1] + s[k]; // Parameter of y calculated with actual's period cost and parameter got from previos S equation (parameter of x multiplied by y from state equation - 1)
        parameters[1][k - 1] = profits[k - 1] - s[k]; // Parameter of z calculated with actual's period profit and parameter got from previos S equation (parameter of x multiplied by z from state equation - -1)
        parameters[2][k - 1] = s[k]; // Parameter of x calculated with parameter got from previous S equation (parameter of x multiplied by x from state equation - 1)
        
        if (parameters[1][k - 1] > 0)
        {
            s[k - 1] = parameters[2][k - 1] + parameters[1][k - 1]; // If parameter of z is positive, it is added to parameter x and set as final parameter of x (z as high as possbile - based on the constraint)
        }
        else
        {
            s[k - 1] = parameters[2][k - 1]; // If parameter of z is negative, final parameter of x is equal only to parameter of x
        }
    }
    
    // Sets the intial stock of the store
    values[2][0] = initial_stock;
    
    // Calculates values of z, y and x starting from first period and ending at last k period
    for (int k = 1; k <= periods; k++)
    {
        // Calculates value of z
        if (parameters[1][k - 1] < 0)
        {
            values[1][k - 1] = 0; // If the parameter of z is negative, its value is set as low as possible (0 based on the constraint)
        }
        else
        {
            values[1][k - 1] = values[2][k - 1]; // If the parameter of z is positive, its value is set as high as possible (equal to x based on the constraint)
        }
        
        // Calculates value of y
        if (parameters[0][k - 1] < 0)
        {
            values[0][k - 1] = 0; // If the parameter of y is negative, its value is set as low as possible (0 based on the constraint)
        }
        else
        {
            values[0][k - 1] = store_capacity - (values[2][k - 1] - values[1][k - 1]); // If the parameter of y is positive, its value is set as high as possible (calculated by the constraint)
        }
        
        // Calculates value of x, based on the state equation and values of parameters from previous state
        values[2][k] = values[2][k - 1] + values[0][k - 1] - values[1][k - 1];
    }
    
    return values;
}

// Calculates total profit based on values of z and profits, y and costs
int calculate_total_profit(int periods, vector <int> costs, vector <int> profits, vector <vector <int>> parameters)
{
    int total_profit = 0;
    
    for (int k = 1; k <= periods; k++)
    {
        total_profit += (parameters[1][k - 1] * profits[k - 1]); // Profit at period k
        total_profit += (parameters[0][k - 1] * -costs[k - 1]); // Cost at period k
    }
    
    return total_profit;
}

// Shows final result in form of a table
void show_result(int periods, vector <vector <int>> parameters, int total_profit)
{
    cout << "\n\nPeriod\t\t\t";
    for (int k = 1; k <= periods; k++)
    {
        cout << k << "\t\t"; // Prints number of all period
    }
    cout << "\n\n";
    
    cout << "Items in stock\t\t";
    for (int k = 1; k <= periods; k++)
    {
        cout << parameters[2][k - 1] << "\t\t"; // Prints number of items in stock at k-th period
    }
    cout << "\n\n";
    
    cout << "Items sold\t\t";
    for (int k = 1; k <= periods; k++)
    {
        cout << parameters[1][k - 1] << "\t\t"; // Prints number of items sold at k-th period
    }
    cout << "\n\n";
    
    cout << "Items bought\t\t";
    for (int k = 1; k <= periods; k++)
    {
        cout << parameters[0][k - 1] << "\t\t"; // Prints number of items bought at k-th period
    }
    cout << "\n\n";
    
    cout << "Total profit: " << total_profit << "\n\n";
}

int main()
{
    int periods = get_periods_number();
    
    int initial_stock = get_initial_stock();
    
    int store_capacity = get_store_capacity();
    
    vector <int> costs = get_costs(periods);
    
    vector <int> profits = get_profits(periods);
    
    /* Test values
    periods = 6;
    initial_stock = 30;
    store_capacity = 50;
    costs = {20, 2, 40, 10, 15, 30};
    profits = {10, 50, 2, 10, 3, 1};
    */
    
    vector <vector <int>> parameters = analytical_method(periods, initial_stock, store_capacity, costs, profits);
    
    int total_profit = calculate_total_profit(periods, costs, profits, parameters);
    
    show_result(periods, parameters, total_profit);
    
    return 0;
}
