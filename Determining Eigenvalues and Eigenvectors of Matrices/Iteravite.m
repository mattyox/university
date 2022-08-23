type Matrix.txt;
A = readmatrix('Matrix.txt');

x1=A(1,1);
    x2=A(1,2);
    x3=A(1,3);
    alp = 1/x3 * (x1 + A(3,2)*x2+A(3,3)*x3);
    for k = 1:8
        t = x1;
        x1 = 1/alp * (x1*A(1,1)+x2*A(1,2)+x3*A(1,3));
        x2 = 1/alp * (x1*A(2,1)+x2*A(2,2)+A(2,3));
        alp = x1*A(3,1) + x2*A(3,2) + A(3,3);
        X = ['Iteration: (', num2str(k),') x1=',num2str(x1),' x2=',num2str(x2),' x3=',num2str(x3),' a=',num2str(alp)];
        disp(X)
    end