# Eikonal 
Eikonal implementation for the Advanced Methods for Scientific Computing (AMSC) Course @Polimi

**Students**:
- Sabrina Cesaroni
- Melanie Tonarelli
- Tommaso Trabacchin 

## Introduction
An Eikonal equation is a non-linear first-order partial differential equation 
that is encountered in problems of wave propagation. <br>

An Eikonal equation is one of the form:
$$  \begin{cases} 
H(x, \nabla u(x)) = 1 & \quad x \in \Omega \subset \mathbb{R}^d \\  
u(x) = g(x) & \quad x \in \Gamma \subset \partial\Omega 
\end{cases} $$
where 
- $d$ is the dimension of the problem, either 2 or 3;
- $u$ is the eikonal function, representing the travel time of a wave;
- $\nabla u(x)$ is the gradient of $u$, a vector that points in the direction of the wavefront;
- $H$ is the Hamiltonian, which is a function of the spatial coordinates $x$ and the gradient $\nabla u$.

In most cases, $H(x, \nabla u(x)) = |\nabla u(x)|_{M} = \sqrt{(\nabla u(x))^{T} M(x) \nabla u(x)} $ where $M(x)$ is a symmetric
positive definite function. In the simplest cases, $M(x) = c^2 I$ therefore the equation becomes:
$$  \begin{cases}
|\nabla u(x)| = \frac{1}{c} & \quad x \in \Omega \\  
u(x) = g(x) & \quad x \in \Gamma \subset \partial\Omega
\end{cases} $$
where $c$ represents the celerity of the wave.

## Description



## Usage


## Example
