Poiseuille Flow (Traction Boundaries)
=====================================

.. _shlChannel:

This case is designed to specifically test the traction boundary condition in NekRS.
The test comprises Poiseuille flow simulation in a half-channel of non-dimensional height, $H=1$, and periodic boundary conditions in the streamwise (x) and lateral (z) direction.
Traction boundary condition is applied at :math:`y=-1` and symmetry at :math:`y=0`.
From the momentum balance at steady state,

.. math::

    \frac{1}{Re} \frac{d^2 u}{d y^2} = \frac{dp}{dx}

The steady state non-dimensional velocity is given by,

.. math::
  
  u(y) = 1.5 * (1.0 - y^2)

To drive the flow in streamwise direction, pressure gradient forcing term is applied based on the above solution,

.. math::

  f_x = \frac{3}{Re}

while the traction boundary condition is given by,

.. math::

  \tau_w = \left.\frac{1}{Re} \frac{du}{dy}\right|_{y=-1} = - \frac{3}{Re}

Two CI tests are evaluated for this case.
CI index 1 corresponds to the case as described above and CI 2 corresponds to the geometry rotated at a :math:`45^{\circ}` angle.
Both tests evaluate the :math:`L_2`-norm of velocity against the exact Poiseuille flow solution.
