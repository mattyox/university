% Power method

%type Matrix.txt;
%A = readmatrix('Matrix.txt');

% Test matrix
A = [1 1 1; 1 2 3; 1 3 6];

% Test number of iterations
it = 8;

% Initial vector
n = length(A);
y = zeros(n, it+1);
y(1, 1) = 1;

for i = 1:it
    y(:, i+1) = A * y(:, i);
end

sum = 0;
for i = 1:n
    sum = sum + y(i, it+1) / y(i, it);
end

eig = (1/n) * sum;

disp(eig);