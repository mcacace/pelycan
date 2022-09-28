#include "PelycanKernelTime.h"

registerMooseObject("pelycanApp", PelycanKernelTime);

InputParameters
PelycanKernelTime::validParams()
{
  InputParameters params = TimeDerivative::validParams();
  params.addClassDescription("Kernel to inherit Moose class TimeDerivative.");
  return params;
}

PelycanKernelTime::PelycanKernelTime(const InputParameters & parameters)
  : TimeDerivative(parameters)
{
}

Real
PelycanKernelTime::computeQpResidual()
{
  return TimeDerivative::computeQpResidual();
}

Real
PelycanKernelTime::computeQpJacobian()
{
  return TimeDerivative::computeQpJacobian();
}
