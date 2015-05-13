#include "DetGammaActionInitialization.hh"
#include "DetGammaPrimaryGeneratorAction.hh"
#include "DetGammaRunAction.hh"
#include "DetGammaEventAction.hh"
#include "DetGammaSteppingAction.hh"
#include "TreeManager.hh"

DetGammaActionInitialization::DetGammaActionInitialization(TreeManager* tree_manager)
  : G4VUserActionInitialization(),
    _tree_manager(tree_manager)
{}


DetGammaActionInitialization::~DetGammaActionInitialization()
{}


void DetGammaActionInitialization::BuildForMaster() const
{
  SetUserAction(new DetGammaRunAction);
}


void DetGammaActionInitialization::Build() const
{
  SetUserAction(new DetGammaPrimaryGeneratorAction);
  SetUserAction(new DetGammaRunAction(_tree_manager));
  SetUserAction(new DetGammaEventAction(_tree_manager));
  SetUserAction(new DetGammaSteppingAction(_tree_manager));
}  


