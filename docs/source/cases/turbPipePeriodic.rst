Turbulent Pipe Flow (LES)
=========================

.. _turbPipePeriodic:

This case verifies the large-eddy simulation (LES) capability in NekRS using fully developed turbulent pipe flow.
The pipe has a nondimensional diameter :math:`D=1` and a bulk Reynolds number :math:`Re_b=19000`, defined as

.. math::

  Re_b = \frac{U_b D}{\nu}

where :math:`U_b` is the bulk velocity and :math:`\nu` is the kinematic viscosity.
The corresponding friction Reynolds number is approximately :math:`Re_\tau=550`.
Subgrid-scale modeling is performed using the high-pass filtering approach of Stolz *et al.* [Stolz2005]_ with a cut-off wavenumber of :math:`N-1` for construction of the low-pass filter and a filter strength of 10.

The simulation is qualified by computing the time-averaged velocity field and evaluating the friction velocity,

.. math::

  u_\tau = \sqrt{\frac{\tau_w}{\rho}}

where the wall shear stress is

.. math::

  \tau_w = 2 \mu \left| \underline{S} \cdot \vec{n} \right|_w

and the mean strain-rate tensor is

.. math::

  \underline{S} = \frac{1}{2} \left(\nabla \overline{\vec{v}} + \nabla \overline{\vec{v}}^T \right)

Here, :math:`\overline{\vec{v}}` is the time-averaged velocity, :math:`\tau_w` is the computed wall shear stress, :math:`\rho` is the fluid density, :math:`\mu` is the dynamic viscosity, :math:`\underline{S}` is the strain-rate tensor, and :math:`\vec{n}` is the outward unit normal vector at the wall.

The computed friction velocity is compared with the direct numerical simulation (DNS) reference of El Khoury *et al.* [Khoury2013]_ using

.. math::

  err = |u_\tau - u_{\tau_{DNS}}|

where

.. math::

  u_{\tau_{DNS}} = 5.79 \times 10^{-2}.
