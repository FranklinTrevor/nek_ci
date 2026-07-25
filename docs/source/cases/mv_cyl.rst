Moving Cylinder (Low-Mach)
==========================

.. _mv_cyl:

The *mv_cyl* case verifies the low-Mach and moving mesh capabilities in NekRS using a moving piston in a quasi-two-dimensional domain.
The piston is located at :math:`y=-L_y`, while a stationary wall is located at :math:`y=0`.
The domain has width :math:`L_x`, and symmetry boundary conditions are imposed at :math:`x=\{0,L_x\}`.
Under the ideal gas assumption, the nondimensional low-Mach governing equations are

.. math::

  \nabla \cdot \vec{v} = \frac{1}{T} \frac{DT}{Dt} - \frac{1}{p_t} \frac{dp_t}{dt} = Q \\
  \rho \left(\frac{\partial \vec{v}}{\partial t} + \vec{v} \cdot \nabla \vec{v} \right) = - \nabla p_1 +  \frac{1}{Re} \nabla \cdot \left(2\boldsymbol{\underline{S}} - \frac{2}{3} Q \boldsymbol{\underline{I}} \right) \\
  \rho c_p \frac{DT}{Dt} = \frac{1}{Pe} \nabla \cdot \nabla T + \frac{\gamma - 1}{\gamma} \frac{dp_t}{dt} \\
  p_t = \rho T

where :math:`Q` is the thermal divergence, :math:`p_t` is the thermodynamic pressure, :math:`p_1` is the hydrodynamic pressure, :math:`\gamma` is the ratio of specific heats, :math:`\boldsymbol{\underline{S}}=\frac{1}{2}\left(\nabla\vec{v}+\nabla\vec{v}^{T}\right)` is the strain-rate tensor, and :math:`\boldsymbol{\underline{I}}` is the identity tensor.
The Reynolds and Peclet numbers are denoted by :math:`Re` and :math:`Pe`, respectively.

The piston is prescribed a sinusoidal velocity,

.. math::

  v_p(t) = A \sin(\omega t)

where :math:`A` is the piston velocity amplitude.
The flow remains isentropic, and the analytical solution is

.. math::

  V(t) = V_0 + A_p A \frac{(\cos(\omega t) - 1)}{\omega} \\
  p_t(t) = p_0 \left(\frac{V_0}{V(t)}\right)^\gamma \\
  \frac{d p_t}{d t} = \gamma p_0 A_p \frac{V_0}{V(t)^{\gamma+1}} v_p(t) \\
  y_p(t) = -\frac{1}{2}\left(1+\cos(\omega t)\right) \\
  Q = \left(\frac{\gamma-1}{\gamma}-1 \right) \frac{1}{p_t(t)} \frac{dp_t}{dt} \\
  \overline{T}(t) = p_t(t)^{\frac{\gamma-1}{\gamma}}

Here, :math:`y_p(t)` is the piston location, :math:`V_0` is the initial domain volume, :math:`V(t)` is the instantaneous domain volume, :math:`p_0` is the initial thermodynamic pressure, :math:`A_p` is the piston surface area, and :math:`\overline{T}(t)` is the spatially averaged temperature.

The CI tests are qualified by evaluating the absolute errors in :math:`\{V(t),dV(t)/dt,p_t(t),dp_t/dt,y_p(t),\overline{T}(t)\}` at a specified time.
Because these quantities are spatially uniform, the case also verifies the expected temporal convergence rate.

For CI modes 1, 2, and 5, the mesh velocity is prescribed analytically as

.. math::

  v_m(y,t) = v_p(t) \frac{y-y_{max}(t)}{y_{min}(t)-y_{max}(t)}

For CI modes 3 and 6, the mesh motion is computed using the elasticity solver with the piston velocity prescribed as a Dirichlet boundary condition.
The resulting temporal convergence is shown below.

CI Mode 1
---------

This CI mode tests:

  * Low-Mach solver.
  * Passive scalar solver.
  * Prescribed moving mesh.

Errors were computed at :math:`t=0.1` and are shown in :numref:`fig:mv_cyl_1`.

.. _fig:mv_cyl_1:
.. figure:: figs/mv_cyl_1.png
  :align: center
  :figclass: align-center
  :scale: 15%

  Temporal convergence for the *mv_cyl* case, CI mode 1.

CI Mode 2
---------

This CI mode tests:

  * All capabilities exercised in CI mode 1.
  * Characteristic subcycling.

Errors were computed at :math:`t=0.1` and are shown in :numref:`fig:mv_cyl_2`.

.. _fig:mv_cyl_2:
.. figure:: figs/mv_cyl_2.png
  :align: center
  :figclass: align-center
  :scale: 15%

  Temporal convergence for the *mv_cyl* case, CI mode 2.

CI Mode 3
---------

This CI mode tests:

  * All capabilities exercised in CI mode 2.
  * Elasticity mesh solver.
  * Mesh projection.

Errors were computed at :math:`t=0.1` and are shown in :numref:`fig:mv_cyl_3`.

.. _fig:mv_cyl_3:
.. figure:: figs/mv_cyl_3.png
  :align: center
  :figclass: align-center
  :scale: 15%

  Temporal convergence for the *mv_cyl* case, CI mode 3.

CI Mode 5
---------

This CI mode verifies the capabilities of CI mode 1 on an unaligned mesh.

Errors were computed at :math:`t=0.1` and are shown in :numref:`fig:mv_cyl_5`.

.. _fig:mv_cyl_5:
.. figure:: figs/mv_cyl_5.png
  :align: center
  :figclass: align-center
  :scale: 15%

  Temporal convergence for the *mv_cyl* case, CI mode 5.

CI Mode 6
---------

This CI mode verifies the capabilities of CI mode 3 on an unaligned mesh.

Errors were computed at :math:`t=0.1` and are shown in :numref:`fig:mv_cyl_6`.

.. _fig:mv_cyl_6:
.. figure:: figs/mv_cyl_6.png
  :align: center
  :figclass: align-center
  :scale: 15%

  Temporal convergence for the *mv_cyl* case, CI mode 6.
