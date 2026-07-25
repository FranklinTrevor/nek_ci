Conjugate Heat Transfer
=======================

.. _conj_ht:

NekRS provides a built-in conjugate heat transfer (CHT) module for simulations involving conforming fluid and solid domains.
The *conj_ht* case verifies the CHT module against an analytical solution.
The computational domain is illustrated in :numref:`fig:conj_ht_geometry`.
The domain consists of a fluid channel of height :math:`H` bounded by solid plates of equal height :math:`H_p`.
The fluid channel and solid plates have length :math:`L`, and the domain is periodic in the transverse :math:`z` direction.

.. _fig:conj_ht_geometry:
.. figure:: figs/conj_ht_geometry.png
  :align: center
  :figclass: align-center
  :scale: 60%

  conj_ht geometry and boundary conditions.

Fully developed hydrodynamic and thermal conditions are assumed.
The velocity field is prescribed as the Poiseuille flow solution,

.. math::

   u(y) = Re \frac{dp}{dx} y(1-y)

A nondimensional uniform heat source, :math:`\dot{q}`, is applied throughout both solid plates.
The analytical temperature solution is

.. math::

   T_F (x,y) =  -\dot{q} Pe \frac{H_p}{H} \left[y^4 - 2y^3 + y\right] + \dot{q} \frac{H_p}{H} \left[2x + \frac{17}{10} Pe \right] \\
   T_I(x,y) = -\dot{q} Pe \frac{1}{k_r} \left[\frac{y^2}{2} + y \frac{H_p}{H} \right] + \dot{q} \frac{H_p}{H} \left[2x + \frac{17}{10} Pe \right] \\
   T_S(x,y) = -\dot{q} Pe \frac{1}{k_r} \left[\frac{y^2}{2} - y \left(1 + \frac{H_p}{H}\right) + \left(\frac{1}{2} + \frac{H_p}{H} \right) \right] + \dot{q} \frac{H_p}{H} \left[2x + \frac{17}{10} Pe \right]

where :math:`T_F` denotes the fluid temperature and :math:`T_I` and :math:`T_S` denote the temperatures in the lower and upper solid plates, respectively.
The conductivity ratio is defined as :math:`k_r=k_s/k_f`, where :math:`k_s` and :math:`k_f` are the solid and fluid thermal conductivities.
The Reynolds and Peclet numbers are defined as

.. math::

  Re = \frac{\rho_f U_0 H}{\mu_f} \\
  Pe = \frac{\rho_f U_0 H c_{pf}}{k_f}

The nondimensional parameters used in the *conj_ht* CI test are summarized in :numref:`tab:setup`.

.. _tab:setup:

.. csv-table:: Case properties and simulation parameters.
   :align: center
   :header: "Parameter name","Variable","Value"
   :widths: 15, 10, 5

   "Non-dimensional channel height",":math:`H`","1"
   "Non-dimensional channel length",":math:`L`","8"
   "Non-dimensional plate height",":math:`H_p`","0.5"
   "Reynolds number",":math:`Re`","500"
   "Peclet number",":math:`Pe`","1000"
   "Heat source",":math:`\dot{q}`","1"
   "Fluid density",":math:`\rho_f`","1"
   "Fluid volumetric heat capacity",":math:`\rho_f c_{pf}`","1"
   "Solid volumetric heat capacity",":math:`\rho_s c_{ps}`","0.1"
   "Solid-to-fluid conductivity ratio",":math:`k_r`","10"
   "Pressure gradient",":math:`\frac{\partial p}{\partial x}`","0.012"

Dirichlet boundary conditions are imposed at :math:`x=0` for both the velocity and temperature fields.
An outflow boundary condition is imposed at :math:`x=L` for the velocity field.
A Neumann boundary condition derived from the analytical solution is imposed for temperature,

.. math::

  \left. \frac{k}{Pe} \nabla T \cdot \vec{n} \right|_{x=L} = \left. \frac{k}{Pe} \frac{\partial T}{\partial x} \right|_{x=L} = 2 \dot{q} \frac{H_p}{H} \frac{k}{Pe}

where :math:`\vec{n}` is the outward unit normal vector.
The conductivity is :math:`k=1` in the fluid and :math:`k=k_r` in the solid.
Insulated boundary conditions are imposed on the outer surfaces of both solid plates.
The CI test is qualified by evaluating the volume-integrated error norms of the streamwise velocity and temperature fields at steady state.
