#pragma once

#include "DerivativeMaterialInterface.h"

class PelycanMaterial : public DerivativeMaterialInterface<Material>
{
public:
  static InputParameters validParams();
  PelycanMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  const VariableValue & _f;
  const VariableValue & _T;
};
