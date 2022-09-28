#pragma once

#include "TimeDerivative.h"

class PelycanKernelTime : public TimeDerivative
{
public:
  static InputParameters validParams();
  PelycanKernelTime(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
};
