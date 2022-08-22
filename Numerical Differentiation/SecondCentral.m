f = @(x) 2*cos(0.96*x * (1-x) + 0.4); % Given function for differentation


%{
% Differentation parameters
x = 3;
h = 0.1;
n = 8; % For calculating the first derivative with central difference the number of heightest difference operator used in series expansion must be even

der = SecondCentralDifference(f, x, h, n); % Second order differentation using central difference

disp(der); % Displaying the result
%}



% Differentation parameters
h = 0.1;
n = 8; % For calculating the first derivative with central difference the number of heightest difference operator used in series expansion must be even

% Vectors for stroring values to plot
x_vector = zeros(0, 0);
y_vector = zeros(0, 0);

for x = 0:0.01:9
    der = SecondCentralDifference(f, x, h, n); % Second order differentation using central difference
    disp(der); % Displaying the result
    
    % Assigning values to vectors
    x_vector(end+1) = x;
    y_vector(end+1) = der;
end

% Creating the plot
plot(x_vector, y_vector);
grid on;

% Central difference for second derivative
function der = SecondCentralDifference(f, x, h, n)

arg = zeros(n+1, 1); % First column of difference table for storing arguments

% Calculating arguments for central difference
arg_chg = (-(n/2)); % Inintial value of change in argument
for i = 1:n+1
    arg(i, 1) = x + arg_chg * h; % Change of argument value
    arg_chg = arg_chg + 1; % Incrementing the value of change in argument each step
end

differences = zeros(n+1); % Remaining columns of difference table for storing values

% Calculaitng values for second column of difference table
for i = 1:n+1
    differences(i, 1) = f(arg(i, 1)); % Simply the function value for given argument from first column
end

% Calculating values for remaining part of difference table
for i = 2:n+1 % Selecting column
    for j = 1:n+2-i % Selecting row
        differences(j, i) = differences(j+1, i-1) - differences(j, i-1); % Calculating successive backward difference operators
    end
end

% Calculating value of power series expansion

series = differences(n/2, 3); % First term in the series

chg = 2; % Additional variable needed for moving through the table
for i = 2:n/2
    if mod(i,2) == 0 % For even step
        series = series - 2^(2*i - 3) / factorial(2*i) * differences(n/2 - chg + 1, 2*i + 1); % Subtracting
    else % For odd step
        series = series + 2^(2*i - 3) / factorial(2*i) * differences(n/2 - chg + 1, 2*i + 1); % Adding
    end
    chg = chg + 1; % Incrementing the variable each step
end

der = series / h^2; % Final result

end