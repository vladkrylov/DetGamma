#ifndef DetGammaSteppingAction_h
#define DetGammaSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

/// Stepping action class
/// 

class G4LogicalVolume;
class TreeManager ;

class DetGammaSteppingAction : public G4UserSteppingAction
{
public:
  DetGammaSteppingAction(TreeManager* tree_manager=0);
  virtual ~DetGammaSteppingAction();
  
  // method from the base class
  virtual void UserSteppingAction(const G4Step*);
  
private:
  TreeManager* _tree_manager ;
};


#endif
