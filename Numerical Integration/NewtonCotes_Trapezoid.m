% First function
f1 = @(x) 3*x^2 - 2*x + 2;

% Limits of integration for first function
a1 = 1;
b1 = 6;

% Integration of first function with limits
int1 = NewtonCotes_TrapezoidFormula(f1, a1, b1);

% Displaying result of first integration
disp(int1);


% Second function
f2 = @(x) x^3 + 3*x^2 - 3*x + 1;

% Limits of integration for second integration
a2 = 2;
b2 = 4;

% Integration of second function with given limits
int2 = NewtonCotes_TrapezoidFormula(f2, a2, b2);

% Displaying result of second integration
disp(int2);


% Third function
f3 = @(x) 5*x^4 + 2*x^2 - 2;

% Limits of integration for third integration
a3 = 1;
b3 = 4;

% Integration of third function with given limits
int3 = NewtonCotes_TrapezoidFormula(f3, a3, b3);

% Displaying result of third integration
disp(int3);


% Function for calculating definite integral value for given limits of integration using Newton-Cotes method - trapezoid formula
function int = NewtonCotes_TrapezoidFormula(f, a, b)

% Calculating spacing
h = b - a;

% Evaluation of integral value
int = (1/2) * h * (f(a) + f(b));

end