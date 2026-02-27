N = 8;                       % matrix size
Y = zeros(N, N);             % admittance matrix

for k = 1:N
    % Load numeric data from file
    data = load(sprintf('dipol_%d.txt', k));
    
    % Remove frequency (first value)
    vals = data(2:end);
    
    % Convert real/imag pairs to complex numbers
    column = vals(1:2:end) + 1i * vals(2:2:end);
    
    % Store as k-th row
    Y(:, k) = column;
end

Z = inv(Y);      % impedance matrix

I = [0.779; 0.710; 0.895; 1; 1; 0.895; 0.710; 0.779];

U = Z * I;

U_mag   = abs(U);                   % |U|
U_phase_deg = rad2deg(angle(U));    % phase in angles