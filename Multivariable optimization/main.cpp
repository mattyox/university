#include <iostream>
#include <cmath>

double function_value(double x, double y)
{
    return (2 * (2 * pow(x, 2) - 2 * x * y + pow(y, 2)));
}

double function_gradient_x(double x, double y)
{
    return(8 * x - 4 * y);
}

double function_gradient_y(double x, double y)
{
    return(4 * y - 4 * x);
}

void Gauss_Seidel_method(double z0[2])
{
    std::cout << "Searching for minimum of function at initial point [" << z0[0] << ", " << z0[1] << "]" << std::endl;
    int iterations = 0;
    long double z[2] = { z0[0],z0[1] };
    long double z2[2] = { z0[0],z0[1] };
    long double alpha;
    do
    {
        z[0] = z2[0];
        z[1] = z2[1];
        // d/da(2(2(a + z2[0]) ^ 2 - 2(a + z2[0]) * z2[1] + z2[1] ^ 2)) = 0
        alpha = 0.5 * z2[1] - z2[0];
        z2[0] = z2[0] + alpha;
        // d/da(2(2*z2[0] ^ 2 - 2 * z2[0] * (a + z2[1]) + (a + z2[1]) ^ 2)) = 0
        alpha = z2[0] - z2[1];
        z2[1] = z2[1] + alpha;
        iterations++;
    } while (abs(function_value(z2[0], z2[1]) - function_value(z[0], z[1])) > 0.00000001);
    std::cout << "Gauss Seidel iteration: " << iterations << std::endl;
    std::cout << "Minimum found at point: [" << z2[0] << ", " << z2[1] << "]" << std::endl;
}

void Steepest_descent_method(double z0[2])
{
    std::cout << "Searching for minimum of function at initial point [" << z0[0] << ", " << z0[1] << "]" << std::endl;
    int iterations = 1;
    long double z[2] = { z0[0],z0[1] };
    long double z2[2] = { z0[0],z0[1] };
    long double d[2];
    long double alpha;
    do
    {
        z[0] = z2[0];
        z[1] = z2[1];
        d[0] = function_gradient_x(z2[0], z2[1]);
        d[1] = function_gradient_y(z2[0], z2[1]);
        // d/da(2(2(d[0] * a + z2[0]) ^ 2 - 2(d[0] * a + z2[0]) * (d[1] * a + z2[1]) + (d[1] * a + z2[1]) ^ 2)) = 0
        alpha = (d[0] * (z2[1] - 2 * z2[0]) + d[1] * (z2[0] - z2[1])) / (2 * pow(d[0], 2) - 2 * d[0] * d[1] + pow(d[1], 2));
        z2[0] = z2[0] + alpha * d[0];
        z2[1] = z2[1] + alpha * d[1];
        iterations++;
    } while (abs(function_value(z2[0], z2[1]) - function_value(z[0], z[1])) > 0.00000001);
    std::cout << "Steepest descent iteration: " << iterations << std::endl;
    std::cout  << "Minimum found at point: [" << z2[0] << ", " << z2[1] << "]" << std::endl;
}

int main()
{
    double z0[2] = { 4,4 };
    Gauss_Seidel_method(z0);
    std::cout << std::endl;
    Steepest_descent_method(z0);
    std::cout << std::endl << std::endl;
    z0[0] = 2.5;
    z0[1] = -3.2;
    Gauss_Seidel_method(z0);
    std::cout << std::endl;
    Steepest_descent_method(z0);
    return 0;
}
