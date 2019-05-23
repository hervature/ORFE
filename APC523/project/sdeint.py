
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
79
80
81
82
83
84
85
86
87
88
89
90
91
92
93
94
95
96
97
98
99
100
101
102
103
104
105
106
107
108
109
110
111
112
113
114
115
116
117
118
119
120
121
122
123
124
125
126
127
128
129
130
131
132
133
134
135
136
137
138
139
140
141
142
143
144
145
146
147
148
149
150
151
152
153
154
155
156
157
158
159
160
161
162
163
164
165
166
167
168
169
170
171
172
173
174
175
176
177
178
179
180
181
182
183
184
185
186
187
188
189
190
191
192
193
194
195
196
197
198
199
200
201
202
203
204
205
206
207
208
209
210
211
212
213
214
215
216
217
218
219
220
221
222
223
224
225
226
227
228
229
230
231
232
233
234
235
236
237
238
239
240
241
242
243
244
245
246
247
248
249
250
251
252
253
254
255
256
257
258
259
260
261
262
263
264
265
266
267
268
269
270
271
272
273
274
275
276
277
278
279
280
281
282
283
284
285
286
287
288
289
290
291
292
293
294
295
296
297
298
299
300
301
302
303
304
305
306
307
308
309
310
311
312
313
314
315
316
317
318
319
320
321
322
323
324
325
326
327
328
329
330
331
332
333
334
335
336
337
338
339
340
341
342
343
344
345
346
347
348
349
350
351
352
353
354
355
356
357
358
359
360
361
362
363
364
365
366
367
368
369
370
371
372
373
374
375
376
377
378
379
380
381
382
383
384
385
386
387
388
389
390
391
392
393
394
395
396
397
398
399
400
401
402
403
404
405
406
407
408
409
410
411
412
413
414
415
416
417
418
419
420
421
422
423
424
425
426
427
428
429
430
431
432
433
434
435
436
437
438
439
440
441
442
443
444
445
446
447
448
449
450
451
452
453
454
455
456
457
458
459
460
461
462
463
464
465
466
467
468

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

  The primary use of an integrator is to perform various SDE integration algorithms,
  which are methods listed below:

  Integration Methods
  ----------

  Euler
      Simple explicit Euler integration for an Ito SDE, the lowest order method, 
      with strong convergence of order 1/2 and weak convergence order 1.

  SemiEuler
      Semi-implicit Euler integration for a Statonovich SDE.  
      This has improved stability compared to the explicit Euler method,
      and has both weak and strong convergence of order 1

  Milstein
      Explicit integration of an Ito SDE following the Milstein algorithm, 
      which includes the next highest-order approximation to the stochastic integral 
      when compared with the lower order Euler algorithm.  
      Strong and weak convergence is of order 1, however stability is the same as the explicit Euler.
      The difficulty in evaluating multiple stochastic integrals means this algorithm is only worthwile in 1-D,
      for vector SDEs the explicit Euler algorithm is used instead
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

  def xt_func(self, f):
    """ helper function to make f a function of x and t, used when integrating an SDE.
        Future version may work around this, as it seems to add ns per function call
    """
    if callable(f) is False:
      return lambda x, t: f

    elif len(signature(f).parameters) is 1:
      return lambda x, t: f(x)  # assume if 1 parameter its x not t
    else:
      return f

  def Euler(self, A, B, x0, tspan):
    """
    Explicit Euler integration of an Ito SDE given by: :math: 'dx=A(x, t)dt + B(x, t)dW'

    Example usage:  ``xtraj = SdeInt().Euler(A, B, x0, tspan)'', 
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
    # calculate time step dT
    N = len(tspan)
    Dt = (tspan[-1] - tspan[0]) / (N - 1)
    sqDt = np.sqrt(Dt)

    # check if A and B are functions/constants,
    # turn A and B into f(x,t) if not
    # this might slow code down since doing all these dummy calls,
    # could potentially fix if issue
    Af = self.xt_func(A)
    Bf = self.xt_func(B)

    Asamp = Af(x0, tspan[0])
    Bsamp = Bf(x0, tspan[0])
    # check if x0 is scalar, get n, m if vector
    if np.isscalar(x0):
      n = 1
      if np.isscalar(Asamp) is False:
        raise ValueError(
            """The initial state is a scalar but A has {1:d} dimensions
                    """.format(len(Asamp) ))
      if np.isscalar(Bsamp):
        m = 1
      else:
        (nb, m) = Bsamp.shape
        if nb != n:
          raise ValueError(
              """The initial state has {0:d} dimensions but B is ({1:d}, {2:d})
                        """.format(n, nb, m ))
    else:
      n = x0.size
      if len(Asamp) != n:
        raise ValueError(
            """The initial state has {0:d} dimensions but A has {1:d}
                    """.format(n, len(Asamp) ))

      (nb, m) = Bsamp.shape
      if nb != n:
        raise ValueError(
            """The initial state has {0:d} dimensions but B is ({1:d}, {2:d})
                    """.format(n, nb, m ))

    # set seed for Weiner increment
    local_state = rnd.RandomState(self.seed)  # should allow to parallelize

    if n == m == 1:  # all scalars, i think array multiplication is considerably slower
      x = np.zeros(N)
      x[0] = x0
      # generate Weiner increments
      Weiners = local_state.normal(size=N - 1)

      for k in range(N - 1):
        # do forward step!
        x[k + 1] = x[k] + Af(x[k], tspan[k]) * Dt + sqDt * \
            Bf(x[k], tspan[k]) * Weiners[k]
    else:
      # if n =1, multiple Weiner increments for 1 d system, useful if complex noise
      # for n>1, A(x_k,t_k) and B(x_k, t_k) @ dW are both n element arrays
      x = np.zeros((n, N))
      x[:, 0] = x0
      # generate Weiner increment
      Weiners = local_state.normal(size=(m, N - 1))

      for k in range(N - 1):
        # do forward step!
        x[:, k + 1] = x[:, k] + Af(x[:, k], tspan[k]) * Dt + sqDt * Bf(x[:, k], tspan[k]) @ Weiners[:, k]

       # return trajectory, remove unnecessary dimensions
    return x.squeeze()

  def SemiEuler(self, As, B, x0, tspan, Niters=3, Ito=False, dx=1.e-3, Acorr=None):
    """
    Semi-implicit Euler integration of a Stratonovich SDE: 
    :math: 'dx = A^{(s)}(x, t)dt + B(x, t)dW'

    The Stratonovich SDE can be related to the equivalent Ito SDE via the transformation:
    :math: 'A^{(s)}(x, t)= A(x,t) +A^{(c)}'
    where the Stratonovich correction to the Ito drift vector is
    :math: 'A^{(c)}_k= -1/2\\sum_{j,k} B_{kj} \\partial_k B_{ij} '
    Which for 1-D systems reduces to:
    'A^{(c)}(x, t)= -1/2 B(x,t) \\partial_x B(x,t) '

    Example usage:  ``xtraj = SdeInt().SemiEuler(As, B, x0, tspan, Niters=3)'', 
    where As and B can be functions of x and t. 
    xtraj is evaluated at the N sample times passed in tspan.
    For n-dimensional x0, xtraj is n x N, 
    and A and B must be an n and n x m array respectively.

    ``As'' is assumed to be the Stratonovich drift vector, 
    which can be derived from the equations above if one has an Ito SDE.  
    If one has an the Ito drift vector instead, pass the kwarg ``Ito = True''.
    One can then either provide ``Acorr'' as an explicit function of x and t:
    ``xtraj = SdeInt().SemiEuler(A, B, x0, tspan, Ito=True, Acorr = Ac )'',
    or numerically approximate ``Acorr'' by differentiating B with differential dx:
    ``xtraj = SdeInt().SemiEuler(A, B, x0, tspan, Ito=True, dx = 1e-3 )''

    In typical usage, one would produce a set of trajectories:
    `` for k in range Ntraj: xtraj[:, k] = SdeInt().SemiEuler(As, B, x0, tspan)'', 

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
    Niters
        Number of times xtemp = (x[k+1]+x[k])/2 is iteratively calculated from itself
        before it is used to calculate x[k+1] = 2*xtemp-x[k].  Default is 3
    Ito
        Flag which is set to True if given an Ito SDE
    dx
        step size in x used to approximate ``Acorr'' by differentiating ``B''
    Acorr
        Correction to Ito drift vector A(x, t), used only if ``Ito = True''. Should be
        a function of dimension n.
    Returns
    ----------
    np.array
        x(t), the stochastic trajectory at each point in tspan, 
        array has length N=len(tspan) if x is 1-D, n x N if n-D
    """
    # calculate time step dT
    N = len(tspan)
    Dt = (tspan[-1] - tspan[0]) / (N - 1)
    sqDt = np.sqrt(Dt)

    # check if A and B are functions/constants,
    # turn A and B into f(x,t) if not
    # Dont worry if Af is stratonovich or ito drift vector yet
    Af0 = self.xt_func(As)
    Bf = self.xt_func(B)

    Asamp = Af0(x0, tspan[0])
    Bsamp = Bf(x0, tspan[0])
    # check if x0 is scalar, get n, m if vector
    if np.isscalar(x0):
      n = 1
      if np.isscalar(Asamp) is False:
        raise ValueError(
            """The initial state is a scalar but A has {1:d} dimensions
                    """.format(len(Asamp) ))
      if np.isscalar(Bsamp):
        m = 1
      else:
        (nb, m) = Bsamp.shape
        if nb != n:
          raise ValueError(
              """The initial state has {0:d} dimensions but B is ({1:d}, {2:d})
                        """.format(n, nb, m ))
    else:
      n = x0.size
      if len(Asamp) != n:
        raise ValueError(
            """The initial state has {0:d} dimensions but A has {1:d}
                    """.format(n, len(Asamp) ))

      (nb, m) = Bsamp.shape
      if nb != n:
        raise ValueError(
            """The initial state has {0:d} dimensions but B is ({1:d}, {2:d})
                    """.format(n, nb, m ))
    if Ito is False:
      # Af0 is stratanovich drift vector
      Af = Af0
    else:
      # Af0 is not stratonovich drift vector,add correction
      if Acorr is not None:  # know explicit function for correction
        Af = lambda x, t: Af0(x, t) + Acorr(x, t)
      else:
        if n is 1 and m is 1:
          # clearly assumes x is scalar in this form
          Af = lambda x, t: Af0(x, t) - 0.5 * Bf(x, t) * \
              (Bf(x + dx / 2) - Bf(x - dx / 2, t)) / dx
        else:
          # function to approximate n-D correction vector
          def Acorr_approx(x, t, dx, n, m):
            Ac = 1.
            return Ac
          Af = lambda x, t: Af0(x, t) + Acorr_approx(x, t, dx, n, m)

    # set seed for Weiner increment
    local_state = rnd.RandomState(self.seed)  # should allow to parallelize

    if n == m == 1:  # all scalars, I think array multiplication is considerably slower
      x = np.zeros(N)
      x[0] = x0
      # generate Weiner increments
      Weiners = local_state.normal(size=N - 1)

      for k in range(N - 1):
        xtemp = x[k]
        for l in range(Niters):
          xtemp = x[k] + Af(xtemp, tspan[k]) * Dt / 2 + sqDt * \
              Bf(xtemp, tspan[k]) * Weiners[k] / 2
        x[k + 1] = 2 * xtemp - x[k]
    else:
      # if n =1, multiple Weiner increments for 1 d system, useful if complex noise
      # for n>1, A(x_k,t_k) and B(x_k, t_k) @ dW are both n element arrays
      x = np.zeros((n, N))
      x[:, 0] = x0
      # generate Weiner increment
      Weiners = local_state.normal(size=(m, N - 1))

      for k in range(N - 1):
        xtemp = x[:, k]
        for l in range(Niters):
          xtemp = x[:, k] + Af(xtemp, tspan[k]) * Dt / 2 + sqDt * Bf(xtemp, tspan[k]) @ Weiners[:, k] / 2
        x[:, k + 1] = 2 * xtemp - x[:, k]
       # return trajectory, remove unnecessary dimensions
    return x.squeeze()

  def Milstein(self, A, B, x0, tspan, dx=1.e-3, C=None):
    """
    Milstein Algorithm for explicit integration of a scalar Ito SDE, 
    which has strong convergence of order 1.  It improves the Euler algorithm 
    by including the next highest-order term in the stochastic integral approximation.

    For a general n-D system, this next highest term is: 
    :math: '\\sum_{j, j'} C_{i,j,j] \\Delta W_{j, j'}'
    where the multiple stochastic integral :math: '\\Delta W_{j, j'}' 
    is in general difficult to evaluate.  For a 1-D system this simplifies to:
    :math: ' C(x,t) (\\Delta W(t)^2-\\Delta t)'
    where 
    :math: ' C(x,t) = 1/2 B(x,t) \\partial_x B(x,t) '

    Example usage:  ``xtraj = SdeInt().Milstein(s, B, x0, tspan, C=C)'', 
    where A and B can be functions of x and t. 
    xtraj is evaluated at the N sample times passed in tspan.
    C is the additional Milstein correction term defined above, 
    also a function of x and t.

    If an explicit form of C is not given, C is approximated 
    by differentiating B with differential dx:
    ``xtraj = SdeInt().Milstein(A, B, x0, tspan, dx = 1e-3 )''

    If given an N-dimensional SDE, runs the explicit Euler integrator instead
    Future versions may attempt to evaluate the multiple stochastic integral,
    but this is not commonly done

    In typical usage, one would produce a set of trajectories:
    `` for k in range Ntraj: xtraj[:, k] = SdeInt().Milstein(A, B, x0, tspan)'', 

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
    # calculate time step dT
    N = len(tspan)
    Dt = (tspan[-1] - tspan[0]) / (N - 1)
    sqDt = np.sqrt(Dt)

    # check if A and B are functions/constants,
    # turn A and B into f(x,t) if not
    # this might slow code down since doing all these dummy calls,
    # could potentially fix if issue
    Af = self.xt_func(A)
    Bf = self.xt_func(B)

    Asamp = Af(x0, tspan[0])
    Bsamp = Bf(x0, tspan[0])
    # check if x0 is scalar, get n, m if vector
    if np.isscalar(x0):
      n = 1
      if np.isscalar(Asamp) is False:
        raise ValueError(
            """The initial state is a scalar but A has {1:d} dimensions
                    """.format(len(Asamp) ))
      if np.isscalar(Bsamp):
        m = 1
      else:
        (nb, m) = Bsamp.shape
        if nb != n:
          raise ValueError(
              """The initial state has {0:d} dimensions but B is ({1:d}, {2:d})
                        """.format(n, nb, m ))
    else:
      n = x0.size
      if len(Asamp) != n:
        raise ValueError(
            """The initial state has {0:d} dimensions but A has {1:d}
                    """.format(n, len(Asamp) ))

      (nb, m) = Bsamp.shape
      if nb != n:
        raise ValueError(
            """The initial state has {0:d} dimensions but B is ({1:d}, {2:d})
                    """.format(n, nb, m ))
    if C is None:
      # if dont have preset C matrix
      C = lambda x, t: 0.5 * \
          Bf(x, t) * (Bf(x + dx / 2, t) - Bf(x - dx / 2, t)) / dx

    # set seed for Weiner increment
    local_state = rnd.RandomState(self.seed)  # should allow to parallelize

    if n == m == 1:  # all scalars, i think array multiplication is considerably slower
      x = np.zeros(N)
      x[0] = x0
      # generate Weiner increments
      Weiners = local_state.normal(size=N - 1)

      for k in range(N - 1):
        # do forward step!
        x[k + 1] = x[k] + Af(x[k], tspan[k]) * Dt + sqDt * Bf(x[k], tspan[k]) * \
            Weiners[k] + Dt * C(x[k], tspan[k]) * (Weiners[k]**2 - 1)
      return x.squeeze()

    else:  # Don't have code to evaluate multiple stochastic integrals
      return self.Euler(A, B, x0, tspan)
