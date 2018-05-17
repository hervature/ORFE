clear;
clc;

m = 5;
n = 200;

A = rand(m,n);
b = rand(m,1);

% alternating projection
x_ap(:,1) = rand(n,1);
i = 1;
res_ap(1) = norm(x_ap(:,1),2);

while (size(res_ap) < 1000)
    i = i + 1;
    % project into affine set
    x_ap(:,i) = x_ap(:,i-1) + A'/(A*A')*(b-A*x_ap(:,i-1));
    
    i = i + 1;
    % project into positive orthant
    x_ap(:,i) = max(0, x_ap(:,i-1));
    
    % res_ap(ceil(i/2)) = norm(A*x_ap(:,i) - b,2);
    res_ap(ceil(i/2)) = norm(x_ap(:,i) - x_ap(:,i-1),2);
end

% Douglas-Rachford
x_dr = rand(n,1);
lambda_dr = rand(m,1);
p_dr = rand(n,1);
q_dr = rand(m,1);
eta = 0.1;
i = 1;
res_dr(1) = norm(x_dr(:,1),2);

while (size(res_dr) < 1000)
    % project onto positive orthant
    x_dr_half(:,i) = max(0, p_dr(:,i));
    % proximal for norm
    lambda_dr_half(:,i) = (q_dr(:,i) - eta*b)/(1+eta);
    
    % update full step
    tmp = inv([eye(n), eta*A'; -eta*A, eye(m)])*[2*x_dr_half(:,i)-p_dr(:,i);2*lambda_dr_half(:,i)-q_dr(:,i)];
    x_dr(:,i+1) = tmp(1:n);
    lambda_dr(:,i+1) = tmp(n+1:n+m);
    
    % update p and q
    p_dr(:,i+1) = p_dr(:,i) + x_dr(:,i+1) - x_dr_half(:,i);
    q_dr(:,i+1) = q_dr(:,i) + lambda_dr(:,i+1) - lambda_dr_half(:,i);
    
    i = i + 1;
    % res_dr(i) = norm(A*x_dr(:,i) - b,2);
    res_dr(i) = norm(x_dr(:,i) - x_dr_half(:,i-1),2);
end

semilogy(res_ap)
hold on
semilogy(res_dr)