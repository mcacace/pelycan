#include "PelycanMaterial.h"

registerMooseObject("PelcanApp", PelycanMaterial);

InputParameters
PelycanMaterial::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Material to compute stability conditions following the approach of "
                             "Housemanand Housemn 2009 (Lithos).");
  params.addRequiredCoupledVar("f", "The thickening/stretching factor varaiables.");
  params.addRequiredCoupledVar("T", "The depth average temperature.");
  return params;
}

PelycanMaterial::PelycanMaterial(const InputParameters & parameters)
  : DerivativeMaterialInterface<Material>(parameters), _f(coupledValue("f")), _T(coupledValue("T"))
{
}

void
PelycanMaterial::computeQpProperties()
{
}
