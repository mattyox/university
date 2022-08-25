function pow = power(a,it)
    r = size(a)(1) %size of matrix in one dimension
    x = zeros(r,1) %our matrixes during iterations
    temp=zeros(r,1)
    y=zeros(r,1) % y matrix
    x(1)=1
    hme = 0 %highest-module eigenvalue
    ean = zeros(r,1) %eigenvector after normalisation
    for i = 1:it
        x=a*x
        if i == it-1
        temp = x
        end
    end
    for i = 1:r
        y(i) =x(i)/temp(i)
    end
    for i = 1:r
        hme = hme + y(i)
    end
    hme = 1/r * hme

    for i = 1:r
        ean(i) = 1/max(x) * x(i)
    end

    display('Eigenvector of matrix A: ')
    disp(x)
    display('Eigenvalue: ')
    disp(hme)
    display('After normalisation: ')
    disp(ean)
end
