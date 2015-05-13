#include "DetGammaRunAction.hh"
#include "DetGammaPrimaryGeneratorAction.hh"
#include "DetGammaDetectorConstruction.hh"
#include "DetGammaRun.hh"
#include "TreeManager.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserRunAction.hh"

DetGammaRunAction::DetGammaRunAction(TreeManager* tree_manager)
  : G4UserRunAction(), 
    _tree_manager(tree_manager)
{ }


DetGammaRunAction::~DetGammaRunAction()
{ ; }


G4Run* DetGammaRunAction::GenerateRun()
{ return new DetGammaRun; }


void DetGammaRunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  if(_tree_manager) _tree_manager->initialize() ;

}


void DetGammaRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int nofEvents = aRun->GetNumberOfEvent();

  if(_tree_manager) _tree_manager->write() ;

  if (nofEvents == 0) return;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const DetGammaPrimaryGeneratorAction* generatorAction
   = static_cast<const DetGammaPrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }
        
  // Print
  //  
  if (IsMaster())
  {
    G4cout
     << "\n--------------------End of Global Run-----------------------";
  }
  else
  {
    G4cout
     << "\n--------------------End of Local Run------------------------";
  }
  G4cout
     << "\n The run consists of " << nofEvents << " "<< runCondition
     << "\n------------------------------------------------------------\n"
     << G4endl;
}

