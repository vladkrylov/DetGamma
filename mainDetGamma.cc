#include "DetGammaDetectorConstruction.hh"
#include "DetGammaActionInitialization.hh"
#include "DetGammaPhysicsList.hh"
#include "DetGammaMessenger.hh"
#include "TreeManager.hh"


#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "QBBC.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"
#include "G4Cerenkov.hh"

#include <stdexcept>

int main(int argc, char** argv)
{

  // Choose the Random engine
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine);  

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // create messenger (to read script files) ;
  DetGammaMessenger* messenger = new DetGammaMessenger() ;

  // initialize the TreeManager
  TreeManager*  tree_manager = new TreeManager(messenger);

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new DetGammaDetectorConstruction(messenger));

  // Physics list
  runManager->SetUserInitialization(new DetGammaPhysicsList()) ;
    
  // User action initialization
  runManager->SetUserInitialization(new DetGammaActionInitialization(tree_manager));

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // read detector configuration file
  if (argc<2)
    throw std::runtime_error("Specify detector configuration macro") ;
  
  UImanager->ApplyCommand("/control/execute " + (G4String) argv[1]);
  
  
  // Initialize G4 kernel 
  runManager->Initialize();
  
#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  
  if (argc>2) {

    // batch mode
    G4String runFile = argv[2];  
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+runFile);
  }
  else {
    // interactive mode : define UI session
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#endif
    ui->SessionStart();
  delete ui;
#endif
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
  if (visManager)  delete visManager ;
  delete runManager;

  return 0;
}

