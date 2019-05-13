# SDE integrator class

import numpy as np
from numpy import random as rnd
from inspect import signature
class SdeInt(object):
    """
    A SdeInt object is a specific instance of a SDE integrator, 
    used to solve generate trajectories x(t) from a (set) of
    stochastic differential equation(s) dx=A(x, t)dt+B(x, t)dW

    A integrator is initialized with ``myintegrator = SdeInt(Seed=seed)``, 
    **Not sure yet which params it will need

    The primary use of an integrator is to perform various SDE integration algorithms,
    which are methods listed below

    Integration Methods
    ----------

    Euler
        Simple explicit Euler integration

    Parameters
    ----------
    Seed
        The seed used to initialize the pseudo-random number generator,
        which is used to calculate the wiener increment during SDE integration
        default is None, which uses the current time
 

    Attributes
    ----------
    Seed
        The seed used to initialize the pseudo-random number generator,
        which is used to calculate the wiener increment during SDE integration
        default is None, which uses the current time

    """

    def __init__(self, Seed=None):
        self.seed = Seed


    def __repr__(self):

        return 'SDE integrator object'


    def set_seed(self, Seed=None):
        """ Sets new seed for integrator"""
        self.seed = Seed



    def Euler(self, A, B, x0, tspan):
        """
        Explicit Euler integration of an Ito SDE given by: :math: 'dx=A(x, t)dt + B(x, t)dW'

        Example usage  ``xtraj = SdeInt().Euler(A, B, x0, tspan)'', 
        where A and B can be functions of x and t. 
        xtraj is evaluated at the N sample times passed in tspan.
        For n-dimensional x0, xtraj is n x N, 
        and A and B must be an n and n x m array respectively.

        In typical usage, one would produce a set of trajectories:
        `` for k in range Ntraj: xtraj[:, k] = SdeInt().Euler(A, B, x0[k], tspan)'', 
        where this example describes a 1d system where x0 is different for each trajectory
        
        Wiener increments are :math:'dw = \\sqrt{\\Delta t} \\mathcal{N}(0,1)',
        where :math:'\\mathcal{N}(0,1)' is the normal distribution with zero mean and unit variance.
        The Wiener Increment thus has :math:'\\rangle dw \\langle = 0',
        :math:'\\rangle dw^2 \\langle = dt'

        Parameters
        ----------
        A
            Drift vector A(x, t), a scalar or n-dimensional function, 
            where n is the dimension of the system of SDEs

        B
            Noise matrix B(x,t), a scalar or n x m function, 
            where m is the number of independent Wiener increments
        x0
            starting point of trajectory, a scalar or n-dimensional vector.
            To sample a distribution of initial points, sample x0 from the distribution,
            then pass to SdeInt.Euler()
        tspan
            array of equally-spaced time points at which x(t) is calculated, 
            Weiner increment assumes time intervals of equal lengths
        Returns
        ----------
        np.array
            x(t), the stochastic trajectory at each point in tspan, 
            array has length N=len(tspan) if x is 1-D, n x N if n-D
        """
        #calculate time step dT
        N=len(tspan)
        Dt=(tspan[-1]-tspan[0])/(N-1)
        sqDt=np.sqrt(Dt)

        # check if A and B are functions/constants,
        # turn A and B into f(x,t) if not
        #this might slow code down since doing all these dummy calls,
        #could potentially fix if issue 
        if callable(A) is False:
            Af = lambda x, t : A 

        elif len(signature(A).parameters) is 1:
            Af = lambda x, t : A(x) # assume if 1 parameter its x not t
        else:
            Af = A

        if callable(B) is False:
            B = lambda x, t : B

        elif len(signature(B).parameters) is 1:
            Bf = lambda x, t : B(x) # assume if 1 parameter its x not t
        else:
            Bf = B
        
        Asamp = Af(x0, tspan[0])
        Bsamp = Bf(x0, tspan[0])
        #check if x0 is scalar, get n, m if vector
        if np.isscalar(x0):
            n = 1
            if np.isscalar(Asamp) is False:
                raise ValueError(
                    """The initial state is a scalar but A has {1:d} dimensions
                    """.format(len(Asamp) ))
            if np.isscalar(Bsamp):
                m=1
            else: 
                (nb, m)=Bsamp.shape
                if nb != n:
                    raise ValueError(
                        """The initial state has {0:d} dimensions but B is ({1:d}, {2:d})
                        """.format(n, nb, m ))   


        else:
            n = x0.size
            if len(Asamp) != n:
                raise ValueError(
                    """The initial state has {0:d} dimensions but A has {1:d}
                    """.format(n,len(Asamp) ))

            (nb, m) = Bsamp.shape
            if nb != n:
                raise ValueError(
                    """The initial state has {0:d} dimensions but B is ({1:d}, {2:d})
                    """.format(n, nb, m ))            

        #set seed for Weiner increment
        local_state = rnd.RandomState(self.seed) #should allow to parallelize
    
        if n==m==1: # all scalars, i think array multiplication is considerably slower
            x = np.zeros(N)
            x[0]=x0
            #generate Weiner increments
            Weiners =  local_state.normal(size=N-1)

            for k in range(N-1):
                # do forward step!
                x[k+1] = x[k] + Af(x[k], tspan[k])*Dt + sqDt*Bf(x[k], tspan[k]) * Weiners[k]
        else: 
        # if n =1, multiple Weiner increments for 1 d system, useful if complex noise
        # for n>1, A(x_k,t_k) and B(x_k, t_k) @ dW are both n element arrays 
            x = np.zeros((n, N))
            x[:, 0]=x0
            #generate Weiner increment
            Weiners = local_state.normal(size=(m, N-1))

            for k in range(N-1):
                # do forward step!
                x[:, k+1] = x[:, k] + Af(x[:, k], tspan[k])*Dt + sqDt*Bf(x[:, k], tspan[k]) @ Weiners[:, k]
       
       # return trajectory, remove unnecessary dimensions
        return x.squeeze()


    def SemiEuler(self, As, B, x0, tspan, Niters = 3, Ito = False):
        """
        ## Rewrite later, note: 
        Need to pass stratanovich drift vector to agree with equivalent Ito SDE,
        :math: 'A^{(s)}_i = A_i - B_{kj}\\partial_{x_k}B_{ij}'
        ##

        Semi-implicit Euler integration of an Stratanovich SDE given by: 
        :math: 'dx=A^{(s)}(x, t)dt + B(x, t)dW'

        Example usage  ``xtraj = SdeInt().Euler(A, B, x0, tspan)'', 
        where A and B can be functions of x and t. 
        xtraj is evaluated at the N sample times passed in tspan.
        For n-dimensional x0, xtraj is n x N, 
        and A and B must be an n and n x m array respectively.

        In typical usage, one would produce a set of trajectories:
        `` for k in range Ntraj: xtraj[:, k] = SdeInt().Euler(A, B, x0[k], tspan)'', 
        where this example describes a 1d system where x0 is different for each trajectory
        
        Wiener increments are :math:'dw = \\sqrt{\\Delta t} \\mathcal{N}(0,1)',
        where :math:'\\mathcal{N}(0,1)' is the normal distribution with zero mean and unit variance.
        The Wiener Increment thus has :math:'\\rangle dw \\langle = 0',
        :math:'\\rangle dw^2 \\langle = dt'

        Parameters
        ----------
        A
            Drift vector A(x, t), a scalar or n-dimensional function, 
            where n is the dimension of the system of SDEs

        B
            Noise matrix B(x,t), a scalar or n x m function, 
            where m is the number of independent Wiener increments
        x0
            starting point of trajectory, a scalar or n-dimensional vector.
            To sample a distribution of initial points, sample x0 from the distribution,
            then pass to SdeInt.Euler()
        tspan
            array of equally-spaced time points at which x(t) is calculated, 
            Weiner increment assumes time intervals of equal lengths
        Returns
        ----------
        np.array
            x(t), the stochastic trajectory at each point in tspan, 
            array has length N=len(tspan) if x is 1-D, n x N if n-D
        """
        #calculate time step dT
        N=len(tspan)
        Dt=(tspan[-1]-tspan[0])/(N-1)
        sqDt=np.sqrt(Dt)

        # check if A and B are functions/constants,
        # turn A and B into f(x,t) if not
        #this might slow code down since doing all these dummy calls,
        #could potentially fix if issue 
        if callable(As) is False:
            Af = lambda x, t : As

        elif len(signature(As).parameters) is 1:
            Af = lambda x, t : As(x) # assume if 1 parameter its x not t
        else:
            Af = As

        if callable(B) is False:
            B = lambda x, t : B

        elif len(signature(B).parameters) is 1:
            Bf = lambda x, t : B(x) # assume if 1 parameter its x not t
        else:
            Bf = B
        
        Asamp = Af(x0, tspan[0])
        Bsamp = Bf(x0, tspan[0])
        #check if x0 is scalar, get n, m if vector
        if np.isscalar(x0):
            n = 1
            if np.isscalar(Asamp) is False:
                raise ValueError(
                    """The initial state is a scalar but A has {1:d} dimensions
                    """.format(len(Asamp) ))
            if np.isscalar(Bsamp):
                m=1
            else: 
                (nb, m)=Bsamp.shape
                if nb != n:
                    raise ValueError(
                        """The initial state has {0:d} dimensions but B is ({1:d}, {2:d})
                        """.format(n, nb, m ))   


        else:
            n = x0.size
            if len(Asamp) != n:
                raise ValueError(
                    """The initial state has {0:d} dimensions but A has {1:d}
                    """.format(n,len(Asamp) ))

            (nb, m) = Bsamp.shape
            if nb != n:
                raise ValueError(
                    """The initial state has {0:d} dimensions but B is ({1:d}, {2:d})
                    """.format(n, nb, m ))            

        #set seed for Weiner increment
        local_state = rnd.RandomState(self.seed) #should allow to parallelize
    
        if n==m==1: # all scalars, I think array multiplication is considerably slower
            x = np.zeros(N)
            x[0]=x0
            #generate Weiner increments
            Weiners =  local_state.normal(size=N-1)

            for k in range(N-1):
                xtemp = x[k]
                for l in range(Niters):
                    xtemp = x[k] + Af(xtemp, tspan[k])*Dt/2  + sqDt*Bf(xtemp, tspan[k])*Weiners[k]/2
                x[k+1] = 2*xtemp-x[k]
        else: 
        # if n =1, multiple Weiner increments for 1 d system, useful if complex noise
        # for n>1, A(x_k,t_k) and B(x_k, t_k) @ dW are both n element arrays 
            x = np.zeros((n, N))
            x[:, 0]=x0
            #generate Weiner increment
            Weiners = local_state.normal(size=(m, N-1))

            for k in range(N-1):
                xtemp = x[:, k]
                for l in range(Niters):
                    xtemp = x[:, k] + Af(xtemp, tspan[k])*Dt/2  + sqDt*Bf(xtemp, tspan[k]) @ Weiners[:, k]/2
                x[:, k+1] = 2*xtemp-x[:, k]
       # return trajectory, remove unnecessary dimensions
        return x.squeeze()

    def Milstein(self, A, B, x0, tspan, Bdot):
        """
        Pretend it works
        """
        return self.Euler(A, B, x0, tspan)


