[Mesh]
  [./mesh]
    type = GeneratedMeshGenerator
    dim = 1
  [../]
[]

[Functions]
  [./f_ini]
    type = ParsedFunction
    value = 'fs+dev'
    vars = 'fs dev'
    vals = '1 0.4'
  [../]
  [./T_ini]
    type = ParsedFunction
    value = 'Ts+dev'
    vars = 'Ts dev'
    vals = '0.5 0.15'
  [../]
[]

[Variables]
  [./f]
    [./InitialCondition]
      type = FunctionIC
      function = f_ini
    [../]
  [../]
  [./T]
    [./InitialCondition]
      type = FunctionIC
      function = T_ini
    [../]
  [../]
[]

[Kernels]
  [./KernelTimeF]
    type = PelycanKernelTime
    variable = f
  [../]
  [./KernelF]
    type = PelycanKernelF
    variable = f
  [../]
  [./KernelTimeT]
    type = PelycanKernelTime
    variable = T
  [../]
  [./KernelT]
    type = PelycanKernelT
    variable = T
  [../]
[]

[Materials]
  [./mat]
    type = PelycanMaterial
     T = T
     f = f
     Lo = 140000
     So = 42000
     act_Q = 0.0
     Tl = 1350
     R_const = 8.314
     n_exp = 1
     Co = 2.22e-23
     diff = 1e-06
     rho_m = 3300
     rho_c = 2800
     rho_o = 3.26e3
     grav = 9.81
     Ho = 0
     alpha = 3e-05
  [../]
[]

[AuxVariables]
  [./thickness]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./potential_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./dpotential_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./strain_rate]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./Tc]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./thickness_aux]
    type = MaterialRealAux
    variable = thickness
    property = thickness
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./potential_energy_aux]
    type = MaterialRealAux
    variable = potential_energy
    property = potential_energy
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./dpotential_energy_aux]
    type = MaterialRealAux
    variable = dpotential_energy
    property = dpotential_energy
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./strain_rate_aux]
    type = MaterialRealAux
    variable = strain_rate
    property = strain_rate
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./Tc_aux]
    type = MaterialRealAux
    variable = Tc
    property = Tc
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
[]

[Preconditioning]
  active = 'hypre'
  [./hypre]
    type = SMP
    petsc_options = '-snes_ksp_ew'
    petsc_options_iname = '-snes_atol -snes_rtol
                           -snes_linesearch_type
                           -ksp_type -ksp_rtol -ksp_max_it
                           -pc_type -pc_hypre_type'
    petsc_options_value = '1e-8 1e-10
                           basic
                           fgmres 1e-10 100
                           hypre boomeramg'
  [../]
  [./fieldsplit]
    type = FSP
    topsplit = 'FT'
    [./FT]
      splitting = 'F T'
      splitting_type = multiplicative
      petsc_options = '-snes_ksp_ew'
      petsc_options_iname = '-ksp_type
                             -ksp_rtol -ksp_max_it
                             -snes_type -snes_linesearch_type
                             -snes_atol -snes_stol -snes_max_it'
      petsc_options_value = 'fgmres
                             1e-12 50
                             newtonls basic
                             1e-4 0 100'
    [../]
    [./F]
      vars = 'f'
      petsc_options_iname = '-ksp_type
                             -pc_type -pc_hypre_type
                             -ksp_rtol -ksp_max_it'
      petsc_options_value = 'preonly
                             hypre boomeramg
                             1e-04 500'
    [../]
    [./T]
      vars = 'T'
      petsc_options_iname = '-ksp_type
                             -pc_type
                             -sub_pc_type -sub_pc_factor_levels
                             -ksp_rtol -ksp_max_it'
      petsc_options_value = 'fgmres
                             asm
                             ilu 1
                             1e-04 500'
    [../]
  [../]
[]

[Postprocessors]
  [./f_pp]
    type = PointValue
    point = '0.0 0.0 0.0'
    variable = f
    execute_on = 'INITIAL TIMESTEP_END'
    outputs = 'csv'
  [../]
  [./T_pp]
    type = PointValue
    point = '0.0 0.0 0.0'
    variable = T
    execute_on = 'INITIAL TIMESTEP_END'
    outputs = 'csv'
  [../]
  [./thickness_pp]
    type = PointValue
    point = '0.0 0.0 0.0'
    variable = thickness
    execute_on = 'INITIAL TIMESTEP_END'
    outputs = 'csv'
  [../]
  [./potential_energy_pp]
    type = PointValue
    point = '0.0 0.0 0.0'
    variable = potential_energy
    execute_on = 'INITIAL TIMESTEP_END'
    outputs = 'csv'
  [../]
  [./dpotential_energy_pp]
    type = PointValue
    point = '0.0 0.0 0.0'
    variable = dpotential_energy
    execute_on = 'INITIAL TIMESTEP_END'
    outputs = 'csv'
  [../]
  [./strain_rate_pp]
    type = PointValue
    point = '0.0 0.0 0.0'
    variable = strain_rate
    execute_on = 'INITIAL TIMESTEP_END'
    outputs = 'csv'
  [../]
  [./Tc_pp]
    type = PointValue
    point = '0.0 0.0 0.0'
    variable = Tc
    execute_on = 'INITIAL TIMESTEP_END'
    outputs = 'csv'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  start_time = 0
  end_time = 20
  dt = 1e-1
  automatic_scaling = true
  compute_scaling_once = false
  [./TimeIntegrator]
    type = LStableDirk4
  [../]
[]

[Outputs]
  print_linear_residuals = false
  perf_graph = true
  exodus = false
  [./csv]
    type = CSV
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
[]
