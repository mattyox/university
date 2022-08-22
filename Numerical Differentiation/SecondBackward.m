f = @(x) 2*cos(0.96*x * (1-x) + 0.4); % Given function for differentation


%{
% Differentation parameters
x = 3;
h = 0.1;
n = 4;

der = SecondBackwardDifference(f, x, h, n); % Second order differentation using backward difference

disp(der); % Displaying the result
%}


% Differentation parameters
h = 0.1;
n = 5;

% Vectors for stroring values to plot
x_vector = zeros(0);
y_vector = zeros(0);

for x = 0:0.01:9
    der = SecondBackwardDifference(f, x, h, n); % Second order differentation using backward difference
    disp(der); % Displaying the result
    
    % Assigning values to vectors
    x_vector(end+1) = x;
    y_vector(end+1) = der;
end

% Creating the plot
plot(x_vector, y_vector);
grid on;


% Backward difference for second derivative
function der = SecondBackwardDifference(f, x, h, n)

arg = zeros(n+1, 1); % First column of difference table for storing arguments

% Calculating arguments for backward difference
for i = 1:n+1
    arg(i, 1) = x - (i-1) * h; % Arguments starts from x and are increased by h n-times
end

differences = zeros(n+1); % Remaining columns of difference table for storing values

% Calculaitng values for second column of difference table
for i = 1:n+1
    differences(i, 1) = f(arg(i, 1)); % Simply the function value for given argument from first column
end

% Calculating values for remaining part of difference table
for i = 2:n+1 % Selecting column
    for j = 1:n+2-i % Selecting row
        differences(j, i) = differences(j, i-1) - differences(j+1, i-1); % Calculating successive backward difference operators
    end
end

% Calculating value of power series expansion

series = differences(1, 3) + differences(1, 4); % First two terms in the series

for i = 4:n
    series = series + ((15 - i) / 12) * differences(1, i+1); % Remaining terms
end

der = series / h^2; % Final result

end