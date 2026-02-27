clear;

N = 8;
% R_dB = 26.0206;
R_dB = 17;
R = 10 ^ (R_dB / 20);
m = N - 1;

x_0 = 0.5 * (((R + sqrt((R^2) - 1)) ^ (1/m)) + ((R - sqrt(R^2 - 1)) ^ (1/m)));

A_3 = x_0 ^ 7;
A_2 = 7 * (A_3 - (x_0 ^ 5));
A_1 = (5 * A_2) - (14 * A_3) + (14 * (x_0 ^ 3));
A_0 = (3 * A_1) - (5 * A_2) + (7 * A_3) - (7 * x_0);

nA_3 = A_3 / A_0;
nA_2 = A_2 / A_0;
nA_1 = A_1 / A_0;
nA_0 = A_0 / A_0;

H = sigwin.chebwin(N, R_dB);
win = generate(H);
wvtool(H);
