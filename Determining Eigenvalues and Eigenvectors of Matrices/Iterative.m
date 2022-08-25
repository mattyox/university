function it = iteration(a,it)
    r = size(a)(1) %size of matrix in one dimension
    x=zeros(1,r)
    for i=1:r
        x(1,i)=a(1,i)
    end
    x(1,r) = 1
    p=0
    for i=1:r
        p=p+x(1,i)*a(r,i)
    end
    alp = 1/x(1,r) * p
    for k = 1:it
        for i=1:r-1
            c=0
            for j = 1:r
                c = c + x(1,j)*a(i,j)
            end
            x(1,i) = 1/alp * c
        end
        p=0
        for i = 1:r
                p = p+x(1,i)*a(r,i)
        end
        alp = 1/x(1,r) * p
        disp(k)
    end
    disp("our x: ")
    disp(x)
    disp("our eigenvalue: ")
    disp(alp)
    disp("number of iterations: ")
    disp(it)
end
