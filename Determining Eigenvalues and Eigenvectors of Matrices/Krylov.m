function kr = krylov(a)
    r = size(a)(1) %size of matrix in one dimension
    x = zeros(r,1) %our matrixes during iterations
    A = zeros(r,r)
    B = zeros(r,1)
    A(1,r)=1
    x(1)=1
    c = r-1
    for i = 1:r
        x=a*x
        if i != r
            for j = 1:r
                A(j,c)=x(j)
            end
        else
            B=x
            B = B*(-1)
        end

        c=c-1
    end

    disp('Matrix A(our coefficients) - they are in the order like the should be in the equations')
    disp(A)
    disp('Matrix B(our results) - already *(-1) because the are on the right side of formula')
    disp(B)
    disp('Obtained coefficients')
    linsolve(A,B)
end
