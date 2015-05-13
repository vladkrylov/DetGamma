#include "DetGammaPhysicsList.hh"
#include "DetGammaEMPhysics.hh"

#include "G4Cerenkov.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4OpticalPhysics.hh"
#include "G4OpticalProcessIndex.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4Cerenkov.hh"
#include "G4ProcessManager.hh"

#include "G4SystemOfUnits.hh"

DetGammaPhysicsList::DetGammaPhysicsList(G4int verbose) : G4VModularPhysicsList()
{
  SetVerboseLevel(verbose);

  // General Physics                                                                                                                                                       
  // default cut value  (1.0mm)
  defaultCutValue = 1.0*mm;

  // EM Physics
  RegisterPhysics( new DetGammaEMPhysics("standard EM"));

  // Optical Physics
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  RegisterPhysics( opticalPhysics );

  //  opticalPhysics->SetScintillationYieldFactor(1.0);
  //  opticalPhysics->SetScintillationExcitationRatio(0.0);

  opticalPhysics->SetMaxNumPhotonsPerStep(300);
  opticalPhysics->SetMaxBetaChangePerStep(10.0);
  opticalPhysics->SetTrackSecondariesFirst(kCerenkov,true);
  //  opticalPhysics->SetTrackSecondariesFirst(kScintillation,true);

  G4Cerenkov* fCerenkovProcess = opticalPhysics->GetCerenkovProcess() ;

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    if (fCerenkovProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(fCerenkovProcess);
      pmanager->SetProcessOrdering(fCerenkovProcess,idxPostStep);
    }
    if (particleName == "opticalphoton") {
      pmanager->AddDiscreteProcess(opticalPhysics->GetOpAbsorptionProcess());
      pmanager->AddDiscreteProcess(opticalPhysics->GetOpRayleighProcess());
      pmanager->AddDiscreteProcess(opticalPhysics->GetOpMieHGProcess());
      pmanager->AddDiscreteProcess(opticalPhysics->GetOpBoundaryProcess());
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetGammaPhysicsList::~DetGammaPhysicsList() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetGammaPhysicsList::SetCuts(){
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
  //   the default cut value for all particle types
  SetCutsWithDefault();
  if (verboseLevel>0) DumpCutValuesTable();
}
