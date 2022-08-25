ourA=[-2 1 0; 1 -1 1; 0 1 -3];
ourB=[1 0 -2];
ourX=zeros(length(ourA),1)
tolerance = 0.001
iterations = 4

Seidel(tolerance,iterations,ourB,ourA,ourX)

function s = Seidel(acc,it,b,A,X)
    Y = zeros(length(A),1)
    for i = 1 : it
        Y=X
        for j = 1 : length(b)
            for k = 1 : length(A)
                if k == j
                    r = r + b(j)
                else
                    r = r + A(j,k)*X(k)
                end
            end
            X(j) = r/A(j,j)
        end
        if abs(Y-X) < acc
            break
        end
    end
    X(2) = X(2)*(-1)

    disp('Number of iterations: ')
    disp(i)
    disp('Obtained x1,x2,x3: ')
    sprintf('%1.5f ',X)
end
