Turbulent Channel Flow (RANS)
=============================

.. _ktauChannel:

This case verifies the standard :math:`k`-:math:`\tau` Reynolds-averaged Navier--Stokes (RANS) turbulence model [Tombo2024]_ using fully developed turbulent flow in an infinite half-channel.
The bulk Reynolds number is :math:`Re=43500`, based on the bulk velocity, :math:`U_b`, and the half-channel width, :math:`L`,

.. math::

  Re = \frac{U_b L}{\nu}

where :math:`\nu` is the kinematic viscosity.
The corresponding friction Reynolds number, based on the friction velocity :math:`u_\tau`, is approximately 2000 and is defined as

.. math::

  Re_\tau = \frac{u_\tau L}{\nu}

where

.. math::

  u_\tau = \sqrt{\frac{\tau_w}{\rho}}

Here, :math:`\tau_w` is the computed wall shear stress and :math:`\rho` is the fluid density.
The CI test is qualified by comparing the computed friction velocity with the direct numerical simulation (DNS) reference of Lee *et al.* [Lee2015]_ using

.. math::

  err = |u_\tau - u_{\tau_{DNS}}|

where

.. math::

  u_{\tau_{DNS}} = 4.58 \times 10^{-2}.
