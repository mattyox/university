% First function
f1 = @(x) 3*x^2 - 2*x + 2;

% Limits of integration for first function
a1 = 1;
b1 = 6;

% Demanded accuracy first function
epsilon1 = 0.001;

% Integration of first function with limits
int1 = CompositeQuadrature_TrapezoidFormula(f1, a1, b1, epsilon1);

% Displaying result of first integration
disp(int1);


% Second function
f2 = @(x) x^3 + 3*x^2 - 3*x + 1;

% Limits of integration for second integration
a2 = 2;
b2 = 4;

% Demanded accuracy for second function
epsilon2 = 0.001;

% Integration of second function with given limits
int2 = CompositeQuadrature_TrapezoidFormula(f2, a2, b2, epsilon2);

% Displaying result of second integration
disp(int2);


% Third function
f3 = @(x) 5*x^4 + 2*x^2 - 2;

% Limits of integration for third integration
a3 = 1;
b3 = 4;

% Demanded accuracy for third function
epsilon3 = 0.001;

% Integration of third function with given limits
int3 = CompositeQuadrature_TrapezoidFormula(f3, a3, b3, epsilon3);

% Displaying result of third integration
disp(int3);


% Function for calculating definite integral value for given limits of integration using Composite quadrature formulae - trapezoid formula
function int = CompositeQuadrature_TrapezoidFormula(f, a, b, epsilon)

% Calculating initial number of sub-intervals based on given accuracy
m = (b - a) / sqrt(epsilon);

% Rounding up the result to achieve a integer number of sub-intervals
m = ceil(m);

% Assuring the number of sub-intervals is even
if mod(m, 2) ~= 0
    m = m + 1;
end

% Displaying the obtained number
fprintf('Initial number of sub-intervals based on given accuracy: %d\n', m);

% Variables for storing temporary results
% Due to MATLAB limitations (not avaible do...while loop) one of the initial
% results must be nonzero to enter the iteration loop
res1 = 1;
res2 = 0;

% Variable for storing number of performed iterations
it = 0;

% Iteration loop
% Performs calculation as long as the last obtained result is not in given accuracy
while (abs(res2 - res1)) > epsilon
    
    % Copies last result to make place for the new one
    res1 = res2;
    
    % Calculating spacing
    h = (b - a) / m;

    % Vector for storing sub-intervals arguments
    x = zeros(m+1, 1);

    % Calculating sub-intervals arguments
    for i = 1:m+1
        x(i, 1) = a + (i-1) * h;
    end

    % Calculating sum of function values at borders of sub-intervals
    sum = 0;
    for i = 1:m
        sum = sum + f(x(i, 1)) + f(x(i+1, 1));
    end

    % Evaluation of integral value
    res2 = (1/2) * h * sum;
    
    % Doubling the number of sub-intervals
    m = 2*m;
    
    % Increasing number of iterations performed
    it = it + 1;
    
end

% Displaying the final numbers of sub-intervals
fprintf('Final number of sub-intervals needed to achieve given accuracy: %d\n', m/2);

% Displaying the number of performed iterations
fprintf('Number of performed iterations: %d\n', it);

% The final result
int = res2;

end