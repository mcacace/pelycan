#include "PelycanKernelF.h"

registerMooseObject("pelycanApp", PelycanKernelF);

InputParameters
PelycanKernelF::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Compute the evolution of the thicknening/stretching.");
  params.addParam<Real>("mult", 1.0, "Multiplication factor.");
  return params;
}

PelycanKernelF::PelycanKernelF(const InputParameters & parameters)
  : DerivativeMaterialInterface<Kernel>(parameters),
    _mult(getParam<Real>("mult")),
    _eps_dot(getDefaultMaterialProperty<Real>("strain_rate"))
{
}

Real
PelycanKernelF::computeQpResidual()
{
  return _mult * _eps_dot[_qp] * _test[_i][_qp] * _u[_qp];
}

Real
PelycanKernelF::computeQpJacobian()
{
  return _mult * _eps_dot[_qp] * _test[_i][_qp] * _phi[_j][_qp];
}
