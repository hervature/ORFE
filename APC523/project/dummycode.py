x[:, 0]=x0
Weiners = local_state.normal(size=(m, N-1))
for k in range(N-1):
    xtemp = x[:, k]
    for l in range(Niters):
        xtemp =  x[:, k] \
        + Dt*Af(xtemp, tspan[k]+Dt/2)/2  \
        + np. sqrt(Dt)*Bf(xtemp, tspan[k]+Dt/2) \
        @ Weiners[:, k]/2
    x[:, k+1] = 2*xtemp-x[:, k]
