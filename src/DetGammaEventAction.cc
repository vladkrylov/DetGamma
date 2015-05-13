#include "DetGammaEventAction.hh"
#include "DetGammaEventInformation.hh"
#include "TreeManager.hh"

#include "G4EventManager.hh"
#include "G4Event.hh"


DetGammaEventAction::DetGammaEventAction(TreeManager* tree_manager)
: G4UserEventAction(),
  fPrintModulo(10000),
  _tree_manager(tree_manager) 
{ ; } 


DetGammaEventAction::~DetGammaEventAction()
{ ; }


void DetGammaEventAction::BeginOfEventAction(const G4Event* event)
{  
  G4int eventNb = event->GetEventID();
  if (eventNb%fPrintModulo == 0) {
    G4cout << "\n---> Begin of event: " << eventNb << G4endl;
  }
 
  fpEventManager->SetUserInformation(new DetGammaEventInformation());

  _tree_manager->beginEvent(eventNb) ;
  
}


void DetGammaEventAction::EndOfEventAction(const G4Event*)
{ 
  _tree_manager->endEvent() ;
}

