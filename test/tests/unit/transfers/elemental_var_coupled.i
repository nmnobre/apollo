[Mesh]
  type = CoupledMFEMMesh
  file = ./gold/cylinder-hex-q2.e
  dim = 3
[]

[Problem]
  type = MFEMProblem
  formulation = EBForm
  order = 2
[]

[AuxVariables]
  [./dummy_mfem]
    type = MFEMVariable
    fespace = ND
    order = SECOND
  [../]
  [./dummy_moose_elemental]
    family = MONOMIAL
    order = CONSTANT
  [../]  
[../]

[ICs]
  [./dummy_moose_elem_ic]
    type = FunctionIC
    variable = dummy_moose_elemental
    function = 2-x
  [../]  
[]

[Materials]
  [./copper]
    type = MFEMConductor
    electrical_conductivity_coeff = CopperEConductivity
    electric_permittivity_coeff = CopperPermittivity
    magnetic_permeability_coeff = CopperPermeability
    block = 1
  [../]
  [./air]
    type = MFEMConductor
    electrical_conductivity_coeff = AirEConductivity
    electric_permittivity_coeff = AirPermittivity
    magnetic_permeability_coeff = AirPermeability
    block = 2
  [../]
[]

[UserObjects]
  [./CopperEConductivity]
    type = MFEMConstantCoefficient
    value = 5.96e7
  [../]
  [./CopperPermeability]
    type = MFEMConstantCoefficient
    value = 1.25663706e-6
  [../]  
  [./CopperPermittivity]
    type = MFEMConstantCoefficient
    value = 0.0
  [../]

  [./AirEConductivity]
    type = MFEMConstantCoefficient
    value = 1.0
  [../]
  [./AirPermeability]
    type = MFEMConstantCoefficient
    value = 1.25663706e-6
  [../]
  [./AirPermittivity]
    type = MFEMConstantCoefficient
    value = 0.0
  [../]
[]

[Executioner]
  type = Transient
  dt = 0.5
  start_time = 0.0
  end_time = 0.5
[]

[Outputs]
  exodus=true
  [VisItDataCollection]
    type = MFEMVisItDataCollection
    file_base = OutputData/ElementalCoupledMFEMVarTest
  []
[]
