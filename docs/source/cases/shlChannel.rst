Poiseuille Flow (Traction Boundaries)
=====================================

.. _shlChannel:

This case verifies the traction boundary condition implementation in NekRS using fully developed Poiseuille flow in a half-channel.
The nondimensional channel height is :math:`H=1`, and periodic boundary conditions are imposed in the streamwise (:math:`x`) and spanwise (:math:`z`) directions.
A traction boundary condition is imposed at :math:`y=-1`, while a symmetry boundary condition is imposed at :math:`y=0`.
The steady-state momentum equation is

.. math::

    \frac{1}{Re} \frac{d^2 u}{d y^2} = \frac{dp}{dx}

The analytical velocity profile is

.. math::

  u(y) = 1.5 \left(1-y^2\right)

The corresponding streamwise body force is

.. math::

  f_x = \frac{3}{Re}

and the traction boundary condition is

.. math::

  \tau_w = \left.\frac{1}{Re} \frac{du}{dy}\right|_{y=-1} = -\frac{3}{Re}

Two CI modes are provided for this case.
CI mode 1 uses the channel geometry described above.
CI mode 2 verifies the same solution on a geometry rotated by :math:`45^\circ`.
Both CI modes are qualified by evaluating the volume-integrated error norm of the velocity field with respect to the analytical Poiseuille solution.
