clear;
clc;

d = 512;
iterations = 10;
% Keep track of probability of success for pair (n,s)
success = zeros(floor(d/50)+1, floor(d/50)+1);

for i=1:iterations
    for s=1:50:d
        for n=1:50:d
            % Change the random method
            X = SubDCT_Phi(n,d);

            % Generate random beta and zero the first d-s entries
            beta = randn(d, 1);
            beta(1:(d-s),1)=0;

            Y = X*beta;

            sol = l1eq_pd(0*beta, X, 0*X, Y);

            % Check if solution is close to original beta
            if(norm(l1eq_pd(0*beta, X, 0*X, Y) - beta) < 0.01)
                success(floor(n/50) + 1, floor(s/50) + 1) = success(floor(n/50) + 1, floor(s/50) + 1) + 1;
            end
        end
    end
end

dlmwrite('dct.txt', success/iterations)
colormap('jet')
imagesc((1:50:d)/d,(1:50:d)/d, success/iterations)
colorbar
xlabel('s/d')
ylabel('n/d')
title('Discrete Cosine Matrix 10 Iterations')