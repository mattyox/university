A = [-2 1 0; 1 -1 1; 4 -2 0]; % Values of coefficient matrix
b = [1; 0; -2]; % Values of free terms vector

x = GaussElimination(A, b);

function x = GaussElimination(A, b)

x = 0;

[mA, nA] = size(A);
if mA ~= nA
    disp("The system of linear equations can not be solved");
    disp("The coefficients matrix A is not a square matrix");
else

if mA ~= length(b)
    disp("The system of linear equations can not be solved");
    disp("The coefficients matrix A and free terms vector b have different sizes");
else

if det(A) == 0
    disp("The matrix coefficients A is singular, the solution may not exist or it is not unique");
end

% Forward elimination (creating upper triangular system)
for i = 1:nA % Selecting column (from first to last)
    if A(i, i) == 0
        disp("The system of linear equations can not be solved");
        disp("The use partial pivoting is needed");
    else
        for j = i+1:mA % Selecting row (starting at next)
            m = - A(j, i) / A(i, i); % Pivot element
            A(j, :) = A(j, :) + m*A(i, :); % New coefficients matrix row
            b(j) = b(j) + m*b(i); % New free term value
        end
    end
end

x = zeros(mA, 1); % Creating vector for calculating the solutions

% Backward substitution
for i = mA:-1:1 % Selecting row (from last to first)
    s = 0; % The initial row consist only of coefficient and free term
    for j = mA:-1:i+1 % Selecting column (from last to one before the current c 
        s = s + A(i, j) * x(j); % Calculation of substitution
    end
    x(i) = (b(i)- s) / A(i, i); % Calculation of solution
end

% Displaying the calculated result
disp("x = ");
disp(x);

end

end

end