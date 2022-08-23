a1 = -5.8;
b1 = 4.7;
c1 = -0.9;
d1 = 1.9;%initial condition
x1 = 0;%x1x<e1
e1 = 9;%x1x<e1
h1 = 0.2;%step

function eu = euler(a,b,c,d,x,e,h)
    it = e/h; %number of our iterations
    ANS = ['Our function with a=',num2str(a),' b=',num2str(b),' c=',num2str(c),' with initial     	condition x=',num2str(d),
         ' where: ',num2str(x),'<x<',num2str(e),' and our step h=',num2str(h),'. Number of iterations:  	 ',num2str(it)];
    disp(ANS)
    table = zeros(it+1,5);
    for i = 1:it+1
        table(i,1)=i-1;
        table(i,2)=x;
        f = a*d + b*x + c*(x^2); %This is our function
        table(i,4) = f;
        deltaf = f * h;
        table(i,5) = deltaf;
        x = x+h;
        table(i,3) = d;
        d = d + deltaf;
    end
    disp('Our final solution')
    disp('    step       x          y          f(x,y)     h*f(x,y)')
    disp(table)
    plot(table(1:it,2), table(1:it,3));
    grid on
    grid minor
    print -dpng some_name.png;
end

euler(a1,b1,c1,d1,x1,e1,h1)