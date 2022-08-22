% First function
f1 = @(x) 3*x^2 - 2*x + 2;

% Limits of integration for first function
a1 = 1;
b1 = 6;

% Integration of first function with limits
int1 = NewtonCotes_SimpsonFormula(f1, a1, b1);

% Displaying result of first integration
disp(int1);


% Second function
f2 = @(x) x^3 + 3*x^2 - 3*x + 1;

% Limits of integration for second integration
a2 = 2;
b2 = 4;

% Integration of second function with given limits
int2 = NewtonCotes_SimpsonFormula(f2, a2, b2);

% Displaying result of second integration
disp(int2);


% Third function
f3 = @(x) 5*x^4 + 2*x^2 - 2;

% Limits of integration for third integration
a3 = 1;
b3 = 4;

% Integration of third function with given limits
int3 = NewtonCotes_SimpsonFormula(f3, a3, b3);

% Displaying result of third integration
disp(int3);


% Function for calculating definite integral value for given limits of integration using Newton-Cotes method - Simpson formula
function int = NewtonCotes_SimpsonFormula(f, a, b)

% The midpoint of limits
c = (a + b) / 2;

% Calculating spacing
h = (1/2) * (b - a);

% Evaluation of integral value
int = (1/3) * h * (f(a) + 4*f(c) + f(b));

end