a1 = -5.8;
b1 = 4.7;
c1 = -0.9;
d1 = 1.9; %initial condition
x1 = 0; %x1x<e1
e1 = 9; %x1x<e1
h1 = 0.2; %step

function rk = runge(a,b,c,d,x,e,h)
    it = e/h; %number of our iterations
    ANS = ['Our function with a=',num2str(a),' b=',num2str(b),' c=',num2str(c),' with initial condition x=',num2str(d),
         ' where:',num2str(x),'<x<',num2str(e),' and our step h=',num2str(h),'. Number of iterations:',num2str(it)];
    disp(ANS)
    table = zeros((it*5)+1,5);
    xy = zeros(it,3);
    for i=1:it
        r = d;
        sum = 0;
        for j=1:5
            table((i-1)*5+j,1)=i-1;
            if j==1
                table((i-1)*5+j,2)=x;
                table((i-1)*5+j,3)=d;
                f = (a*d + b*x + c*(x^2)) * h;
                sum = sum + f;
                table((i-1)*5+j,4)=f;
                table((i-1)*5+j,5)=f;
            elseif j==2
                d = r + f/2;
                x=x+h/2;
                table((i-1)*5+j,2)=x;
                table((i-1)*5+j,3)=d;
                f = (a*d + b*x + c*(x^2)) * h;
                sum = sum + f;
                table((i-1)*5+j,4)=f;
                table((i-1)*5+j,5)=2*f;
            elseif j==3
                d = r + f/2;
                table((i-1)*5+j,2)=x;
                table((i-1)*5+j,3)=d;
                f = (a*d + b*x + c*(x^2)) * h;
                sum = sum + f;
                table((i-1)*5+j,4)=f;
                table((i-1)*5+j,5)=2*f;
            elseif j==4
                d = r + f;
                x=x+h/2;
                table((i-1)*5+j,3)=d;
                table((i-1)*5+j,2)=x;
                f = (a*d + b*x + c*(x^2)) * h;
                sum = sum + f;
                table((i-1)*5+j,4)=f;
                table((i-1)*5+j,5)=f;
            else
                table((i-1)*5+j,5)=sum/6;
            end
        end
        if i == it
            table((it*5)+1,1) = it;
            table((it*5)+1,2) = x;
            table((it*5)+1,3) = d;
            table((it*5)+1,4) = f;
            table((it*5)+1,5) = sum/6;
        end  
    end
    disp('    step       x          y          f(x,y)     h*f(x,y)')
    disp(table)
    
    for i=1:it+1
        xy(i,1) = i-1;
        xy(i,2) = table(((i-1)*5)+1,2);
        xy(i,3) = table(((i-1)*5)+1,3);
    end
    disp('Table of x and y')
    disp(xy)
    %plotting
    plot(table(1:5:(it*5)+1,2), table(1:5:(it*5)+1,3));
    grid on
    grid minor
    print -dpng some_name.png;
end
%callin out our function
runge(a1,b1,c1,d1,x1,e1,h1)