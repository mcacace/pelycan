#include "PelycanKernelT.h"

registerMooseObject("pelycanApp", PelycanKernelT);

InputParameters
PelycanKernelT::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Compute the evolution of the temperature.");
  return params;
}

PelycanKernelT::PelycanKernelT(const InputParameters & parameters)
  : DerivativeMaterialInterface<Kernel>(parameters),
    _eps_dot(getDefaultMaterialProperty<Real>("strain_rate")),
    _Tc(getDefaultMaterialProperty<Real>("Tc"))
{
}

Real
PelycanKernelT::computeQpResidual()
{
  Real res = (_u[_qp] - _Tc[_qp]) - _eps_dot[_qp] * (1.0 - _u[_qp]);
  return res * _test[_i][_qp];
}

Real
PelycanKernelT::computeQpJacobian()
{
  Real jac = _test[_i][_qp] * _phi[_j][_qp] + _eps_dot[_qp] * _test[_i][_qp] * _phi[_j][_qp];
  return jac;
}
