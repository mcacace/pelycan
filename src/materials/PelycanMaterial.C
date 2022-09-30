#include "PelycanMaterial.h"

#define PI 3.141592653589793238462643383279502884197169399375105820974944592308

registerMooseObject("pelycanApp", PelycanMaterial);

InputParameters
PelycanMaterial::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Material to compute stability conditions following the approach of "
                             "Housemanand Housemn 2009 (Lithos).");
  params.addRequiredCoupledVar("f", "The thickening/stretching factor varaiables.");
  params.addRequiredCoupledVar("T", "The depth average temperature.");
  params.addParam<bool>("use_old_vars", true, "Whether or not to lag variable updates in time.");
  params.addParam<Real>("So", 42e3, "Initial crustal thickness.");
  params.addParam<Real>("Lo", 140e3, "Initial lithosphere thickness.");
  params.addParam<Real>("act_Q", 0.0, "The activation energy of constitutive law.");
  params.addParam<Real>("Tl", 1350, "The temperature at the base of the lithosphere.");
  params.addParam<Real>("R_const", 8.314, "The gas constant [J/mol/K].");
  params.addParam<Real>("n_exp", 1.0, "The power law exponent of constitutive law.");
  params.addParam<Real>(
      "Co", 1.1e-24, "The strain-rate coefficient (inversely related to the effective viscosity.");
  params.addParam<Real>("diff", 1e-6, "The thermal diffusivity constant of the plate (m^2/s).");
  params.addParam<Real>("rho_m", 3.3e3, " The density of the mantle.");
  params.addParam<Real>("rho_o", 3.26e3, " The reference density.");
  params.addParam<Real>("grav", 9.81, "The gravitational acceleration constant.");
  params.addParam<Real>("Ho", 0.0, "The rate of crustal heat generation.");
  params.addParam<Real>("rho_c", 2.8e3, "The density of the crust.");
  params.addParam<Real>("alpha", 3e-5, "The coefficient of thermal expansion.");
  return params;
}

PelycanMaterial::PelycanMaterial(const InputParameters & parameters)
  : DerivativeMaterialInterface<Material>(parameters),
    _f(coupledValue("f")),
    _f_old(_is_transient ? coupledValueOld("f") : _zero),
    _T(coupledValue("T")),
    _T_old(_is_transient ? coupledValueOld("T") : _zero),
    _use_old_vars(getParam<bool>("use_old_vars")),
    _So(getParam<Real>("So")),
    _Lo(getParam<Real>("Lo")),
    _act_Q(getParam<Real>("act_Q")),
    _Tl(getParam<Real>("Tl")),
    _R_const(getParam<Real>("R_const")),
    _n_exp(getParam<Real>("n_exp")),
    _Co(getParam<Real>("Co")),
    _diff(getParam<Real>("diff")),
    _rho_m(getParam<Real>("rho_m")),
    _rho_o(getParam<Real>("rho_o")),
    _grav(getParam<Real>("grav")),
    _Ho(getParam<Real>("Ho")),
    _rho_c(getParam<Real>("rho_c")),
    _alpha(getParam<Real>("alpha")),
    _L(declareProperty<Real>("thickness")),
    _E(declareProperty<Real>("potential_energy")),
    _DE(declareProperty<Real>("dpotential_energy")),
    _eps_dot(declareProperty<Real>("strain_rate")),
    _Tc(declareProperty<Real>("Tc")),
    _En(declareProperty<Real>("En")),
    _deps_dT(declareProperty<Real>("deps_dT")),
    _deps_df(declareProperty<Real>("deps_df"))
{
  computeAdimensionalConstants();
}

void
PelycanMaterial::computeAdimensionalConstants()
{
  _Q_adim = _act_Q / _R_const / _Tl;
  _h_ratio = _So / _Lo;
  _energy = (_Co * std::pow(_Lo, 2.0)) / (std::pow(PI, 2.0) * _diff);
  _energy *= std::pow(_grav * _rho_m * _Lo, _n_exp);
  _H_rate = (_Ho * std::pow(_So, 2.0)) / (_diff * _Tl);
  _rho_ratio = (_rho_m - _rho_c) / _rho_m;
  _T_ratio = (_rho_o * _alpha * _Tl) / _rho_m;
  _T_cr = 0.5 + (_H_rate / 4.0) * (1.0 - (2.0 / 3.0) * _h_ratio);
  // mooseError("H_rate = ", _H_rate, " So = ", _So, " diff = ", _diff, " Tl = ", _Tl);
}

void
PelycanMaterial::computeQpProperties()
{
  Real f_var = _use_old_vars ? _f_old[_qp] : _f[_qp];
  Real T_var = _use_old_vars ? _T_old[_qp] : _T[_qp];
  Real la = 1.0 + _rho_ratio * _h_ratio * (f_var - 1.0);
  Real lb = 1.0 + _T_ratio * (T_var - _T_cr);
  _L[_qp] = la * lb;
  Real tca = (_H_rate / 4.0);
  Real tcb = (1.0 - (2.0 / 3.0) * f_var * _h_ratio);
  _Tc[_qp] = 0.5 + std::pow(f_var, 2.0) * tca * tcb;
  Real dea = 0.5 * _T_ratio;
  Real deb = _Tc[_qp] + _T_cr * (std::pow(_L[_qp], 2.0) - 1.0);
  Real dec = T_var * (2.0 * _L[_qp] - 1.0);
  Real ded = _L[_qp] - 1.0;
  Real dee = (_H_rate / 12.0);
  Real def = std::pow(f_var, 2.0);
  Real deg = (2.0 - f_var * _h_ratio);
  Real deh = 2.0 - _h_ratio;
  _DE[_qp] = dea * (deb - dec - std::pow(ded, 2.0) -
                    dee * (def * std::pow(deg, 2.0) - std::pow(deh, 2.0)));
  Real ea = 0.5 * _rho_ratio;
  Real eb = _L[_qp] - f_var * _h_ratio;
  Real ec = 1.0 - _h_ratio;
  Real ed = 0.5 * (1.0 - _rho_ratio);
  Real ee = std::pow(_L[_qp], 2.0) - 1.0;
  _E[_qp] = ea * (std::pow(eb, 2.0) - std::pow(ec, 2.0)) + ed * ee + _DE[_qp];
  Real epsa = 1.0 / _T_cr - 1.0 / T_var;
  Real ratio = _E[_qp] / _L[_qp];
  Real nexp = _n_exp;
  if (ratio < 0.0 && (_n_exp > 1 && _n_exp < 2))
    nexp = 1.0;
  _eps_dot[_qp] = _energy * std::pow(ratio, nexp) * std::exp(_Q_adim * epsa);
  _En[_qp] = _energy;
  computeKernelMaterialProperties();
}

void
PelycanMaterial::computeKernelMaterialProperties()
{
  // derivative of strain rate wrt temperature
  Real dL_dT = _T_ratio * (1.0 + _rho_ratio * _h_ratio * (_f[_qp] - 1.0));
  Real dE_dT = -0.5 * _T_ratio * (2.0 * _T[_qp] * dL_dT + 2.0 * _L[_qp] - 1.0);
  Real dEL_dT = _n_exp * std::pow(_E[_qp] / _L[_qp], _n_exp - 1.0) *
                (1.0 / std::pow(_L[_qp], 2.0)) * (_L[_qp] * dE_dT - _E[_qp] * dL_dT);
  Real expa = 1.0 / _T_cr - 1.0 / _T[_qp];
  Real dexp_dT = std::exp(_energy * expa) * _energy * std::pow(1.0 / _T[_qp], 2.0);
  Real epsa = dEL_dT * std::exp(_energy * expa);
  Real epsb = std::pow(_E[_qp] / _L[_qp], _n_exp) * dexp_dT;
  _deps_dT[_qp] = _energy * (epsa + epsb);
  // derivative of strain rate wrt thicknening/stretching factor
}
