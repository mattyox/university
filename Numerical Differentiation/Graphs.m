f = @(x) - (96*sin((24*x*(x - 1))/25 - 2/5))/25 - 2*cos((24*x*(x - 1))/25 - 2/5)*((48*x)/25 - 24/25)^2;

% Vectors for stroring values to plot
x_vector = zeros(0);
y_vector = zeros(0);

for x = 0:0.01:9
    y = f(x);
    
    % Assigning values to vectors
    x_vector(end+1) = x;
    y_vector(end+1) = y;
end

% Creating the plot
plot(x_vector, y_vector);
grid on;