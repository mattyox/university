h = 0.2;                                            
x = 0:h:9;                                         
Y = zeros(1,length(x)); 
y(1) = 1.9; 
Fun = @(t,r) -5.8.*r + 4.7.*t - 0.9.*t.*t; 

tspan = [0,9];
y0 = 1.9;
[tx, yx] = ode45(Fun, tspan, y0);
plot(x3_1, y3_1, x3_2, y3_2, tx, yx);
legend({'Euler method','4th order Runge-Kutta method', 'MATLAB solution (ode45)'},'Location','southwest')
xlabel('x');
ylabel('y');
grid on;
grid minor;

