#pragma once

#include "DerivativeMaterialInterface.h"
#include "Kernel.h"

class PelycanKernelF : public DerivativeMaterialInterface<Kernel>
{
public:
  static InputParameters validParams();
  PelycanKernelF(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  const Real _mult;

  const MaterialProperty<Real> & _eps_dot;
};
