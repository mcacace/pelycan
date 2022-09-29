#pragma once

#include "DerivativeMaterialInterface.h"
#include "Kernel.h"

class PelycanKernelT : public DerivativeMaterialInterface<Kernel>
{
public:
  static InputParameters validParams();
  PelycanKernelT(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  const MaterialProperty<Real> & _eps_dot;
  const MaterialProperty<Real> & _Tc;
};
