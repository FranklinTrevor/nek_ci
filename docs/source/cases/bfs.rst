Backward Facing Step
====================

.. _bfs:

The backward-facing step experiment of Driver \& Seegmiller [Driver1985]_ is one of the standard validation benchmarks for Reynolds-averaged Navier--Stokes (RANS) turbulence models.
The predicted reattachment length downstream of the step is commonly used as a primary metric for assessing turbulence model performance.
The *bfs* case verifies the :math:`k`-:math:`\tau` RANS model in NekRS by comparing the predicted skin friction coefficient along the downstream wall with the experimental measurements of Driver \& Seegmiller [Driver1985]_.

The step is located at the origin, and the inlet and outlet boundaries are located at :math:`x/H=-4` and :math:`x/H=40`, respectively, where :math:`H` is the step height.
The upstream and downstream channel heights are :math:`8H` and :math:`9H`, respectively.
The Reynolds number based on the inlet velocity, :math:`U`, and the step height is :math:`Re=37,\!425`.
The simulation is advanced to a quasi-steady state before the validation metrics are evaluated.
:numref:`fig:bfs1` shows the resulting streamwise velocity and turbulent kinetic energy fields.

.. _fig:bfs1:

.. figure:: figs/bfs/contour.png
  :align: center
  :figclass: align-center

  Quasi-steady streamwise velocity and turbulent kinetic energy contours.

The CI test is qualified by comparing the computed skin friction coefficient along the downstream wall (:math:`x>0`) with the experimental measurements of Driver \& Seegmiller [Driver1985]_.
The skin friction coefficient is defined as

.. math::

  C_f = \frac{2\tau_w}{\rho_0 U^2}

where :math:`\tau_w` is the wall shear stress, :math:`\rho_0` is the reference density, and :math:`U` is the reference inlet velocity.
The CI test evaluates the :math:`L_2` norm of the difference between the computed and experimental skin friction coefficients,

.. math::

  \|C_f-C_f^{exp}\|_{L_2}

:numref:`fig:bfs2` compares the computed skin friction coefficient profile with the experimental measurements.
A polynomial fit to the experimental data is used to evaluate the error norm for CI qualification.

.. _fig:bfs2:

.. figure:: figs/bfs/cf.png
  :align: center
  :figclass: align-center

  Comparison of the computed skin friction coefficient with the experimental measurements of Driver \& Seegmiller [Driver1985]_.
