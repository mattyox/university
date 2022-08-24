//
//  main.cpp
//  Lab 08. Approximate Solution of Non-linear Equation
//
//  Created by Mateusz Wolowczyk on 15/06/2021.
//

#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

// Bisection method implemenation for finding root of function f(x) = x^3 - 5 in given limit [a, b] with given accuracy epsilon
double bisection(double a, double b, double epsilon)
{
    double m = 0; // Variable for storing midpoint between a and b (beginning and ending of limit)
    double m_old = 0; // Variable for storing the previous midpoint for checking the result's accuracy
    double f_a = 0; // Variable for storing the function's at a
    double f_m = 0; // Variable for storing the function's at b
    int i = 0; // Step counter
    
    // Loop for iterative calculation of root
    do
    {
        i++; // Increasing of step counter
        
        m_old = m; // Saving the previous midpoint
        
        m = (a + b) / 2; // Calcualtion of the new midpoint
        
        cout << "Step: " << i << "\na = " << a << "\tm = " << m << "\tb = " << b << "\n"; // Printing the number of current step, argument a and b
        
        f_a = a*a*a - 5; // Calculating value of the function at begining of limit
        f_m = m*m*m - 5; // Calculating value of the function at midpoint
        
        cout << "f(a) = " << f_a << "\tf(m) = " << f_m << "\n\n"; // Printing the values of function at a and b
        
        if((f_a * f_m) < 0) // Checking if the function changes sing
        {
            b = m; // Setting the midpoint as the new ending of limit
        }
        else if((f_a * f_m) > 0) // Checking if the function does not change sing
        {
            a = m; // Setting the midpoint as the new begining of limit
        }
        else // Funtion value is equal to 0
        {
            return m; // Midpoint is the root of function
        }
    }
    while (abs((m - m_old) / m) > epsilon); // Checking if the obtained result fulfills the demanded accuracy by calculating the relative approximation error
    
    cout << "\n";
    
    return m; // Returning the obtained result
}

// Function for calculating analytical solution of root of function f(x) = x^3 - 5
double analytical()
{
    double root;
    
    root = cbrt(5); // Cube root of 5 (obtained from transformation of the initial equation
    
    return root;
}

// Function for calculating the absolute error of calculated root
double absolute_error(double bisection_result, double analytical_result)
{
    double error;
    
    error = bisection_result - analytical_result; // Simply the difference between results of bisection method and analytical
    
    return error;
}


int main()
{
    double a = 1; // Beginning of given limit
    double b = 3; // Ending of given limit
    double epsilon = 0.005; // Given accuracy
    
    
    double bisection_result = 0; // Variable for storing the bisection method solution
    
    bisection_result = bisection(a, b, epsilon); // Calculating the bisection method solution
    
    cout << "The bisection method solution for calculating root of the function: " << bisection_result << "\n\n"; // Displaying the bisection method result
    
    
    double analytical_result = 0; // Variable for storing the analytical solution
    
    analytical_result = analytical(); // Calculating the analytical solution
    
    cout << "The analytical solution for calculating root of the function: " << analytical_result << "\n\n"; // Displaying the analytical result
    
    
    double error = 0;  // Variable for storing the absolute error
    
    error = absolute_error(bisection_result, analytical_result); // Calculating the absolute error
    
    cout << "The absolute error: " << error << "\n\n"; // Displaying the absolute error
}
