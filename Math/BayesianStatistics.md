# Course Notes for Foundations of Data Science

<script
  src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"
  type="text/javascript">
</script>

## Counting Example
Let's consider a bag with 4 balls. The probability of reaching in and grabbing a ball that is blue is p. I want to get the sequence Blue White Blue, sampling with replacement. How many ways are there to do this? With the below fractions its easy to count out the possibilities.

| p | Number of Ways | Plausibility |
|---|---|---|
| 0 | 0 | 0 |
| .25 | 3 | .15 |
| .5 | 8 | .4 |
| .75 | 9 | .45 |
| 1 | 0 | 0 |

Now I want to find the plausibility of finding B W B B. How do I do this? We can take our existing count and multiply by the number of ways to find another B.

| p | Ways to Get BWB | Ways to get B | Plausibility|
|---|---|---|---|
| 0 | 0 | 0 | 0 |
| .25 | 3 | 1 | 3/46 |
| .5 | 8 | 2 |  16/46 |
| .75 | 9 | 3 | 27/46 |
| 1 | 0 | 4 | 0 |

## Prior Distribution
A prior distribution could be any distribution (uniform, normal, lognormal etc).
If we have some predictions to start with, we could also use a beta distribution. The Probability Density Function(PDF) for a Beta X ∼ Beta(a, b):

$f(x)
\begin{array}{cc}
  \{ & 
    \begin{array}{cc}
      \frac{1}{B(a,b)}x^{a-1} & 0<x<1 \\
      0 & \mathrm{otherwise}
    \end{array}
\end{array}
\mathrm{where \;}
B(a,b)=\int_0^1 x^{a-1}(1-x)^{b-1}dx
$

In python:
```python
import scipy.stats as stats
import numpy as np

grid_points = 1000
p_grid = np.linspace(0, 1, grid_points)
prior = stats.beta.pdf(p_grid, a=3, b=1)
```

## Posterior

Updating the prior is seemingly as simple as seen in the first example with the balls.

```python
def posterior_grid_approx(grid_points=100, success=6, tosses=9):
    p_grid = np.linspace(0, 1, grid_points)
    prior = np.repeat(5, grid_points)
    likelihood = stats.binom.pmf(success, tosses, p_grid)
    unorm_posterior = likelihood * prior
    posterior = unorm_posterior / unorm_posterior.sum()
    return p_grid, posterior
```
