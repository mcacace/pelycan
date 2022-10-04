#include "PelycanKernelT.h"

#define PI 3.141592653589793238462643383279502884197169399375105820974944592308

registerMooseObject("pelycanApp", PelycanKernelT);

InputParameters
PelycanKernelT::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Compute the evolution of the temperature.");
  params.addParam<Real>(
      "limit_strain_rate",
      -1e99,
      "The amount of strain rate (convergence) under which to fix the conductive heat flow at the "
      "base instead of the temperature.");
  return params;
}

PelycanKernelT::PelycanKernelT(const InputParameters & parameters)
  : DerivativeMaterialInterface<Kernel>(parameters),
    _limit_strain_rate(getParam<Real>("limit_strain_rate")),
    _eps_dot(getDefaultMaterialProperty<Real>("strain_rate")),
    _Tc(getDefaultMaterialProperty<Real>("Tc"))
{
}

Real
PelycanKernelT::computeQpResidual()
{
  Real mult = (_eps_dot[_qp] <= _limit_strain_rate) ? (0.5 * PI * _eps_dot[_qp] - 1.0 / 4.0) : 1.0;
  Real res = mult * (_u[_qp] - _Tc[_qp]) - _eps_dot[_qp] * (1.0 - _u[_qp]);
  return res * _test[_i][_qp];
}

Real
PelycanKernelT::computeQpJacobian()
{
  Real mult =
      (_eps_dot[_qp] <= _limit_strain_rate) ? ((PI / 4.0) * _eps_dot[_qp] - 1.0 / 4.0) : 1.0;
  Real jac = mult * _test[_i][_qp] * _phi[_j][_qp] + _eps_dot[_qp] * _test[_i][_qp] * _phi[_j][_qp];
  return jac;
}
