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
    vals = '1 -0.5'
  [../]
  [./T_ini]
    type = ParsedFunction
    value = 'Ts+dev'
    vars = 'Ts dev'
    vals = '0.5 -0.2'
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
    # limit_strain_rate = -0.477
  [../]
[]

[Materials]
  [./mat]
    type = PelycanMaterial
     T = T
     f = f
     Lo = 140e3
     So = 35e3
     act_Q = 10e3 #10e3
     Tl = 1350
     R_const = 8.314
     n_exp = 1
     Co = 8.8e-23
     diff = 1e-6
     rho_m = 3.3e3
     rho_c = 2.8e3
     rho_o = 3.26e3
     grav = 9.81
     Ho = 0.0
     alpha = 3e-05
     C_erosion = 2e-11
     tau_erosion = 4.6e14
  [../]
[]

[AuxVariables]
  [./sediment]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./dsediment]
    order = CONSTANT
    family = MONOMIAL
  [../]
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
  [./En]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./sediment_aux]
    type = MaterialRealAux
    variable = sediment
    property = sediment
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./dsediment_aux]
    type = MaterialRealAux
    variable = dsediment
    property = dsediment
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
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
  [./En_aux]
    type = MaterialRealAux
    variable = En
    property = En
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
[]

[Preconditioning]
  active = 'hypre'
  [./hypre]
    type = SMP
    petsc_options = '-snes_ksp_ew'
    petsc_options_iname = '-snes_atol -snes_rtol -snes_max_it -snes_force_it
                           -snes_linesearch_type
                           -ksp_type -ksp_rtol -ksp_max_it
                           -pc_type -pc_hypre_type'
    petsc_options_value = '1e-8 1e-10 500 1
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
  [./dsed_pp]
    type = PointValue
    point = '0.0 0.0 0.0'
    variable = dsediment
    execute_on = 'INITIAL TIMESTEP_END'
    outputs = 'csv'
  [../]
  [./sed_pp]
    type = PointValue
    point = '0.0 0.0 0.0'
    variable = sediment
    execute_on = 'INITIAL TIMESTEP_END'
    outputs = 'csv'
  [../]
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
  [./En_pp]
    type = PointValue
    point = '0.0 0.0 0.0'
    variable = En
    execute_on = 'INITIAL TIMESTEP_END'
    outputs = 'csv'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  start_time = 0
  end_time = 30
  dt = 1e-2
  automatic_scaling = true
  compute_scaling_once = false
  [./TimeIntegrator]
    type = AStableDirk4
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
