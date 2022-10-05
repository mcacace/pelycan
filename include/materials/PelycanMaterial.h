#pragma once

#include "DerivativeMaterialInterface.h"

class PelycanMaterial : public DerivativeMaterialInterface<Material>
{
public:
  static InputParameters validParams();
  PelycanMaterial(const InputParameters & parameters);
  static MooseEnum erosionType();
  static MooseEnum sedimentationType();

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  void computeAdimensionalConstants();
  void computeKernelMaterialProperties();

  Real computeErosionRate();
  Real computeSedimentationRate();

  const VariableValue & _f;
  const VariableValue & _f_old;
  const VariableValue & _T;
  const VariableValue & _T_old;

  const bool _use_old_vars;

  // input parameters
  const Real _So;
  const Real _Lo;
  const Real _act_Q;
  const Real _Tl;
  const Real _R_const;
  const Real _n_exp;
  const Real _Co;
  const Real _diff;
  const Real _rho_m;
  const Real _rho_o;
  const Real _grav;
  const Real _Ho;
  const Real _rho_c;
  const Real _alpha;

  const bool _do_erosion_sedimentation;

  MooseEnum _erosion_type;
  Real _erosion_rate;

  MooseEnum _sedimentation_type;
  Real _sedimentation_rate;
  const Real _max_sediment_thickness;

  Real _Q_adim;
  Real _h_ratio;
  Real _energy;
  Real _H_rate;
  Real _rho_ratio;
  Real _T_ratio;
  Real _T_cr;

  // Material Properties
  MaterialProperty<Real> & _L;
  MaterialProperty<Real> & _E;
  MaterialProperty<Real> & _DE;
  MaterialProperty<Real> & _eps_dot;
  MaterialProperty<Real> & _Tc;
  MaterialProperty<Real> & _En;
  MaterialProperty<Real> & _dsediment;
  MaterialProperty<Real> & _sediment;
  const MaterialProperty<Real> & _sediment_old;

  MaterialProperty<Real> & _deps_dT;
  MaterialProperty<Real> & _deps_df;
};
