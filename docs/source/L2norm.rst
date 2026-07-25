Error Norms
===========

.. _error-norm-calculation:

Many verification cases in the NekRS CI test suite evaluate the error between
a numerical solution field and an exact or manufactured solution. For a field
:math:`\phi`, the volume-integrated :math:`L^N` error norm is defined as

.. math::

   \epsilon_{\phi}
   =
   \left[
   \int_{\Omega}
   \left|
   \phi - \phi_{\mathrm{exact}}
   \right|^N
   \, \mathrm{d}\Omega
   \right]^{1/N},

where :math:`\Omega` is the computational domain,
:math:`\phi` is the numerical solution,
:math:`\phi_{\mathrm{exact}}` is the exact or manufactured solution, and
:math:`N` is the order of the norm.

In the Cardinal input files, these errors are evaluated directly on the NekRS
computational mesh using the ``NekVolumeNorm`` postprocessor. The value of
:math:`N` is user selectable, although the majority of verification cases in
this test suite employ the :math:`L^2` norm.

The solution fields used for verification may include the velocity components
:math:`\{u,v,w\}`, pressure :math:`p`, temperature :math:`T`, and one or more
passive scalars.

Not every case is evaluated using a field error norm. Turbulence validation
cases instead compare integral quantities, such as skin-friction coefficient,
wall shear stress, or friction velocity, against reference data.
