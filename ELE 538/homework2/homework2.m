clear;
clc;

m = 5;
n = 2000;

A = rand(m,n);
b = rand(m,1);
x(:,1) = zeros(n,1);
lambda = zeros(1,100);
j = 1;

for gamma=1:0.01:1.99
    clear x;
    x(:,1) = zeros(n,1);
    i = 1;
    while (norm(A*x(:,i) - b,2) > 10^-5)
        i = i + 1;
        % project into affine set
        x(:,i) = x(:,i-1) + gamma*(x(:,i-1) + A'/(A*A')*(b-A*x(:,i-1))-x(:,i-1));

        i = i + 1;
        % project into positive orthant
        x(:,i) = x(:,i-1) + gamma*(max(0, x(:,i-1))-x(:,i-1));
    end
    lambda(j) = i;
    j = j + 1;
end

plot(1:0.01:1.99, lambda)